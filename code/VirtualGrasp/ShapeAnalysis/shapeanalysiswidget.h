#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QFrame>
#include <QLabel>
#include <QListWidget>
#include <QListWidgetItem>
#include <QGroupBox>
#include <QRadioButton>

#include "Agent/HumanHand.h"
#include "Experiment.h"
#include "RigidPartWidget.h"

class ShapeAnalysisWidget : public QWidget
{
	Q_OBJECT

public:
	ShapeAnalysisWidget(QWidget *parent = 0);

	QVBoxLayout * layout;

	// Interactions
	QFrame * interactionsFrame;
	QGridLayout * interactionsLayout;
	QPushButton * showHand;	
	QPushButton * showSkin;
	QPushButton * closeHand;
	QPushButton * openHand;	
	QPushButton * grabObj;
	QPushButton * ungrabObj;	

	// Scale control
	QFrame * scaleFrame;
	QGridLayout * scaleLayout;
	QDoubleSpinBox * meshScale;
	QPushButton * setMeshScale;

	// Experiments
	QFrame * experimentFrame;
	QGridLayout * experimentLayout;
	QSpinBox * numSamples;
	QDoubleSpinBox * thresholdDist;
	QDoubleSpinBox * angleStep;
	QCheckBox * outputSamplesFile;
	QCheckBox * saveSnapshots;
	QPushButton * startSamp;
	QListWidget * obs;
	QPushButton * exportSelected;

	QPushButton * randomPos;

	bool isExperimentRunning;

	QGroupBox * featureSpaceGroupBox;
	QRadioButton * featureAngle;
	QRadioButton * featureDist;

	// Rigid part properties widget
	RigidPartWidget * rpw;

public slots:
	void showHandClicked();
	void showHandSkinClicked();
	void colorMesh(bool intersect);
	void openHandClicked();
	void closeHandClicked();
	void randomPosClicked();
	void closeTillCollide();
	void openAll();

	void meshScaleChanged();

	void sampleMeshButtonClicked();
	//void sampleMesh(int n = 100);

	//void addObservation();
	//void readObservation();
	//void selectFirstObs();
	//void exportObservations();
	void angleStepChanged(double newStep);

signals:
	void updateViewer();
};
