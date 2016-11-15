//#include "isa_headers.h"
#include "HumanHand.h"

#define pow2(x) ((x)*(x))

double defaultAngleStep = 2.0;

HumanHand::HumanHand()
{
	this->isReady = false;
	this->shape = NULL;
	this->parent = NULL;
	this->isReflect = false;
}

/*HumanHand & HumanHand::operator=( const HumanHand& from )
{
	// Rigid part stuff..
	if(from.shape)	shape = from.shape->Clone();

	if(from.parent == NULL) 
		parent = NULL;
	else
		parent = from.parent;

	name = from.name;
	scale = from.scale;
	mat = from.mat;
	rot = from.rot;
	color = from.color;
	anchorPos = from.anchorPos;
	anchorRotations = from.anchorRotations;
	isReflect = from.isReflect;
	reflectAxis = from.reflectAxis;

	for(int i = 0; i < (int)from.child.size(); i++){
		child.push_back(new RigidPart(from.child[i]));
		child.back()->parent = this;
	}

	fingCount = from.fingCount;
	fingerThick = from.fingerThick;
	fingerWidth = from.fingerWidth;
	jointCount = from.jointCount;
	HB = from.HB;
	HL = from.HL;
	isReady = from.isReady;

	return *this;
}*/

void HumanHand::init( double scale /*= 1.0*/, double handLength /*= 19.41*/, double handWidth /*= 9.04*/, RigidPart * parent /*= NULL*/)
{
	this->isReady = false;

	// Scale parameters
	HL = handLength * scale;
	HB = handWidth * scale;

	// Fingers
	fingCount = 4;
	jointCount = 4;

	fingerWidth = 0.15 * HB; // approximate (0.116 of HB)
	fingerThick = fingerWidth;

	// Fingers shapes (boxes)
	Vector< Vector< Box* > > finger (fingCount, Vector< Box* >(jointCount));

	// Fingers metacarpal bone placement
	Vector<Vec> fingerPlacement (fingCount);

	// Finger fixed angles in hand structure
	Vector<double> fingerAngle(fingCount);

	// Index
	fingerPlacement[0] = Vec(0, -0.169 * HB, 0.165 * HL);
	finger[0][0] = new Box(fingerThick, fingerWidth, sqrt(pow2(0.374*HL) + pow2(0.126*HB)));
	finger[0][1] = new Box(fingerThick, fingerWidth, 0.265 * HL);
	finger[0][2] = new Box(fingerThick, fingerWidth, 0.143 * HL);
	finger[0][3] = new Box(fingerThick, fingerWidth, 0.097 * HL);
	fingerAngle[0] = 6;

	if(fingCount > 1)
	{
		// Middle
		fingerPlacement[1] = Vec(0, 0, 0.2 * HL);
		finger[1][0] = new Box(fingerThick, fingerWidth, 0.373 * HL);
		finger[1][1] = new Box(fingerThick, fingerWidth, 0.277 * HL);
		finger[1][2] = new Box(fingerThick, fingerWidth, 0.170 * HL);
		finger[1][3] = new Box(fingerThick, fingerWidth, 0.108 * HL);
		fingerAngle[1] = 0;

		if(fingCount > 2)
		{
			// Ring
			fingerPlacement[2] = Vec(0, 0.130 * HB, 0.2 * HL);
			finger[2][0] = new Box(fingerThick, fingerWidth, sqrt(pow2(0.336*HL) + pow2(0.077*HB)));
			finger[2][1] = new Box(fingerThick, fingerWidth, 0.259 * HL);
			finger[2][2] = new Box(fingerThick, fingerWidth, 0.165 * HL);
			finger[2][3] = new Box(fingerThick, fingerWidth, 0.107 * HL);
			fingerAngle[2] = -6;

			// Small
			fingerPlacement[3] = Vec(0, 0.231 * HB, 0.2 * HL);
			finger[3][0] = new Box(fingerThick, fingerWidth, sqrt(pow2(0.295*HL) + pow2(0.179*HB)));
			finger[3][1] = new Box(fingerThick, fingerWidth, 0.206 * HL);
			finger[3][2] = new Box(fingerThick, fingerWidth, 0.117 * HL);
			finger[3][3] = new Box(fingerThick, fingerWidth, 0.093 * HL);
			fingerAngle[3] = -16;
		}
	}

	// Range for joints
	AngleRange MCP(-10,90);								// Proximal (-10 needs checking)
	AngleRange PIP(0,100);								// Middle
	Vector<AngleRange> DIP (4, AngleRange(-10,90));		// Distal
	DIP[2].setMin(-20);
	DIP[3].setMin(-30);

	// Create palm
	double palmLength = HL * 0.6; // approx.
	double palmWidth = HB;

	//this->shape = new Box(fingerThick, fingerWidth,  fingerWidth);//jjcao
	this->shape = new Box(fingerThick,  palmWidth,  palmLength);
	this->parent = parent;

	if(name.size() < 1)
		this->name = QString(":hand");

	QString names[] = {"Index","Middle","Ring", "Little"};

	// Fingers setup
	for(int i = 0; i < fingCount; i++)
	{
		// Metacarpal bone
		addChild(finger[i][0], name.left(1) + QString("-Metacarpal-%1").arg(names[i]));
		RigidPart * part = child[i];
		part->tranlsateAnchor(fingerPlacement[i]); // placement for metacarpal bone
		part->rotateAnchor(fingerAngle[i], 0, 0);
		part->rot.disableX();
		part->rot.disableY();
		part->rot.disableZ();

		// Add phalangeal bones: Proximal
		part->addChild(finger[i][1], name.left(1) + QString("-Proximal-%1").arg(names[i]));
		part = part->child[0];
		part->tranlsateAnchor( Vec(0,0,part->parent->shape->z) );
		part->rot.disableX();
		part->rot.setRangeY( MCP );
		part->rot.disableZ(); // range z - missing here

		// Middle
		part->addChild(finger[i][2], name.left(1) + QString("-Middle-%1").arg(names[i]));
		part = part->child[0];
		part->tranlsateAnchor( Vec(0,0,part->parent->shape->z) );
		part->rot.disableX();
		part->rot.setRangeY( PIP );
		part->rot.disableZ();

		// Distal
		part->addChild(finger[i][3], name.left(1) + QString("-Distal-%1").arg(names[i]));
		part = part->child[0];
		part->tranlsateAnchor( Vec(0,0,part->parent->shape->z) );
		part->rot.disableX();
		part->rot.setRangeY( DIP[i] );
		part->rot.disableZ();
	}

	this->setupThumb();

	// Center of the palm, help with the grabbing
	//this->palmCenter = Vec();

	color = Color4(255,0,0, 125);
	setChildrenColor(Color4(255,255,0));
	
	updateMatrix();

	this->isReady = true;
}

