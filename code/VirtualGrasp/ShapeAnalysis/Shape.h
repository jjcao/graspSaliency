#pragma once

#include <QMatrix4x4>
#include "Mesh.h"
#include "SimpleDraw.h"

#include <QGLViewer/vec.h>
using qglviewer::Vec;
#include <vector>
using std::vector;

class Shape
{
public:
	double x,y,z;

	Shape(){ x = 0; y = 0; z = 0; }
	Shape(const Shape & other){ x = other.x; y = other.y; z = other.z; }
	virtual Shape* Clone() = 0;

	virtual void setSize(double xx, double yy, double zz){x=xx; y=yy; z=zz;}

	virtual void draw(const Color4 & color) = 0;
	virtual bool isIntersects(const QMatrix4x4 & mat, Mesh * mesh) = 0;
	
	virtual Vector<Vec> getFacesAsVertices() = 0;
};
