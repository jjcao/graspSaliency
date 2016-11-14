#include "Cylinder.h"

Cylinder::Cylinder( int sides, double radius, double height )
{
	this->sides = sides;

	this->x = radius * 2;
	this->y = this->x;
	this->z = height;

	Vec axis (0,0,1);
	Vec ref (0.5,0.5,0);

	double theta = (2.0 * M_PI) / (double) sides;
	double startAngle = 0;

	Vec c1 (0, 0, 0);
	Vec c2 (0, 0, height);

	Vector<Vec> points, points2;

	int vindex = 0, findex = 0;

	mesh = new Mesh(sides * 2 + 2);

	mesh->addVertex(c1, vindex++);

	for(int i = 0; i < sides; i++){
		points.push_back(radius * qglviewer::Quaternion(axis, startAngle + theta).rotate(ref).unit());
		points2.push_back(points.back() + c2);
		startAngle += theta;
		mesh->addVertex(points.back(), vindex++);
	}

	for(int i = 0; i < sides; i++)
		mesh->addVertex(points[i] + c2, vindex++);

	// Bottom cap
	for(int i = 1; i <= sides; i++)	
		mesh->addFace(i, 0, (i % sides) + 1, findex++);

	// Sides
	for(int i = 1; i <= sides; i++)
	{
		int v0 = i;
		int v1 = (i % sides) + 1;
		int v2 = v1 + sides;
		int v3 = v0 + sides;

		// new faces indices
		int f1 = findex;
		int f2 = findex + 1;

		// Add the two faces
		mesh->addFace(v0, v1, v2, f1);
		mesh->addFace(v0, v2, v3, f2);

		findex += 2;
	}

	mesh->addVertex(c2, vindex++);

	// Top cap
	for(int i = sides + 1; i < sides * 2; i++)	
		mesh->addFace(vindex - 1, i, (i % (sides * 2)) + 1, findex++);

	mesh->addFace(vindex - 1, sides * 2, sides + 1, findex++);

	mesh->isVisible = true;
	mesh->isReady = true;
	mesh->isFlatShade = true;

	mesh->computeNormals();
	mesh->clearColors();
}

void Cylinder::setRadius( double r1, double r2 )
{
	Vec bottom = mesh->vec(0);
	Vec top = mesh->vec(mesh->numberOfVertices() - 1);

	for(int i = 1; i <= sides; i++)
		mesh->v(i)->set((mesh->vec(i) - bottom).unit() * r1);
	
	for(int i = sides * 2; i > sides; i--)
		mesh->v(i)->set((mesh->vec(i) - top).unit() * r2 + (top - bottom));

	mesh->computeNormals();
}

void Cylinder::draw( const Color4 & color )
{
	mesh->setColor(color);
	mesh->draw();
}
