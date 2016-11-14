#pragma once

#include "Shape.h"

class Sphere : public Shape
{
public:
	Sphere(double r) { x = y = z = r; }
	virtual Shape* Clone() { return new Sphere(this->x); }

	inline double radius() { return x; };

	virtual void draw(const Color4 & color)
	{
		glColor4ubv(color.m_v);
		SimpleDraw::DrawSphere( Vec(), radius() );
	}

	virtual bool isIntersects(const QMatrix4x4 & mat, Mesh * mesh)
	{
		return false; // for now
	}

	virtual Vector<Vec> getFacesAsVertices()
	{
		Vector<Vec> result;

		int n = 20;
		double r = radius();

		double Half_PI = M_PI / 2.0;

		double dtheta = (M_PI * 2) / n;
		double dphi = (M_PI * 2) / n;

		// For each hight section
		for (double theta = -Half_PI; theta <= Half_PI - dtheta; theta += dtheta)
		{
			for (double phi = 0; phi <= (M_PI * 2) - dphi; phi += dphi)
			{
				Vec p[4];

				p[0] = r * Vec(cos(theta) * cos(phi),cos(theta) * sin(phi),sin(theta));
				p[1] = r * Vec(cos((theta+dtheta)) * cos(phi),cos((theta+dtheta)) * sin(phi),sin((theta+dtheta)));
				p[2] = r * Vec(cos((theta+dtheta)) * cos((phi+dphi)),cos((theta+dtheta)) * sin((phi+dphi)),sin((theta+dtheta)));
				
				if(theta < Half_PI - dtheta){
					result.push_back( p[0] );
					result.push_back( p[2] );
					result.push_back( p[1] );
				}

				if (theta > -Half_PI && theta < Half_PI){
					p[3] = r * Vec(cos(theta) * cos((phi+dphi)),cos(theta) * sin((phi+dphi)),sin(theta));
					result.push_back( p[2] );
					result.push_back( p[0] );
					result.push_back( p[3] );
				}
			}
		}
	
		return result;
	}
};
