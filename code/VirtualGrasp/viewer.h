#pragma once
#include <QGLViewer/qglviewer.h>

enum ViewMode { VIEW, SELECTION, MODIFY };
enum SelectMode { NONE, MESH, SKELETON_NODE, SKELETON_EDGE, SKELETON_FACES, RECONSTRUCTED_POINTS, VERTEX };
enum ModifyMode { DEFAULT, CP_REF_VECTOR, MOVE_VERTEX };

class Viewer : public QGLViewer
{
public:
	Viewer(QWidget *parent = 0);
	
	// Mouse & Keyboard stuff
	virtual void mousePressEvent(QMouseEvent* e); // no use unitll now
	virtual void mouseReleaseEvent(QMouseEvent* e); // no use unitll now
	virtual void mouseMoveEvent(QMouseEvent* e);
	virtual void keyPressEvent(QKeyEvent *e);
public slots:
	void updateHand();
protected:
	virtual void draw();
	virtual void init();
	virtual QString helpString() const;
private:
	bool mouseAltPressed;
	int renderStyle;
	QColor backColor;

	// STATE
	ViewMode viewMode;
	SelectMode selectMode;
	ModifyMode modifyMode;
};

