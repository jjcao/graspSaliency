#include "isa_headers.h"
#include "Ruler.h"
#include "SimpleDraw.h"

void Ruler::draw( Mesh * m )
{
	double extent, thick, halfExtent, halfThick, depth;
	Vec centerX, centerY, tipX, tipY;

	if(m == NULL){
		extent = 40.0;
		thick = extent * 0.11;

		halfExtent = extent * 0.5;
		halfThick = thick * 0.5;

		depth = thick * 0.11f;
	}

	centerX.x = -halfExtent;
	centerX.y = halfThick + extent;

	centerY.x = -centerX.y;
	centerY.y = -centerX.x;

	centerX.z = centerY.z = depth * 0.5;

	glPushMatrix();
	glTranslated(halfExtent, -halfExtent, 0);

	SimpleDraw::DrawSolidBox(centerX, extent, thick, depth, 0.45f, 0.4f, 0.2f);
	SimpleDraw::DrawSolidBox(centerY, thick, extent, depth, 0.45f, 0.4f, 0.2f);

	glPushMatrix();
	glTranslated(-extent, extent, 0);

	double delta = extent / 10.0;

	for(double d = 0; d <= extent; d += delta)
	{
		// On the X-axis
		Vec p1(d, 0, depth);
		Vec p2(d, thick * 0.2f, depth);
		SimpleDraw::IdentifyLine(p1, p2, 0,0,0, false, 2);

		if(d < extent)
		{
			double deltaE = delta / 10.0;
			for(double e = d + deltaE; e < d + delta; e += deltaE)
			{
				Vec p1(e, 0, depth);
				Vec p2(e, thick * 0.1f, depth);
				SimpleDraw::IdentifyLine(p1, p2, 0,0,0, false, 1);
			}
		}

		// On the Y-axis
		Vec p3(0, -d, depth);
		Vec p4(-thick * 0.2f, -d, depth);
		SimpleDraw::IdentifyLine(p3, p4, 0,0,0, false, 2);

		if(d < extent)
		{
			double deltaE = delta / 10.0;
			for(double e = d + deltaE; e < d + delta; e += deltaE)
			{
				Vec p3(0, -e, depth);
				Vec p4(-thick * 0.1f, -e, depth);
				SimpleDraw::IdentifyLine(p3, p4, 0,0,0, false, 1);
			}
		}
	}

	glPopMatrix();

	glPopMatrix();

	Viewer * v = mainWindow->ui.viewer;
	v->makeCurrent();

	// Color of text
	glEnable(GL_BLEND); 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(1,1,1, 0.5);

	Vec proj = v->camera()->projectedCoordinatesOf(Vec(-halfExtent, halfExtent,0));
	v->drawText(proj.x, proj.y, QString::number(0));

	proj = v->camera()->projectedCoordinatesOf(Vec(-halfExtent, 0,0));
	v->drawText(proj.x, proj.y, QString::number(halfExtent));

	proj = v->camera()->projectedCoordinatesOf(Vec(-halfExtent, -halfExtent,0));
	v->drawText(proj.x, proj.y, QString::number(extent));

	proj = v->camera()->projectedCoordinatesOf(Vec(0, halfExtent,0));
	v->drawText(proj.x, proj.y, QString::number(halfExtent));

	proj = v->camera()->projectedCoordinatesOf(Vec(halfExtent, halfExtent,0));
	v->drawText(proj.x, proj.y, QString::number(extent));
}