void HumanHand::setupThumb()
{
	// Thumb bone sizes
	Vector< Box* > thumb (3);
	thumb[0] = new Box(fingerThick, fingerWidth, 0.251 * HL);
	thumb[1] = new Box(fingerThick, fingerWidth, 0.196 * HL);
	thumb[2] = new Box(fingerThick, fingerWidth, 0.158 * HL);

	// Metacarpal (CMC)	
	addChild(thumb[0], name.left(1) + QString("-Metacarpal-Thumb"));
	//RigidPart * part = child[fingCount]; //jjcao
	RigidPart * part = child.back();
	part->tranlsateAnchor(Vec(0, -0.1 * HB, 0.1 * HL));
	part->rot.setRangeX(AngleRange(-60,-30));	// Flex
	part->rot.setRangeY(AngleRange(0,60));	// Abduction
	part->rot.disableZ();

	// Middle (MCP)
	part->addChild(thumb[1], name.left(1) + QString("-Proximal-Thumb"));
	part = part->child[0];
	part->tranlsateAnchor( Vec(0,0,part->parent->shape->z) );
	//part->rot.setRangeX(AngleRange(-55,10));
	part->rot.setRangeX(AngleRange(-10,55));
	part->rot.disableY();
	part->rot.disableZ();

	// Distal (IP)
	part->addChild(thumb[2], name.left(1) + QString("-Distal-Thumb"));
	part = part->child[0];
	part->tranlsateAnchor( Vec(0,0,part->parent->shape->z) );
	//part->rot.setRangeX(AngleRange(-80, 15));
	part->rot.setRangeX(AngleRange(-15, 80));
	part->rot.disableY();
	part->rot.disableZ();
}

void HumanHand::openAll(double theta, bool isStep)
{
	RigidPart *firstBone, *jointA, *jointB, *jointC;
	RigidPart *thumbA, *thumbB, *thumbC;

	for(int i = 0; i < fingCount; i++){
		firstBone = child[i];
		jointA = firstBone->child[0];
		jointB = jointA->child[0];
		jointC = jointB->child[0];

		jointA->rot.addAngle('y', theta, isStep);
		jointB->rot.addAngle('y', theta, isStep);
		jointC->rot.addAngle('y', theta, isStep);
	}

	thumbA = child[fingCount];
	thumbB = thumbA->child[0];
	thumbC = thumbB->child[0];

	// CMC
	thumbA->rot.addAngle('y', theta, isStep);
	thumbA->rot.addAngle('x', theta, isStep);

	thumbB->rot.addAngle('x', theta, isStep); // MCP
	thumbC->rot.addAngle('x', theta, isStep); // PIP
}

