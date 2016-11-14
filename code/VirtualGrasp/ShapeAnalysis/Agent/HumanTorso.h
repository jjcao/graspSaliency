#pragma once

#include "RigidPart.h"

class HumanTorso{
private:

public:
	HumanTorso(double torsoWidth, double torsoHeight, RigidPart * parent);

	double width, height;

	RigidPart * middle;
	RigidPart * shoulder;

	RigidPart * pelvis;

	void draw();
};
