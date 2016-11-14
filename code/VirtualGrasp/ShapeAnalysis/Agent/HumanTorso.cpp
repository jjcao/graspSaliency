#include "HumanTorso.h"

HumanTorso::HumanTorso( double torsoWidth, double torsoHeight, RigidPart * parent )
{
	this->width = torsoWidth;
	this->height = torsoHeight;
	double thick = torsoWidth * 0.4;

	pelvis = parent->addChild( new Box(width * 0.7, thick, height * 0.32), "Pelvis" );
	middle = parent->addChild( new Box( width * 0.8, thick, height * 0.40 ), "MiddleBack" );
	shoulder = middle->addChild( new Box( width, thick, height * 0.29 ), "Sholder" );

	//middle->tranlsateAnchor( Vec(0,0,parent->shape->z) );
	shoulder->tranlsateAnchor( Vec(0,0,middle->shape->z) );
	//pelvis->tranlsateAnchor( Vec(0,0,-pelvis->shape->z) );

	pelvis->rotateAnchor(5,180,0);
	middle->rotateAnchor(5,0,0);
	shoulder->rotateAnchor(-5,0,0);
}

void HumanTorso::draw()
{
	
}
