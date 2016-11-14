#ifndef UTIL_H
#define UTIL_H


#define EPSILON 0.000001

#define CROSS(dest, v1, v2){                 \
	dest[0] = v1[1] * v2[2] - v1[2] * v2[1]; \
	dest[1] = v1[2] * v2[0] - v1[0] * v2[2]; \
	dest[2] = v1[0] * v2[1] - v1[1] * v2[0];}

#define DOT(v1, v2) (v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2])

#define SUB(dest, v1, v2){       \
	dest[0] = v1[0] - v2[0]; \
	dest[1] = v1[1] - v2[1]; \
	dest[2] = v1[2] - v2[2];}



class Util
{
public:
	Util();
	void fromToRotation(double from[3], double to[3], double mtx[3][3]);
};

#define ToQMatrix(x) QMatrix4x4(x[0][0],x[0][1],x[0][2],x[0][3], \
	x[1][0],x[1][1],x[1][2],x[1][3], \
	x[2][0],x[2][1],x[2][2],x[2][3], \
	x[3][0],x[3][1],x[3][2],x[3][3])

#endif // UTIL_H
