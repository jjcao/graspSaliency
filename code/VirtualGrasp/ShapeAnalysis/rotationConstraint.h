#pragma once

#include "Mesh.h"

struct AngleRange{
	double minAngle, maxAngle;

	AngleRange(double n = -180, double x = 180) : minAngle(n), maxAngle(x){}

	bool withinRange(double theta){
		return RANGE(theta, minAngle, maxAngle);
	};

	inline double ranged(double theta) const {return RANGED(minAngle, theta, maxAngle);}

	inline void setMin(double angle){minAngle = angle;}
	inline void setMax(double angle){maxAngle = angle;}
};

class RotationConstraint{

private:

public:

	bool allowX, allowY, allowZ;
	AngleRange rangeX, rangeY, rangeZ;
	double rotx, roty, rotz;

	// Undo
	double prev_rotx, prev_roty, prev_rotz;

	RotationConstraint(bool allowX = true, bool allowY = true, bool allowZ = true, 
		const AngleRange & range_x = AngleRange(), 
		const AngleRange & range_y = AngleRange(), 
		const AngleRange & range_z = AngleRange());
	RotationConstraint(const RotationConstraint& other);

	void setRanges(const AngleRange & range_x, const AngleRange & range_y, const AngleRange & range_z);

	inline void setRangeX(const AngleRange & range){rangeX = range; rotx = range.minAngle;}
	inline void setRangeY(const AngleRange & range){rangeY = range; roty = range.minAngle;}
	inline void setRangeZ(const AngleRange & range){rangeZ = range; rotz = range.minAngle;}

	inline double x() {return rotx;}
	inline double y() {return roty;}
	inline double z() {return rotz;}

	inline void disableX(){allowX = false;}
	inline void disableY(){allowY = false;}
	inline void disableZ(){allowZ = false;}

	void reset();
	void setToMax();
	void setToMin();

	bool addAngle( char axis, double delta, bool isStep = true);
	bool addToAngle(char axis = 'x', double delta = 1.0);
	void setAngle(char axis = 'x', double theta = 1.0);
	
	void saveAngles();
	void undo();
};
