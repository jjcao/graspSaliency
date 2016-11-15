//#include "isa_headers.h"
#include "RigidPart.h"
#include "kdtree.h"
#include "SimpleDraw.h"

#include "Sleeper.h"

RigidPart::RigidPart(QString newName)
{
	this->parent = NULL;
	this->shape = NULL;
	this->name = newName;
	this->child.clear();
	this->isReady = false;
	
	this->isReflect = false;
	this->reflectAxis = 'x';

	scale = 1.0;
}

RigidPart::RigidPart(RigidPart * parent, Shape * newShape, QString newName)
{
	this->parent = parent;
	this->shape = newShape->Clone();
	this->name = newName;
	this->isReady = false;
	this->isReflect = false;
	this->reflectAxis = 'x';

	if(parent)
		scale = parent->getScale();
}

RigidPart::RigidPart(const RigidPart & other)
{
	if(other.shape)
		this->shape = other.shape->Clone();

	this->name = other.name;
	this->scale = other.scale;
	this->mat = other.mat;
	this->rot = other.rot;
	this->color = other.color;
	this->anchorPos = other.anchorPos;
	this->anchorRotations = other.anchorRotations;
	this->isReflect = other.isReflect;
	this->reflectAxis = other.reflectAxis;
	this->isReady = other.isReady;

	if(other.parent == NULL) 
		this->parent = NULL;

	for(int i = 0; i < (int)other.child.size(); i++)
	{
		RigidPart * otherChild = other.child[i];

		this->child.push_back(new RigidPart(*otherChild));
		this->child.back()->parent = this;
	}
}

RigidPart * RigidPart::addChild(Shape * newShape, QString childName)
{
	child.push_back(new RigidPart(this, newShape, childName));
	return child.back();
}

RigidPart * RigidPart::addChild(RigidPart * part)
{
	child.push_back(part);
	return child.back();
}

RigidPart::~RigidPart()
{
	for(int i = 0; i < (int)child.size(); i++)
	{
		delete child[i];	
	}

	delete shape;
}

RigidPart * RigidPart::getChild(int childId)
{
	if (childId >= (int)child.size() || childId < 0)
		return NULL;
	return child[childId];
}

RigidPart * RigidPart::getLastChild()
{
	return child.back();
}

void RigidPart::removeAllChildren()
{
	for(int i = 0; i < (int)child.size(); i++)
	{
		child[i]->removeAllChildren();

		if(child[i]!=NULL)
			delete child[i];
	}
	child.clear();
}

void RigidPart::setChildrenColor( const Color4 & c )
{
	for(int i = 0; i < (int)child.size(); i++)
	{
		child[i]->color = c;
		child[i]->setChildrenColor(c);
	}
}

double RigidPart::getScale()
{
	return scale;
}

void RigidPart::setScale(double scale)
{
	this->scale = scale;
}

void RigidPart::draw()
{
	updateMatrix();

	glPushMatrix();
	glMultMatrixf(mat.constData());

	// DEBUG: draw axis
	glColor4f(0,0,1.0f, 1);
	//mainWindow->ui.viewer->drawAxis(2);

	// Draw shape
	shape->draw(color);

	glPopMatrix();

	// Draw children
	for(int i = 0; i < (int)child.size(); i++)
		child[i]->draw();
}

void RigidPart::drawBranches()
{
	// Draw children
	for(int i = 0; i < (int)child.size(); i++)
	{
		SimpleDraw::IdentifyLine(origin(), child[i]->origin(), 0.2f,0.2f,1.0f, true, 1.0f);

		child[i]->drawBranches();
	}
}

bool RigidPart::isIntersects(Mesh *mesh)
{
	updateMatrix();
	return shape->isIntersects(mat, mesh);
}

bool RigidPart::intersectWithMesh(Mesh *mesh)
{
	Stack<RigidPart*> stack;
	stack.push(this);

	while( !stack.empty() ) 
	{
		RigidPart* r = stack.top();	stack.pop();

		if( r->isIntersects(mesh) )
			return true;

		for(int i = 0; i < (int) r->child.size(); i++)
			stack.push(r->child[i]);
	}

	return false;
}

void RigidPart::tranlsateAnchor( const Vec & delta )
{
	anchorPos += delta;
}

void RigidPart::rotateAnchor( double x, double y, double z )
{
	anchorRotations += Vec(x,y,z);
}

void RigidPart::setMat( const QMatrix4x4 & fromMat )
{
	mat = fromMat;
	updateMatrix();
}

Vec RigidPart::origin()
{
	return localPoint(Vec());
}

Vec RigidPart::localPoint( const Vec & delta )
{
	updateMatrix();

	QVector4D p = mat.map( QVector4D (delta.x, delta.y, delta.z, 1) );

	return Vec(p.x(), p.y(), p.z());
}

