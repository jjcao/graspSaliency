#include "isa_headers.h"
#include "RigidPartWidget.h"

RigidPartWidget::RigidPartWidget( RigidPart * fromRoot, QWidget *parent ) : QWidget(parent)
{
	if(!fromRoot) return;

	this->root = fromRoot;

	QVBoxLayout * layout = new QVBoxLayout;

	// Selector for rigid part
	explorer = new RigidPartExplorer(root, this);
	layout->addWidget(explorer);

	// Properties
	propertyBrowser = new RigidPartPropertyBrowser(root, this);
	layout->addWidget(propertyBrowser);
	
	// Connections
	this->connect(explorer, SIGNAL(currentItemChanged (QTreeWidgetItem *,QTreeWidgetItem *)), 
							SLOT(partChanged(QTreeWidgetItem *,QTreeWidgetItem *)));
	this->connect(propertyBrowser, SIGNAL(itemChanged(QTreeWidgetItem *, int)), 
							SLOT(propertyChanged(QTreeWidgetItem *,int)), Qt::QueuedConnection);
	this->setLayout(layout);
}

void RigidPartWidget::partChanged( QTreeWidgetItem * cur, QTreeWidgetItem * pre )
{
	propertyBrowser->part = explorer->parts[cur->text(0)];

	propertyBrowser->updateValues();
}

void RigidPartWidget::propertyChanged( QTreeWidgetItem * item, int column )
{
	// just update everything, for now..

	RigidPart * p = propertyBrowser->part;

	QTreeWidgetItem * color = propertyBrowser->properties["Color"];
	QTreeWidgetItem * apos = propertyBrowser->properties["anchorPos"];
	QTreeWidgetItem * arot = propertyBrowser->properties["anchorRotations"];
	QTreeWidgetItem * rot = propertyBrowser->properties["rot"];

	p->color.set(color->text(1).toInt(), color->text(2).toInt(), color->text(3).toInt());

	Vec newAnchor (apos->text(1).toDouble(), apos->text(2).toDouble(), apos->text(3).toDouble());
	p->anchorPos = newAnchor;
	p->anchorRotations = Vec(arot->text(1).toDouble(), arot->text(2).toDouble(), arot->text(3).toDouble());

	p->rot.setAngle('x', rot->text(1).toDouble());
	p->rot.setAngle('y', rot->text(2).toDouble());
	p->rot.setAngle('z', rot->text(3).toDouble());

	// Refresh display
	mainWindow->ui.viewer->updateGL();
}

RigidPartExplorer::RigidPartExplorer( RigidPart * root, QWidget * parent ) : QTreeWidget(parent)
{
	this->headerItem()->setText(0,"Select Part");

	QTreeWidgetItem * rootItem = new QTreeWidgetItem(this);
	rootItem->setText(0, root->name);

	addNode(root, rootItem);
}

void RigidPartExplorer::addNode( RigidPart * part, QTreeWidgetItem * parent )
{
	parts[part->name] = part;

	for(int i = 0; i < (int)part->child.size(); i++){
		RigidPart * child = part->child[i];
		parts[child->name] = child;

		QTreeWidgetItem * parentItem = new QTreeWidgetItem(parent);
		parentItem->setText(0, child->name);

		addNode(child, parentItem);
	}
}

RigidPartPropertyBrowser::RigidPartPropertyBrowser( RigidPart * p, QWidget * parent ) : QTreeWidget(parent)
{
	this->part = p;
	setColumnCount(4);

	this->headerItem()->setText(1,tr(""));
	this->headerItem()->setText(2,tr(""));
	this->headerItem()->setText(3,tr(""));

	// Create properties
	createProperty("Color", p->color.m_v[0], p->color.m_v[1], p->color.m_v[2]);
	createProperty("Children", p->child.size());
	createProperty("anchorPos", p->anchorPos.x, p->anchorPos.y, p->anchorPos.z);
	createProperty("anchorRotations", p->anchorRotations.x, p->anchorRotations.y, p->anchorRotations.z);
	createProperty("rot", p->rot.rotx, p->rot.roty, p->rot.rotz);
	createProperty("isReflect", p->isReflect);

	updateValues();
}

void RigidPartPropertyBrowser::updateValues()
{
	RigidPart * p = part;
	if(!p) return;

	this->headerItem()->setText(0,QString("%1").arg(part->name));

	setProperty("Color", p->color.m_v[0], p->color.m_v[1], p->color.m_v[2]);
	setProperty("Children", p->child.size());
	setProperty("anchorPos", p->anchorPos.x, p->anchorPos.y, p->anchorPos.z);
	setProperty("anchorRotations", p->anchorRotations.x, p->anchorRotations.y, p->anchorRotations.z);
	setProperty("rot", p->rot.rotx, p->rot.roty, p->rot.rotz);
	setProperty("isReflect", p->isReflect);

	this->header()->resizeSections(QHeaderView::ResizeToContents);
}

void RigidPartPropertyBrowser::createProperty(QString propertyName, QVariant c1, QVariant c2, QVariant c3)
{
	properties[propertyName] = new QTreeWidgetItem(this);
	properties[propertyName]->setChildIndicatorPolicy(QTreeWidgetItem::DontShowIndicator);
	properties[propertyName]->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled |Qt::ItemIsEditable);

	setProperty(propertyName, c1,c2,c3);
}

void RigidPartPropertyBrowser::setProperty(QString propertyName, QVariant c1, QVariant c2, QVariant c3)
{
	QTreeWidgetItem * p = properties[propertyName];

	p->setText(0, propertyName);
	p->setText(1, c1.toString());
	p->setText(2, c2.toString());
	p->setText(3, c3.toString());
}
