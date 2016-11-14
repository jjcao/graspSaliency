#pragma once

#include "Shape.h"

class Box : public Shape
{
public:
	Box(double xx = 1.0) { x = y = z = xx; }
	Box(double xx, double yy, double zz) {x=xx; y=yy; z=zz;}
	virtual Shape* Clone() { return new Box(this->x, this->y, this->z); }

	Vec bottomCenter(){ return Vec(x * 0.5, y * 0.5, 0); }
	Vec sideCenter(){ return Vec(x * 0.5, y * 0.5, 0); }

	virtual void draw(const Color4 & color);
	virtual bool isIntersects(const QMatrix4x4 & mat,Mesh * mesh);

	virtual Vector<Vec> getFacesAsVertices();
};
