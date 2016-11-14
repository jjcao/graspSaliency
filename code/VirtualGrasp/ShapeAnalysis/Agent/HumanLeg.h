#pragma once

#include "RigidPart.h"

class HumanLeg{
private:
	
public:
	HumanLeg(bool isRightLeg, RigidPart * parent, double height = 83.3);

	bool isRight;
	double legLength;

	RigidPart * thigh;
	RigidPart * leg;
	RigidPart * ankle;
	RigidPart * foot;

	void draw();
};
