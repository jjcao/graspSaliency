#pragma once

#include "Shape.h"

class Cylinder : public Shape
{
private:
	Mesh * mesh;
	int sides;

public:
	Cylinder() { mesh = NULL; sides = 0; }
	Cylinder(int sides, double radius, double height);

	void setRadius(double r1, double r2);
	void draw(const Color4 & color);
};