void HumanHand::openUntilNonCollide( Mesh *mesh, double step)
{
	// Opening is negative angles
	step = -(abs(step));

	// Fingers
	for(int i = 0; i < fingCount; i++)
	{
		RigidPart * finger = getFinger(i);

		RigidPart * partA = finger->child[0];
		RigidPart * partB = partA->child[0];
		RigidPart * partC = partB->child[0];

		// FINGER: distal
		while(finger->intersectWithMesh(mesh))
		{
			if( !partC->rot.addAngle('y', step) )	break;
		}

		// FINGER: middle
		while(finger->intersectWithMesh(mesh))
		{
			if( !partB->rot.addAngle('y', step) )	break;
		}

		// FINGER: proximal
		while(finger->intersectWithMesh(mesh))
		{
			if( !partA->rot.addAngle('y', step) )	break;
		}
	}

	// Thumb - just open it fully
	RigidPart * thumbA = child[fingCount];
	RigidPart * thumbB = thumbA->child[0];
	RigidPart * thumbC = thumbB->child[0];

	// IP
	while( thumbC->rot.addAngle('x', step) );

	// MCP
	while( thumbB->rot.addAngle('x', step) );

	// Thumb - CMC
	while( thumbA->rot.addAngle('x', step) );
	while( thumbA->rot.addAngle('y', step) );
}

void HumanHand::closeUntilCollide( Mesh *mesh, double step)
{
	step = abs(step);

	bool undo = false;

	// Fingers
	for(int i = 0; i < fingCount; i++)
	{
		RigidPart * finger = getFinger(i);

		RigidPart * partA = finger->child[0];
		RigidPart * partB = partA->child[0];
		RigidPart * partC = partB->child[0];

		// FINGER: proximal
		undo = false;
		while(!finger->intersectWithMesh(mesh))
		{
			if(partA->rot.addAngle('y', step))
				undo = true;
			else
				break;
		}
		if(undo) partA->rot.undo();

		// FINGER: middle
		undo = false;
		while(!finger->intersectWithMesh(mesh))
		{
			if(partB->rot.addAngle('y', step))
				undo = true;
			else
				break;
		}
		if(undo) partB->rot.undo();

		// FINGER: distal
		undo = false;
		while(!finger->intersectWithMesh(mesh))
		{
			if(partC->rot.addAngle('y', step))
				undo = true;
			else
				break;
		}
		if(undo) partC->rot.undo();
	}

	// Thumb
	RigidPart * thumbA = child[fingCount];
	RigidPart * thumbB = thumbA->child[0];
	RigidPart * thumbC = thumbB->child[0];

	bool undoAy = false, undoAz = false;
	bool undoB = false, undoC = false;

	// Thumb - CMC
	while( !thumbA->intersectWithMesh(mesh) )
	{	
		if(thumbA->rot.addAngle('y', step))
			undoAy = true;
		else 
			break;

		if (thumbA->intersectWithMesh(mesh))
			break;

		if(thumbA->rot.addAngle('x', step))
			undoAz = true;
		else 
			break;
	}
	if(undoAy || undoAz)
	{
		thumbA->rot.undo();
	}

	// MCP
	while(!thumbB->intersectWithMesh(mesh))
	{
		if(thumbB->rot.addAngle('x', step)) undoB = true;
		else break;
	}
	if(undoB) thumbB->rot.undo();

	// IP
	while(!thumbC->intersectWithMesh(mesh))
	{
		if(thumbC->rot.addAngle('x', step)) undoC = true;
		else break;
	}
	if(undoC) thumbC->rot.undo();
}

RigidPart * HumanHand::getFinger(int i) const
{
	return child[i];
}

RigidPart * HumanHand::getThumb() const
{
	return child[fingCount];
}

double HumanHand::getFingerThick()
{
	return fingerThick;
}

int HumanHand::getFingerCount() const
{
	return fingCount;
}

bool HumanHand::ready()
{
	return isReady;
}

