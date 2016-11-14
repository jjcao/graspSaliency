#include "stdafx.h"
#include "virtualgrasp.h"
#include <QtWidgets/QApplication>

HumanHand * hand;

double Epsilon = 1.0e-6f;

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	VirtualGrasp w;
	w.show();

	return a.exec();
}
