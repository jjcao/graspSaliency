#include "stdafx.h"
#include "viewer.h"
#include <QKeyEvent>
using namespace std;

Viewer::Viewer(QWidget *parent) : QGLViewer(parent)
{
}

// Draws a spiral
void Viewer::draw()
{
	// Ambient light
	GLfloat ambientLight[] = { 0.15f, 0.15f, 0.15f, 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);

	this->setBackgroundColor(backColor);

	// Anti-aliasing
	glEnable(GL_MULTISAMPLE);	

	if (hand != NULL && hand->ready())
	{		
		hand->drawBranches();
		//hand->draw();
	}	
}

void Viewer::init()
{
	// Restore previous viewer state.
	restoreStateFromFile();
	// Opens help window
	help();

	GLfloat lightColor[] = { 0.9f, 0.9f, 0.9f, 1.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);

	// Material
	//float no_mat[] = {0.0f, 0.0f, 0.0f, 1.0f};
	//float mat_ambient_color[] = {0.25f, 0.25f, 0.25f, 1.0f};
	float mat_ambient[] = { 0.1745f, 0.01175f, 0.01175f, 1.0f };
	float mat_diffuse[] = { 0.65f, 0.045f, 0.045f, 1.0f };
	float mat_specular[] = { 0.09f, 0.09f, 0.09f, 1.0f };
	float high_shininess = 100;

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, high_shininess);

	this->setBackgroundColor(backColor = QColor(255, 255, 255));

	this->viewMode = VIEW;
	this->selectMode = NONE;
	this->modifyMode = DEFAULT;

	mouseAltPressed = false;

	// Default camera
	//if (actor != NULL)
	//{
	//	setSceneCenter(actor->getCenter());
	//	setSceneRadius(actor->height*1.2);
	//}
	if (hand != NULL)
	{
		setSceneCenter(hand->getCenter());
		setSceneRadius(hand->getHandLength()*1.2);
	}
	else
	{
		setSceneCenter(Vec());
	}
	
	camera()->showEntireScene();
	//setSceneRadius(80);
	update();
}

void Viewer::keyPressEvent(QKeyEvent *e)
{
	// open and close hand
	if (e->key() == Qt::Key_BracketLeft)
	{
		hand->openAll(-10.0, true);
	}
	if (e->key() == Qt::Key_BracketRight)
	{
		hand->openAll(10.0, true);
	}

	updateGL();

	// Regular behavior
	QGLViewer::keyPressEvent(e);
}
QString Viewer::helpString() const
{
	QString text("<h2>S i m p l e V i e w e r</h2>");
	text += "Use the mouse to move the camera around the object. ";
	text += "You can respectively revolve around, zoom and translate with the three mouse buttons. ";
	text += "Left and middle buttons pressed together rotate around the camera view direction axis<br><br>";
	text += "Pressing <b>Alt</b> and one of the function keys (<b>F1</b>..<b>F12</b>) defines a camera keyFrame. ";
	text += "Simply press the function key again to restore it. Several keyFrames define a ";
	text += "camera path. Paths are saved when you quit the application and restored at next start.<br><br>";
	text += "Press <b>F</b> to display the frame rate, <b>A</b> for the world axis, ";
	text += "<b>Alt+Return</b> for full screen mode and <b>Control+S</b> to save a snapshot. ";
	text += "See the <b>Keyboard</b> tab in this window for a complete shortcut list.<br><br>";
	text += "Double clicks automates single click actions: A left button double click aligns the closer axis with the camera (if close enough). ";
	text += "A middle button double click fits the zoom of the camera and the right button re-centers the scene.<br><br>";
	text += "A left button double click while holding right button pressed defines the camera <i>Revolve Around Point</i>. ";
	text += "See the <b>Mouse</b> tab and the documentation web pages for details.<br><br>";
	text += "Press <b>Escape</b> to exit the viewer.";
	return text;
}