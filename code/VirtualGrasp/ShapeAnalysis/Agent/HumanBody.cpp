#include "HumanBody.h"

HumanBody::HumanBody( double humanHeight /*= 171.6*/, double humanWidth /*= 35.2*/ )
{
	this->isReady = false;

	this->height = humanHeight;
	this->width = humanHeight * 0.20;

	double armLength = humanHeight * 0.36;
	double legLength = humanHeight * 0.48;

	this->shape = new Box( humanWidth * 0.1, humanWidth * 0.1, humanWidth * 0.1 );
	this->name = "HumanBody";

	// TORSO
	torso = new HumanTorso(width, humanHeight * 0.33, this);

	// Neck
	neck = torso->shoulder->addChild(new Box(width * 0.25, torso->shoulder->shape->y * 0.75, 0.047 * height), "Neck");
	neck->tranlsateAnchor(Vec(0, -(torso->shoulder->shape->y * 0.5) + (neck->shape->y * 0.5), torso->shoulder->shape->z));
	neck->rotateAnchor(-8,0,0);

	// HEAD
	double headRadius = 0.06 * height;
	head = neck->addChild(new Sphere(headRadius), "Head");
	head->tranlsateAnchor(Vec(0,0, headRadius + neck->shape->z));

	// ARM - Right
	armRight = new HumanArm(true, torso->shoulder, armLength);

	// ARM - Left
	armLeft = new HumanArm(false, torso->shoulder, armLength);

	// LEG - Right
	legRight = new HumanLeg( true, torso->pelvis, legLength );
	
	// LEG - Left
	legLeft = new HumanLeg( false, torso->pelvis, legLength );

	this->isReady = true;

	// Visualization
	color = Color4(255,0,0);
	setChildrenColor(Color4(0,100,255,100));
}

void HumanBody::draw()
{
	drawBranches();

	// Regular drawing of children
	RigidPart::draw();
}

void HumanBody::motion( double delta )
{
	torso->middle->rot.addAngle('x', delta);
	torso->shoulder->rot.addAngle('x', delta);

	legRight->thigh->rot.addAngle('x', delta * 0.5);
	legLeft->thigh->rot.addAngle('x', delta * 0.5);

	legRight->leg->rot.addAngle('x', delta);
	legLeft->leg->rot.addAngle('x', delta);
}
