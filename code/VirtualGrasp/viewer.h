#pragma once
#include <QGLViewer/qglviewer.h>

class Viewer : public QGLViewer
{
public:
	Viewer(QWidget *parent = 0);
protected:
	virtual void draw();
	virtual void init();
	virtual QString helpString() const;
};