Vec RigidPart::maxExtent()
{
	double MAX = MaxOf(shape->x, shape->y, shape->z);
	if(shape->x == MAX) return Vec(shape->x, 0, 0);
	if(shape->y == MAX) return Vec(0, shape->y, 0);
	if(shape->z == MAX) return Vec(0, 0, shape->z);
	return Vec();
}

Vec RigidPart::maxExtentLocal()
{
	double MAX = MaxOf(shape->x, shape->y, shape->z);
	if(shape->x == MAX) return localPoint(Vec(shape->x, 0, 0));
	if(shape->y == MAX) return localPoint(Vec(0, shape->y, 0));
	if(shape->z == MAX) return localPoint(Vec(0, 0, shape->z));
	return localPoint(Vec());
}

Vector<Vec> RigidPart::getCorners()
{
	Vector<Vec> corners;
	
	double wx = shape->x;
	double wy = shape->y * 0.5;
	double wz = shape->z * 0.5;

	// Bottom
	corners.push_back(localPoint(Vec(0,wy,-wz)));
	corners.push_back(localPoint(Vec(wx,wy,-wz)));
	corners.push_back(localPoint(Vec(0,-wy,-wz)));
	corners.push_back(localPoint(Vec(wx,-wy,-wz)));

	// Top
	corners.push_back(localPoint(Vec(wx,-wy,wz)));
	corners.push_back(localPoint(Vec(0,-wy,wz)));
	corners.push_back(localPoint(Vec(wx,wy,wz)));
	corners.push_back(localPoint(Vec(0,wy,wz)));

	return corners;
}

Vec RigidPart::getCenter()
{
	Vector<Vec> corners = getCorners();

	Vec center;

	for(int i = 0; i < (int)corners.size(); i++)
		center += corners[i];

	return center /= (double)corners.size();
}

Mesh * RigidPart::toMesh()
{
	Mesh * m = new Mesh();

	int vindex = 0;
	int findex = 0;

	m->id = qPrintable("RigidPart-" + name);

	Stack<RigidPart*> stack;
	stack.push(this);

	while( !stack.empty() ) 
	{
		RigidPart* r = stack.top();	stack.pop();
		
		// Process current rigid part
		{
			KDTree vertices;
			Vector<Vec> tempMesh = r->shape->getFacesAsVertices();

			// Transform shape
			for(int i = 0; i < (int) tempMesh.size(); i++)
			{
				tempMesh[i] = r->localPoint(tempMesh[i]);
			}
		
			// Add vertices
			foreach(Vec v, tempMesh)
			{
				if(!vertices.has(v, 0.0001))
				{
					vertices.insert(v, vindex);
					m->addVertex(v, vindex++);
				}
			}

			// Add faces
			for(int i = 0; i < (int) tempMesh.size(); i += 3)
			{
				int v1 = vertices.getData(tempMesh[i]);
				int v2 = vertices.getData(tempMesh[i+1]);
				int v3 = vertices.getData(tempMesh[i+2]);

				if(r->hasReflectedParent()) 
				{
					SWAP(v1, v3, int);
				}

				m->addFace(v1,v2,v3, findex++);
			}
		}

		// Add my children
		for(int i = 0; i < (int) r->child.size(); i++)
			stack.push(r->child[i]);
	}

	return m;
}

void RigidPart::updateMatrix()
{
	if(parent != NULL)
		mat = parent->mat;

	// Reflections
	if(isReflect){
		QVector4D r = mat.row(0);

		switch(reflectAxis){
			case 'x': r.setX(-r.x()); break;
			case 'y': r.setY(-r.y()); break;
			case 'z': r.setZ(-r.z()); break;
		}

		mat.setRow(0, r);
	}

	mat.translate(anchorPos.x, anchorPos.y, anchorPos.z);

	// Anchor rotation
	mat.rotate(anchorRotations.z, 0, 0, 1);
	mat.rotate(anchorRotations.y, 0, 1, 0);
	mat.rotate(anchorRotations.x, 1, 0, 0);

	// Local rotation
	if (rot.allowZ) mat.rotate(-rot.z(), 0, 0, 1);
	if (rot.allowY) mat.rotate(-rot.y(), 0, 1, 0);
	if (rot.allowX) mat.rotate(-rot.x(), 1, 0, 0);

	// Recurse
	for(int i = 0; i < (int)child.size(); i++)
		child[i]->updateMatrix();
}

bool RigidPart::hasReflectedParent()
{
	if(isReflect) 
		return true;
	else
	{
		if(parent != NULL)
			return parent->hasReflectedParent();
	}

	return false;
}
