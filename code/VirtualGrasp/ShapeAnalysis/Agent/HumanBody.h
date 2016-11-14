#pragma once

#include "HumanTorso.h"
#include "HumanLeg.h"
#include "HumanArm.h"

#include "Sphere.h"

class HumanBody : public RigidPart{
private:

public:
	double width;
	double height;

	RigidPart * head;
	RigidPart * neck;

	HumanTorso * torso;

	HumanArm * armRight;
	HumanArm * armLeft;

	HumanLeg * legRight;
	HumanLeg * legLeft;

	HumanBody(double humanHeight = 171.6, double humanWidth = 35.2);

	bool isReady;

	void motion(double delta);

	void draw();
};
