#pragma once

#include "HumanHand.h"

class HumanArm{
private:

public:
	HumanArm(bool isRightArm = true, RigidPart * parent = NULL, double length = 78.0);

	bool isRight;
	double armLength;
	RigidPart * parent;

	RigidPart * upperarm;
	RigidPart * forearm;
	HumanHand * hand;

	void draw();
};
  