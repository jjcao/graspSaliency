#pragma once

#define DEFAULT_HAND_LENGTH 19.41 
#define DEFAULT_HAND_WIDTH 9.04 

#include <iostream>
#include "RigidPart.h"

enum FeatureSpace { ANGLE, DIST_ANGLE, DISTANCE_JOINTS_ALL };

extern double defaultAngleStep;

class HumanHand : public RigidPart
{
private:

	double fingerWidth;
	double fingerThick;
	double HL, HB;

	int fingCount;
	int jointCount;

public:

	// Constructor
	HumanHand();
	//HumanHand& operator= (const HumanHand& from);

	void init(double scale = 1.0, double handLength = DEFAULT_HAND_LENGTH, 
		double handWidth = DEFAULT_HAND_WIDTH, RigidPart * parent = NULL);
	void setupThumb();

	// Basic open / close hand
	void openAll(double theta = -999, bool isStep = false);
	
	// Intersection and interactions
	void openUntilNonCollide( Mesh *mesh, double step = defaultAngleStep);
	void closeUntilCollide(Mesh *mesh, double step = defaultAngleStep);
	
	RigidPart * getFinger(int i) const;
	RigidPart * getThumb() const;

	double getFingerThick();
	double getHandLength();
	double getHandBreadth ();
	int getFingerCount() const;
	bool ready();

	// Grabability
	static Vector<double> bestGrab(FeatureSpace featureSpace = ANGLE);
	double difference(FeatureSpace featureSpace, const Vector<double> & toGrab);

	// Output
	Vector<double> toVector( FeatureSpace featureSpace );
	StdString toString( FeatureSpace featureSpace );
};
