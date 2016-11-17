#ifndef COMMANDER_H
#define COMMANDER_H

#include <QWidget>
#include <QString>

class Viewer;

class Commander : public QWidget
{
	Q_OBJECT

public:
	Commander(QWidget *parent = 0);
	~Commander();

	Viewer * viewer;
	QString loadedFileName;

public slots:

	// LOAD DATA / SAVE
	void OpenMesh(QString fileName);
};

#endif // COMMANDER_H