Vector<double> HumanHand::toVector(FeatureSpace featureSpace)
{
	assert(false); // need to be checked

	Vector<double> result;

	switch( featureSpace )
	{
	case ANGLE:
		{
			RigidPart * thumbA = getThumb();
			RigidPart * thumbB = thumbA->child[0];
			RigidPart * thumbC = thumbB->child[0];

			// Output thumb data
			result.push_back( thumbA->rot.y() );	
			result.push_back( thumbA->rot.x() );
			result.push_back( thumbB->rot.x() );
			result.push_back( thumbC->rot.x() );

			// Other four fingers
			for(int i = 0; i < getFingerCount(); i++)
			{
				RigidPart * firstBone = getFinger(i);

				RigidPart * jointA = firstBone->child[0];
				RigidPart * jointB = jointA->child[0];
				RigidPart * jointC = jointB->child[0];

				result.push_back( jointA->rot.y() );
				result.push_back( jointB->rot.y() );
				result.push_back( jointC->rot.y() );
			}
		}
		break;

	case DIST_ANGLE:
		{
			RigidPart * thumbA = getThumb();
			RigidPart * thumbB = thumbA->child[0];
			RigidPart * thumbC = thumbB->child[0];

			// Output thumb data
			result.push_back( thumbA->rot.y() );  
			result.push_back( thumbA->rot.z() );
			result.push_back( thumbB->rot.z() );
			result.push_back( thumbC->rot.z() );

			// Angles of other four fingers
			for(int i = 0; i < getFingerCount(); i++)
			{
				RigidPart * firstBone = getFinger(i);

				RigidPart * jointA = firstBone->child[0];
				RigidPart * jointB = jointA->child[0];
				RigidPart * jointC = jointB->child[0];

				result.push_back( jointA->rot.y() );
				result.push_back( jointB->rot.y() );
				result.push_back( jointC->rot.y() );
			}

			// Intra-finger distances of other four fingers
			for(int i = 1; i < getFingerCount(); i++)
			{
				RigidPart * firstBone1 = getFinger(i);
				RigidPart * firstBone2 = getFinger(i-1);

				RigidPart * jointA1 = firstBone1->child[0];
				RigidPart * jointB1 = jointA1->child[0];
				RigidPart * jointC1 = jointB1->child[0];

				RigidPart * jointA2 = firstBone2->child[0];
				RigidPart * jointB2 = jointA2->child[0];
				RigidPart * jointC2 = jointB2->child[0];

				result.push_back((jointA1->getCenter() - jointA2->getCenter()).norm() );
				result.push_back((jointB1->getCenter() - jointB2->getCenter()).norm() );
				result.push_back((jointC1->getCenter() - jointC2->getCenter()).norm() );
			}
		}
		break;

	case DISTANCE_JOINTS_ALL:
		{
			Vec rc = getCenter();

			RigidPart * thumbA = getThumb();
			RigidPart * thumbB = thumbA->child[0];
			RigidPart * thumbC = thumbB->child[0];

			// Thumb
			result.push_back((thumbA->getCenter() - rc).norm());
			result.push_back((thumbB->getCenter() - rc).norm());
			result.push_back((thumbC->getCenter() - rc).norm());

			for(int i = 0; i < getFingerCount(); i++)
			{
				for(int j = i; j < getFingerCount(); j++)
				{
					RigidPart * firstBone1 = getFinger(i);
					RigidPart * firstBone2 = getFinger(j);

					RigidPart *joint1[3], *joint2[3];

					joint1[0] = firstBone1->child[0];
					joint1[1] = joint1[0]->child[0];
					joint1[2] = joint1[1]->child[0];

					joint2[0] = firstBone2->child[0];
					joint2[1] = joint2[0]->child[0];
					joint2[2] = joint2[1]->child[0];

					if(i == j){
						for(int ii=0; ii<3; ii++)
							for(int jj=ii+1; jj<3; jj++)
								result.push_back(( joint1[ii]->getCenter() - joint2[jj]->getCenter() ).norm());
					} else {
						for(int ii=0; ii<3; ii++)
							for(int jj=0; jj<3; jj++)
								result.push_back(( joint1[ii]->getCenter() - joint2[jj]->getCenter() ).norm());
					}
				}
			}
		}
		break;
	}

	return result;
}

StdString HumanHand::toString(FeatureSpace featureSpace)
{
	stringstream out (stringstream::in | stringstream::out);

	Vector<double> v = toVector(featureSpace);
	
	for(int i = 0; i < (int) v.size(); i++)
		out << v[i] << " ";

	return out.str();
}

Vector<double> HumanHand::bestGrab(FeatureSpace featureSpace)
{
	// Make a hand
	HumanHand h; h.init();

	h.openAll(999); // close

	return h.toVector( featureSpace );
}

double HumanHand::difference(FeatureSpace featureSpace, const Vector<double> & toGrab)
{
	double res = 0;

	Vector<double> current = toVector( featureSpace );

	for (int i = 0; i < (int) toGrab.size(); i++) 
	{
		res += pow((toGrab[i] - current[i]), 2);
	}

	return sqrt(res);
}

double HumanHand::getHandLength()
{
	return HL;
}

double HumanHand::getHandBreadth()
{
	return HB;
}
