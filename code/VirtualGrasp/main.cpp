#include "stdafx.h"
#include "virtualgrasp.h"
#include <QtWidgets/QApplication>

HumanHand * hand;
#include "ShapeAnalysis/Agent/HumanBody.h"
HumanBody * actor;

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

	//actor = new HumanBody;

	VirtualGrasp w;
	w.show();

	return a.exec();
}
