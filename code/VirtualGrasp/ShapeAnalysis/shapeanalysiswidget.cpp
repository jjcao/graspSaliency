#include "isa_headers.h"
#include "shapeanalysiswidget.h"
#include "IntersectionTest.h"
#include "util.h"
#include "Octree.h"

#include "Experiment.h"
#include "Stats.h"

#include "RigidPartWidget.h"

ShapeAnalysisWidget::ShapeAnalysisWidget(QWidget *parent) : QWidget(parent)
{
	this->setWindowTitle( "Shape Analysis" );
	this->setAutoFillBackground( true );
	this->setMinimumWidth(300);

	layout = new QVBoxLayout;

	// Frames
	interactionsFrame = new QFrame(this);
	scaleFrame = new QFrame(this);
	experimentFrame = new QFrame(this);

	// Layouts
	interactionsLayout = new QGridLayout;
	scaleLayout = new QGridLayout;
	experimentLayout = new QGridLayout;

	// Buttons
	showHand = new QPushButton("Show Hand!");
	closeHand = new QPushButton("Close Hand!");
	openHand = new QPushButton("Open Hand!");
	grabObj = new QPushButton("Grab");
	randomPos = new QPushButton("Random Pos");
	startSamp = new QPushButton("Start");
	setMeshScale = new QPushButton("Set");
	exportSelected = new QPushButton("Export selected");

	// Spin box
	numSamples = new QSpinBox;
	meshScale = new QDoubleSpinBox;
	thresholdDist = new QDoubleSpinBox;
	angleStep = new QDoubleSpinBox;

	// Check boxes
	outputSamplesFile = new QCheckBox("Output experiment data");
	saveSnapshots = new QCheckBox("Save snapshots");

	// List box
	obs = new QListWidget();
	//obs->setWrapping(true);

	// Radio box
	featureSpaceGroupBox = new QGroupBox(tr("Feature space"));
	featureAngle = new QRadioButton(tr("Angle"));
	featureDist = new QRadioButton(tr("Distance"));
	featureAngle->setChecked(true);

	QHBoxLayout *hbox = new QHBoxLayout;
	hbox->addWidget(featureAngle);
	hbox->addWidget(featureDist);
	hbox->addStretch();
	featureSpaceGroupBox->setLayout(hbox);

	// Interactions Frame
	int row = 0;
	interactionsLayout->addWidget(new QLabel("Interactions:"), row++, 0, 1, 3);

	interactionsLayout->addWidget(showHand, row++, 0,1,3);
	interactionsLayout->addWidget(openHand, row, 0);
	interactionsLayout->addWidget(closeHand, row, 1);
	interactionsLayout->addWidget(grabObj, row, 2);

	interactionsFrame->setFrameStyle(QFrame::StyledPanel);
	interactionsFrame->setLayout(interactionsLayout);
	layout->addWidget(interactionsFrame);


	// Scale Frame
	row = 0;
	scaleLayout->addWidget(new QLabel("Scale:"), row++, 0, 1, 3);

	scaleLayout->addWidget(new QLabel("Mesh scale:"), row, 0);
	scaleLayout->addWidget(meshScale, row, 1);
	scaleLayout->addWidget(setMeshScale, row++, 2);

	scaleFrame->setFrameStyle(QFrame::StyledPanel);
	scaleFrame->setLayout(scaleLayout);
	layout->addWidget(scaleFrame);


	// Experiment Frame
	experimentLayout->addWidget(new QLabel("Experiments:"), row++, 0, 1, 3);
	experimentLayout->addWidget(saveSnapshots, row++, 0, 1, 3);
	experimentLayout->addWidget(outputSamplesFile, row++, 0, 1, 3);

	experimentLayout->addWidget(new QLabel("Threshold distance (cm):"), row, 0, 1, 2);
	experimentLayout->addWidget(thresholdDist, row++, 2, 1, 1);

	experimentLayout->addWidget(new QLabel("Angle step (degrees):"), row, 0, 1, 2);
	experimentLayout->addWidget(angleStep, row++, 2, 1, 1);

	experimentLayout->addWidget(featureSpaceGroupBox, row++, 0, 1, 3);

	experimentLayout->addWidget(numSamples, row, 0);
	experimentLayout->addWidget(startSamp, row, 1);
	experimentLayout->addWidget(randomPos, row++, 2);

	experimentLayout->addWidget(obs, row, 0, 5, 3); row += 5;
	experimentLayout->addWidget(exportSelected, row++, 0, 1, 3);

	experimentFrame->setFrameStyle(QFrame::StyledPanel);
	experimentFrame->setLayout(experimentLayout);
	layout->addWidget(experimentFrame);

	// Rigid parts exploration
	rpw = new RigidPartWidget(NULL);
	layout->addWidget(rpw);

	layout->addStretch(); // empty space to end

	this->setLayout(layout);

	connect(showHand, SIGNAL(clicked()), SLOT(showHandClicked()));
	connect(closeHand, SIGNAL(clicked()), SLOT(closeHandClicked()));
	connect(openHand, SIGNAL(clicked()), SLOT(openHandClicked()));
	connect(grabObj, SIGNAL(clicked()), SLOT(closeTillCollide()));
	connect(setMeshScale, SIGNAL(clicked()), SLOT(meshScaleChanged()));
	connect(randomPos, SIGNAL(clicked()), SLOT(randomPosClicked()));
	connect(exportSelected, SIGNAL(clicked()), SLOT(exportObservations()));
	connect(startSamp, SIGNAL(clicked()), SLOT(showHandClicked()));
	connect(startSamp, SIGNAL(clicked()), SLOT(sampleMeshButtonClicked()));
	connect(angleStep, SIGNAL(valueChanged(double)), SLOT(angleStepChanged(double)));

	// OpenGL update
	mainWindow->ui.viewer->connect(startSamp, SIGNAL(clicked()), SLOT(update()));

	// get pointer to loaded mesh
	mesh = getMesh("LoadedMesh");

	// Default values
	numSamples->setRange(1, INT_MAX);
	numSamples->setValue(100);

	meshScale->setRange(0.000000001, 10000);
	meshScale->setValue(1.0);
	meshScale->setSingleStep(5);

	thresholdDist->setRange(0.0, 1000);
	thresholdDist->setValue(4.0); // default

	angleStep->setRange(0.001, 180);
	angleStep->setValue(defaultAngleStep);

	isExperimentRunning = false;
}

