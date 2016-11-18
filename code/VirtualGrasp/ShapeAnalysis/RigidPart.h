#pragma once

#include <QMatrix4x4>
#include <QString>
#include "Mesh.h"
#include "Box.h"
#include "rotationConstraint.h"

// Handy macros
#define Rad(x)  ((x)*M_PI/180.0)
#define Deg(x)  ((x)*180.0/M_PI)

class RigidPart
{
private:

public:
	RigidPart * parent;
	vector<RigidPart *> child;
	Shape * shape;
	bool isShowShape;
	QString name;
	bool isReady;

    QMatrix4x4 mat; // global transformation matrix for this part

	Vec anchorPos;
	Vec anchorRotations;

	// Transformations
	double scale;
	RotationConstraint rot;

	bool isReflect;
	char reflectAxis;

	Color4 color;
	void setChildrenColor(const Color4 & c);

    RigidPart * getChild(int childId);
	RigidPart * getLastChild();
    RigidPart * addChild(Shape * shape, QString childName = "");
	RigidPart * addChild(RigidPart * part); // direct
	void removeAllChildren();

    bool isIntersects(Mesh *mesh);
    bool intersectWithMesh(Mesh *mesh);

	void tranlsateAnchor(const Vec & delta);
	void rotateAnchor(double x, double y, double z);
	double getScale();
	void setScale(double scale);
	void setMat( const QMatrix4x4 & fromMat );
	void updateMatrix();

	void draw();
	void drawBranches();

	Vec origin();
	Vec localPoint(const Vec & delta);

	Vec maxExtent();
	Vec maxExtentLocal();
	Vector<Vec> getCorners();
	Vec getCenter();

	Mesh * toMesh();
	bool hasReflectedParent();

    RigidPart( QString newName = "");
	RigidPart( RigidPart * parent, Shape * = NULL, QString newName = "" );
	RigidPart( const RigidPart & from );

	~RigidPart();
};
