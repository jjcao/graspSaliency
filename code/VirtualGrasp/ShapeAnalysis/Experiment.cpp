#include "isa_headers.h"
#include "Experiment.h"

#include <iostream>

#define M_PI_2 (M_PI * 2)
#define closeIt 999

Experiment::Experiment()
{
	this->isDone = false;
}

void Experiment::init( Mesh * src_mesh, int numberSamples, double threshold, bool screenshots, 
					  QString outputFile, FeatureSpace usingFeatureSpace, SamplingMethod samplingMethod)
{
	this->isDone = false;

	this->mesh = src_mesh;

	this->numSamples = numberSamples;
	this->fileName = outputFile;
	this->farThreshold = threshold;
	this->space = usingFeatureSpace;

	// Do we output data?
	this->isOutFile = outputFile.length();
	this->isSnapshots = screenshots;

	this->sampler = NULL;
	this->sampleMethod = samplingMethod;
}

void Experiment::forceRun()
{
	run();
}

void Experiment::run()
{
	// Statistics
	totalTime = Stats( QString(" * Sample %1 * ").arg(numSamples) ); 
	totalTime.start();

	// Number of faces
	int fcount = mesh->numberOfFaces();

	// For each sample point, pick different angles
	int angleSteps = 8;
	double angleStepSize = M_PI_2 / angleSteps;

	// Clear any previous observations
	//obs.clear();
	obs = Vector<Observation*>(numSamples);

	for( int i = 0; i < numSamples; i++ )
		obs[i] = new Observation();

	// Output file stream
	ofstream file;

	if(isOutFile)
	{
		file.open(fileName.toStdString().c_str());

		// Header of file
		file << "% == EXPERIMENT FILE ==" << endl;
		file << "% " << mesh->opendFileName << ", Scaled " << mesh->normalize_scale << ", NumFaces " << mesh->numberOfFaces() <<  endl;
		file << "% NumSamples " << numSamples << endl;
	}

	// Create mesh sampling engine
	if(sampler == NULL)
		sampler = new Sampler( mesh, sampleMethod );

	// Compute and smooth principle curvature
	//Curvature cu;
	//cu.computePrincipalCurvatures( mesh );

	#pragma omp parallel
	{
		// Random seed
	    srand(int(time(NULL)) ^ (omp_get_thread_num() + 1));

		#pragma omp for
		for( int i = 0; i < numSamples; i++ )
		{
			Stats singleObservation(QString("%1 %2").arg(fileName).arg(i)); singleObservation.start();

			HumanHand * h = new HumanHand();
			h->init();

			SamplePoint sp = sampler->getSample();

			while( !samplePoint(sp, h, mesh, farThreshold) )
			{
				// Get another
				sp = sampler->getSample();
			}

			obs[i]->set(sp.weight, h);

			// Relocate camera
			if(isSnapshots){
				//mainWindow->ui.viewer->camera()->setPosition(triCenter 
				// + (mesh->normalize_scale * triNor));
				emit updateViewer();
				emit saveSnapshot();
			}
			
			singleObservation.print();
		}
	}

	totalTime.print();

	// Output observations to file
	if(isOutFile) 
	{
		for(int i = 0; i < numSamples; i++)
		{
			file << obs[i]->hand->toString( space ) << obs[i]->weight << endl; break;
		}

		file << "% END" << endl;
		file.close();
	}

	// Add to widget's list of observations
	if(saWidget != NULL)
	{
		for(int i = 0; i < numSamples; i++)
			emit addObservation();
	}

	this->isDone = true;
}

bool Experiment::samplePoint(const SamplePoint & sp, HumanHand * h, Mesh * mesh, double far_threshold)
{
	// Start with an open hand
	h->mat.setToIdentity();
	h->openAll( -closeIt );

	bool goodInteraction = true;

	// Hand's regular normal
	Vec handNor (1.0, 0, 0);

	Face * face = mesh->f(sp.findex);
	int v1 = face->VIndex(0), v2 = face->VIndex(1), v3 = face->VIndex(2);

	// ROTATION: first is rotation needed to sample's normal
	qglviewer::Quaternion rotateToNormal(handNor, sp.n);

	// Align with Principle Curvature
	//Vec interpDir = (sp.u*cu.pdir1[v1]) + (sp.v*cu.pdir1[v2]) + ((1.0 - (sp.u + sp.v))*cu.pdir1[v3]);
	//double alignAngle = Vertex::singed_angle(rotateToNormal.rotate( Vec(1,0,0) ), interpDir, sp.n);				
	//qglviewer::Quaternion rotatePalm ( sp.n, alignAngle + (rand()%4)*(M_PI/2.) );
	
	// Align randomly on sample point
	qglviewer::Quaternion rotatePalm(handNor, uniform(0, M_PI_2));

	qglviewer::Quaternion res = rotateToNormal * rotatePalm;

	// TRANSLATION
	h->mat.translate(sp.pos.x, sp.pos.y, sp.pos.z);

	// Apply Rotation
	h->mat.rotate(QQuaternion(res[3], res[0], res[1], res[2]));
	//h->mat.rotate(180,0,0,1); // can be changed; within hand structure maybe?

	// Move center to a good palm location
	Vec grabCenter = -(h->getFinger(1)->maxExtent() * 0.8 + h->getFinger(1)->anchorPos);
	h->mat.translate(grabCenter.x, grabCenter.y, grabCenter.z);

	// DEBUG:
	/*Vec vv = sp.pos;
	testLines1.push_back(Line(vv,res.rotate(Vec(1,0,0)),3,0,Color4(255,0,0)));
	testLines2.push_back(Line(vv,res.rotate(Vec(0,1,0)),3,0,Color4(0,255,0)));
	testLines3.push_back(Line(vv,res.rotate(Vec(0,0,1)),3,0,Color4(0,0,255)));
	testLines2.push_back( Line( sp.pos, sp.pos+(sp.n*10) ) );
	testPoints2.push_back( sp.pos );*/
	
	const double step = h->getFingerThick() * 0.5;
	double totalStep = 0.0;

	// Move until not intersecting
	while( h->intersectWithMesh( mesh ) )
	{
		h->mat.translate(step, 0, 0);

		// Filter out bad results using distance
		totalStep += step;

		if(abs(totalStep) > far_threshold)
		{
			goodInteraction = false;
			break;
		}
	}

	if(goodInteraction)
	{
		// Close hand completely, then open it slowly till collision, then close it
		h->openAll( closeIt );
		h->openUntilNonCollide(mesh);
		h->closeUntilCollide(mesh);
	}

	return goodInteraction;
}
