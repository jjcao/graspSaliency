#ifndef VIRTUALGRASP_H
#define VIRTUALGRASP_H

//#include <QtWidgets>
#include <QtWidgets/QMainWindow>
#include "ui_virtualgrasp.h"

class VirtualGrasp : public QMainWindow
{
	Q_OBJECT

public:
	VirtualGrasp(QWidget *parent = 0);
	~VirtualGrasp();

//private:
	Ui::VirtualGraspClass ui;
};

#endif // VIRTUALGRASP_H
