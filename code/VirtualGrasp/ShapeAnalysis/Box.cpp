//#include "isa_headers.h"
#include "Box.h"

#include "IntersectionTest.h"

static const int boxIndex[][3]={
	/* 0 */ {7, 3, 0},
	/* 1 */ {0, 4, 7}, // back

	/* 2 */ {1, 2, 6},
	/* 3 */ {6, 5, 1}, // front

	/* 4 */ {2, 1, 0},
	/* 5 */ {3, 2, 0}, // bottom

	/* 6 */ {4, 5, 6},
	/* 7 */ {4, 6, 7}, // top

	/* 8 */ {0, 1, 5},
	/* 9 */ {5, 4, 0}, // right

	/* 10 */ {2, 3, 7},
	/* 11 */ {7, 6, 2}  // left
};

void Box::draw( const Color4 & color )
{
	SimpleDraw::DrawSolidBox(Vec(0,0,z * 0.5), x, y, z, color.r(), color.g(), color.b(), color.a());
}

bool Box::isIntersects( const QMatrix4x4 & mat, Mesh * mesh )
{
	double width  = x / 2.0;
	double length = y / 2.0;
	double height = z;

	QVector4D vert[8];

	vert[0] = QVector4D(width, length, 0, 1) ;
	vert[1] = QVector4D(-width, length, 0, 1) ;
	vert[2] = QVector4D(-width, -length, 0, 1) ;
	vert[3] = QVector4D(width, -length, 0, 1) ;

	vert[4] = QVector4D(width, length, height, 1) ;
	vert[5] = QVector4D(-width, length, height, 1) ;
	vert[6] = QVector4D(-width, -length, height, 1) ;
	vert[7] = QVector4D(width, -length, height, 1) ;

	for(int i = 0; i < 8; i++)
	{
		// Center to anchor
		vert[i] = mat * vert[i];

		//DEBUG:
		//testPoints1.push_back(Vec(vert[i].x(), vert[i].y(), vert[i].z()));
	}

	for(int boxFaceId = 0; boxFaceId < 4; boxFaceId++)
	{
		QVector4D u0 = vert[ boxIndex[boxFaceId][0] ];
		QVector4D u1 = vert[ boxIndex[boxFaceId][1] ];
		QVector4D u2 = vert[ boxIndex[boxFaceId][2] ];

		Vec U0(u0.x(), u0.y(), u0.z());
		Vec U1(u1.x(), u1.y(), u1.z());
		Vec U2(u2.x(), u2.y(), u2.z());

		vector<BaseTriangle*> tris = mesh->octree->getIntersectingTris(U0,U1,U2);

		for(int faceId = 0; faceId < (int)tris.size(); faceId++)
		{
			BaseTriangle *tri = tris[faceId];

			if ( NoDivTriTriIsect(tri->vec(0), tri->vec(1), tri->vec(2), U0, U1, U2) )
				return true;
		}
	}

	return false;
}

Vector<Vec> Box::getFacesAsVertices()
{
	Vector<Vec> result;

	double width  = x / 2.0, length = y / 2.0, height = z;

	Vec vert[8];

	vert[0] = Vec(width, length, 0) ;
	vert[1] = Vec(-width, length, 0) ;
	vert[2] = Vec(-width, -length, 0) ;
	vert[3] = Vec(width, -length, 0) ;

	vert[4] = Vec(width, length, height) ;
	vert[5] = Vec(-width, length, height) ;
	vert[6] = Vec(-width, -length, height) ;
	vert[7] = Vec(width, -length, height) ;

	for(int boxFaceId = 0; boxFaceId < 12; boxFaceId++)
	{
		result.push_back(vert[boxIndex[boxFaceId][0]]);
		result.push_back(vert[boxIndex[boxFaceId][1]]);
		result.push_back(vert[boxIndex[boxFaceId][2]]);
	}

	return result;
}
