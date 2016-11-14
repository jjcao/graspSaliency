#include "rotationConstraint.h"

#include "Sleeper.h"

RotationConstraint::RotationConstraint( bool allowX, bool allowY, bool allowZ, 
									   const AngleRange & range_x, 
									   const AngleRange & range_y, 
									   const AngleRange & range_z)
{
	this->allowX = allowX;
	this->allowY = allowY;
	this->allowZ = allowZ;

	this->rangeX = range_x;
	this->rangeY = range_y;
	this->rangeZ = range_z;

	rotx = 0;
	roty = 0;
	rotz = 0;

	saveAngles();
}

RotationConstraint::RotationConstraint(const RotationConstraint& other)
{
	this->allowX = other.allowX;
	this->allowY = other.allowY;
	this->allowZ = other.allowZ;

	this->rangeX = other.rangeX;
	this->rangeY = other.rangeY;
	this->rangeZ = other.rangeZ;

	this->rotx = other.rotx;
	this->roty = other.roty;
	this->rotz = other.rotz;

	this->prev_rotx = other.prev_rotx;
	this->prev_roty = other.prev_roty;
	this->prev_rotz = other.prev_rotz;
}

void RotationConstraint::setRanges( const AngleRange & range_x, 
								   const AngleRange & range_y, 
								   const AngleRange & range_z)
{
	this->rangeX = range_x;
	this->rangeY = range_y;
	this->rangeZ = range_z;
}

bool RotationConstraint::addAngle( char axis, double delta, bool isStep )
{
	// This function acts like a switch / handler

	if(isStep)
	{
		return addToAngle(axis, delta);
	}
	else
	{
		setAngle(axis, delta);
		return false;
	}
}

bool RotationConstraint::addToAngle( char axis, double delta )
{
	double futureAngle = 0;

	// helpful so we can undo
	saveAngles();

	//Sleeper::msleep(200);

	switch (axis){
		case 'x':
			if(allowX)
			{
				futureAngle = rotx + delta;

				if(rangeX.withinRange(futureAngle))
				{	
					rotx = futureAngle;
					return true;
				}

			}break;

		case 'y':
			if(allowY)
			{
				futureAngle = roty + delta;

				if(rangeY.withinRange(futureAngle))
				{	
					roty = futureAngle;
					return true;
				}

			}break;

		case 'z':
			if(allowZ)
			{
				futureAngle = rotz + delta;

				if(rangeZ.withinRange(futureAngle))
				{	
					rotz = futureAngle;
					return true;
				}

			}break;

		default: break;
	}

	return false;
}

void RotationConstraint::setAngle( char axis, double theta )
{
	switch (axis)
	{
		case 'x': if(allowX) rotx = rangeX.ranged(theta); break;
		case 'y': if(allowY) roty = rangeY.ranged(theta); break;
		case 'z': if(allowZ) rotz = rangeZ.ranged(theta); break;
	}
}

void RotationConstraint::saveAngles()
{
	prev_rotx = rotx;
	prev_roty = roty;
	prev_rotz = rotz;
}

void RotationConstraint::undo()
{
	rotx = prev_rotx;
	roty = prev_roty;
	rotz = prev_rotz;
}

void RotationConstraint::reset()
{
	setToMin();
}

void RotationConstraint::setToMax()
{
	rotx = rangeX.maxAngle;
	roty = rangeY.maxAngle;
	rotz = rangeZ.maxAngle;
}

void RotationConstraint::setToMin()
{
	rotx = rangeX.minAngle;
	roty = rangeY.minAngle;
	rotz = rangeZ.minAngle;
}
