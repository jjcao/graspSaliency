#include "HumanLeg.h"

HumanLeg::HumanLeg( bool isRightLeg, RigidPart * parent, double height /*= 83.3*/ )
{
	this->legLength = height;
	this->isRight = isRightLeg;

	double thick = legLength * 0.1;

	QString direction = "right";
	if(!isRightLeg) direction = "left";

	thigh = parent->addChild(new Box( thick, thick, legLength * 0.5), direction + QString("-Thigh"));
	leg = thigh->addChild(new Box( thick * 0.6, thick * 0.6, legLength * 0.42), direction + QString("-Leg"));
	ankle = leg->addChild(new Box(thick * 0.7), direction + QString("-Ankle"));
	foot = ankle->addChild(new Box( thick * 0.7, legLength * 0.31, legLength * 0.07), direction + QString("-Foot"));

	thigh->tranlsateAnchor(Vec((parent->shape->x * 0.5) - (thigh->shape->x * 0.5), 0, parent->shape->z));
	leg->tranlsateAnchor(Vec(0, 0, thigh->shape->z));
	ankle->tranlsateAnchor(Vec(0, 0, leg->shape->z));
	foot->tranlsateAnchor(Vec(0,(foot->shape->y * 0.5) - (ankle->shape->y * 0.5),0));

	thigh->rotateAnchor(-8,2,0);
	leg->rotateAnchor(8,0,0);
	ankle->rotateAnchor(-5,-2,-10);

	if(!isRightLeg){
		thigh->isReflect = true;
		thigh->reflectAxis = 'x';
	}
}

void HumanLeg::draw()
{
	
}
