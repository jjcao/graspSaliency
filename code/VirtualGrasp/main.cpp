#include "stdafx.h"
#include "virtualgrasp.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	VirtualGrasp w;
	w.show();

	return a.exec();
}