void ShapeAnalysisWidget::closeHandClicked()
{
	if(hand != NULL)
	{
		hand->openAll(10, true);
		colorMesh(hand->intersectWithMesh( mesh ));
	}
}

void ShapeAnalysisWidget::openHandClicked()
{
	if(hand != NULL)
	{
		hand->openAll(-10, true);
		colorMesh(hand->intersectWithMesh( mesh ));
	}
}

void ShapeAnalysisWidget::showHandClicked()
{
	// Create hand
	hand = new HumanHand();
	hand->init();
	hand->openAll();

	// Movable hand
	agentManipulator = new ManipulatedFrame();
	mainWindow->ui.viewer->setManipulatedFrame(agentManipulator);
}

void ShapeAnalysisWidget::colorMesh(bool intersect)
{
	if(intersect)
		mesh->setColor(255,0,0);
	else
		mesh->setColor(0,255,0);

	mainWindow->ui.viewer->update();
}

void ShapeAnalysisWidget::closeTillCollide()
{
	if(hand != NULL)
	{
		hand->closeUntilCollide(mesh, 1);

		colorMesh(hand->intersectWithMesh( mesh ));
	}
}

void ShapeAnalysisWidget::randomPosClicked()
{
	sampleMesh(1);
}

void ShapeAnalysisWidget::sampleMeshButtonClicked()
{
	obs->clear();

	sampleMesh(numSamples->value());
}

void ShapeAnalysisWidget::sampleMesh(int n)
{
	mainWindow->ui.viewer->print("Sampling..", 3000);

	if(hand && !hand->ready()) return;

	hand->isReady = false;
	hand = NULL;

	QString fileName;

	if(outputSamplesFile->isChecked())
		fileName = QFileDialog::getSaveFileName(this, tr("Save Experiment"), "", 
			tr("Experiment File (*.exp)"));
	
	// Viewer connections
	mainWindow->ui.viewer->connect(e, SIGNAL(saveSnapshot()), SLOT(updateAndSnapshot()), Qt::BlockingQueuedConnection);
	mainWindow->ui.viewer->connect(this, SIGNAL(updateViewer()), SLOT(updateGL()));

	// Feature space decision
	FeatureSpace space = ANGLE;
	if(featureAngle->isChecked()) space = ANGLE;
	if(featureDist->isChecked()) space = DISTANCE_JOINTS_ALL;

	// Perform experiment
	e->init(mesh, n, thresholdDist->value(), saveSnapshots->isChecked(), fileName, space);

	if(s != NULL) e->sampler = s;

	e->start();

	this->connect(e, SIGNAL(addObservation()), SLOT(addObservation()), Qt::BlockingQueuedConnection);
	this->connect(obs, SIGNAL(itemSelectionChanged()), SLOT(readObservation()));
	this->connect(e, SIGNAL(finished()), SLOT(selectFirstObs()));
}

void ShapeAnalysisWidget::meshScaleChanged()
{
	double newScale = meshScale->value();

	mesh->normalize_scale = newScale;
	mesh->normalizeScale();
	mesh->rebuildOctree();

	// Update view
	mainWindow->ui.viewer->update();
	mainWindow->ui.viewer->updateGL();
}

void ShapeAnalysisWidget::addObservation()
{
	//QString text = QString("%1 - %2").arg(weight).arg(obs->count());
	QString text = QString::number(obs->count());

	QListWidgetItem* item = new QListWidgetItem(text);
	item->setCheckState(Qt::Unchecked);

	obs->addItem(item);
}

void ShapeAnalysisWidget::readObservation()
{
	int i = RANGED(0, (int)obs->row(obs->currentItem()), (int)e->obs.size() - 1);

	hand = e->obs[i]->hand;

	agentManipulator->setFromMatrix( (const double*) hand->mat.data());

	emit updateViewer();
}

void ShapeAnalysisWidget::exportObservations()
{
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save Experiment"), "", 
		tr("Index File (*.txt)"));
	
	ofstream file;

	file.open(fileName.toStdString().c_str());

	// Output the checked indices
	for(int row = 0; row < obs->count(); row++)
	{
		if(obs->item(row)->checkState() == Qt::Checked)
			file << row << endl;
	}

	file.close();
}

void ShapeAnalysisWidget::selectFirstObs()
{
	obs->setCurrentItem( obs->itemAt(0,0) );

	// Display timing on viewer
	mainWindow->ui.viewer->print(QString("Experiment time = %1 ms").arg(e->totalTime.time), 1000);
}

void ShapeAnalysisWidget::angleStepChanged(double newStep)
{
	defaultAngleStep = newStep;
}
