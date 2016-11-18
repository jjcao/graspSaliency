#include "stdafx.h"
#include "commander.h"
#include <qfiledialog.h>

Commander::Commander(QWidget *parent)
	: QWidget(parent)
{
	// Nothing to show for..
	this->hide();
}

Commander::~Commander()
{

}

void Commander::OpenMesh(QString fileName)
{
	if (!fileName.length())
		fileName = QFileDialog::getOpenFileName(this, tr("Open Mesh"), "", tr("Mesh Files (*.obj *.off)"));

	if (fileName.length())
	{
		parentWidget()->setCursor(Qt::WaitCursor);
		if (mesh) {
			delete mesh; mesh = NULL;
		}
		mesh = new Mesh();

		// File type
		mesh->loadFromFile(fileName.toLatin1().data());

		mesh->opendFileName = fileName.toLatin1().data();

		// Build octree
		mesh->rebuildOctree();

		// Center camera on object
		viewer->setSceneCenter(mesh->center.vec());
		double radius = std::max(mesh->radius, hand->getHandLength()*1.2);
		viewer->setSceneRadius(radius);
		viewer->camera()->showEntireScene();
		//viewer->setSelectMode(MESH);
		viewer->update();
		

		// clear Side bar
		mainWindow->ui.sideBar->removeAllWidgets();
		if (saWidget) delete saWidget;
		saWidget = new ShapeAnalysisWidget(mainWindow->ui.sideBar);
		mainWindow->ui.sideBar->displayWidget(saWidget);

		loadedFileName = fileName;
		loadedFileName.chop(4);

		parentWidget()->setCursor(Qt::ArrowCursor);
	}
}
