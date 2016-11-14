#include "HumanArm.h"

HumanArm::HumanArm(bool isRightArm, RigidPart * parent, double length)
{
	this->isRight = isRightArm;
	this->armLength = length; //0.24 + 0.35 + 0.40 = 78.0 cm
	this->parent = parent;

	QString direction = "right";
	if(!isRightArm) direction = "left";

	upperarm = parent->addChild(new Box(0.13 * length, 0.13 * length, 0.37 * length), QString("%1-upperarm").arg(direction));
	forearm = upperarm->addChild(new Box(0.05 * length, 0.05 * length, 0.35 * length), QString("%1-forearm").arg(direction));
	
	hand = new HumanHand();
	hand->name = direction + QString("-hand");
	hand->init(1, 0.27 * length, 0.15 * length, forearm);

	forearm->addChild(hand); // special case
	
	forearm->tranlsateAnchor(Vec(0,0,upperarm->shape->z));
	hand->tranlsateAnchor(Vec(0,0,forearm->shape->z));
	hand->rotateAnchor(0,0,180);

	if(!isRightArm) {
		upperarm->isReflect = true;
		upperarm->reflectAxis = 'x';
	}

	Vec armAnchor (parent->shape->x * 0.5, 0, parent->shape->z - (upperarm->shape->y * 0.5) );
	
	upperarm->tranlsateAnchor(armAnchor);
	upperarm->rotateAnchor(0,90,0);

}

void HumanArm::draw()
{
	upperarm->draw();
}
