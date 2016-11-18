#include "stdafx.h"
#include "virtualgrasp.h"
#include <QtWidgets/QApplication>
#include "Viewer.h"
#include "Commander.h"

HumanHand * hand = NULL;
ManipulatedFrame * agentManipulator = NULL;
//HumanBody * actor = NULL;
Mesh * mesh = NULL;
VirtualGrasp * mainWindow = NULL;
ShapeAnalysisWidget * saWidget = NULL;

double Epsilon = 1.0e-6f;

int main(int argc, char *argv[])
{
	srand(time(0));

	QApplication a(argc, argv);

	// Anti-aliasing
	QGLFormat glf = QGLFormat::defaultFormat();
	glf.setSampleBuffers(true);
	glf.setSamples(8);
	QGLFormat::setDefaultFormat(glf);

	// Shape analysis:
	hand = new HumanHand;
	hand->init();
	// Movable hand
	agentManipulator = new ManipulatedFrame();
	//actor = new HumanBody;

	mesh = NULL;

	VirtualGrasp w;
	w.show();

	// Pointers to expose to the entire world!
	mainWindow = &w;
	mainWindow->ui.commander->viewer = mainWindow->ui.viewer;
	mainWindow->ui.viewer->setManipulatedFrame(agentManipulator);	

	return a.exec();
}
