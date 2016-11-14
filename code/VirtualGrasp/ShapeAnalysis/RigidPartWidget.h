#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QListWidget>
#include <QListWidgetItem>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QHeaderView>

#include "RigidPart.h"

class RigidPartExplorer : public QTreeWidget{
	Q_OBJECT
public:
	RigidPartExplorer(RigidPart * root, QWidget * parent);
	void addNode(RigidPart * part, QTreeWidgetItem * parent);
	QMap<QString, RigidPart *> parts;
};

class RigidPartPropertyBrowser : public QTreeWidget{
	Q_OBJECT
private:
	void createProperty(QString propertyName, QVariant c1, QVariant c2=QVariant(), QVariant c3=QVariant());
	void setProperty(QString propertyName, QVariant c1, QVariant c2=QVariant(), QVariant c3=QVariant());
public:
	RigidPartPropertyBrowser(RigidPart * p, QWidget * parent);
	RigidPart * part;
	QMap<QString, QTreeWidgetItem*>properties;
public slots:
	void updateValues();
};

class RigidPartWidget : public QWidget
{
	Q_OBJECT
public:
	RigidPart * root;
	RigidPartExplorer * explorer;
	RigidPartPropertyBrowser * propertyBrowser;

	RigidPartWidget(RigidPart * fromRoot, QWidget *parent = 0);

public slots:
	void partChanged(QTreeWidgetItem * cur, QTreeWidgetItem * pre);
	void propertyChanged(QTreeWidgetItem * item, int column);

signals:
};
