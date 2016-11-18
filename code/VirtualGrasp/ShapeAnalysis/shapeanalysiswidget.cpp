#include "stdafx.h"
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

	// Interactions Frame
	showHand = new QPushButton("Show");
	showSkin = new QPushButton("Skin");
	closeHand = new QPushButton("Close");
	openHand = new QPushButton("Open");
	grabObj = new QPushButton("Grab");
	ungrabObj = new QPushButton("UnGrab");

	int row = 0;
	interactionsLayout->addWidget(new QLabel("Hand:"), row++, 0, 1, 4);

	interactionsLayout->addWidget(showHand, row, 0, 1, 2);
	interactionsLayout->addWidget(showSkin, row++, 2, 1, 2);
	interactionsLayout->addWidget(openHand, row, 0);
	interactionsLayout->addWidget(closeHand, row, 1);
	interactionsLayout->addWidget(grabObj, row, 2);
	interactionsLayout->addWidget(ungrabObj, row, 3);

	interactionsFrame->setFrameStyle(QFrame::StyledPanel);
	interactionsFrame->setLayout(interactionsLayout);
	layout->addWidget(interactionsFrame);


	// Scale Frame
	setMeshScale = new QPushButton("Set");
	meshScale = new QDoubleSpinBox;
	row = 0;
	scaleLayout->addWidget(new QLabel("Scale:"), row++, 0, 1, 3);

	scaleLayout->addWidget(new QLabel("Mesh scale:"), row, 0);
	scaleLayout->addWidget(meshScale, row, 1);
	scaleLayout->addWidget(setMeshScale, row++, 2);

	scaleFrame->setFrameStyle(QFrame::StyledPanel);
	scaleFrame->setLayout(scaleLayout);
	layout->addWidget(scaleFrame);


	// Experiment Frame
	randomPos = new QPushButton("Random Pos");
	startSamp = new QPushButton("Start");
	exportSelected = new QPushButton("Export selected");

	// Spin box
	numSamples = new QSpinBox;	
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

	layout->addStretch(); // empty space to end

	this->setLayout(layout);

	connect(showHand, SIGNAL(clicked()), SLOT(showHandClicked()));
	connect(showSkin, SIGNAL(clicked()), SLOT(showHandSkinClicked()));	
	connect(closeHand, SIGNAL(clicked()), SLOT(closeHandClicked()));
	connect(openHand, SIGNAL(clicked()), SLOT(openHandClicked()));
	connect(grabObj, SIGNAL(clicked()), SLOT(closeTillCollide()));
	connect(ungrabObj, SIGNAL(clicked()), SLOT(openAll()));
	connect(setMeshScale, SIGNAL(clicked()), SLOT(meshScaleChanged()));
	connect(randomPos, SIGNAL(clicked()), SLOT(randomPosClicked()));
	connect(exportSelected, SIGNAL(clicked()), SLOT(exportObservations()));
	connect(startSamp, SIGNAL(clicked()), SLOT(showHandClicked()));
	connect(startSamp, SIGNAL(clicked()), SLOT(sampleMeshButtonClicked()));
	connect(angleStep, SIGNAL(valueChanged(double)), SLOT(angleStepChanged(double)));

	// OpenGL update
	mainWindow->ui.viewer->connect(startSamp, SIGNAL(clicked()), SLOT(update()));


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


void ShapeAnalysisWidget::angleStepChanged(double newStep)
{
	defaultAngleStep = newStep;
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
	hand->isReady = !hand->isReady;
	mainWindow->ui.viewer->update();
}
void ShapeAnalysisWidget::showHandSkinClicked()
{
	hand->isShowShape = !hand->isShowShape;
	mainWindow->ui.viewer->update();
}
void ShapeAnalysisWidget::colorMesh(bool intersect)
{
	if(intersect)
		mesh->setColor(255,0,0);
	else
		mesh->setColor(0,255,0);

	mainWindow->ui.viewer->update();
}
void ShapeAnalysisWidget::openAll()
{
	if (hand) hand->openAll();

	if (mesh) mesh->setColor(0, 255, 0);
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
}

void ShapeAnalysisWidget::sampleMeshButtonClicked()
{
}

