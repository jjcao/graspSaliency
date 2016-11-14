#pragma once

int tri_tri_intersect(float V0[3],float V1[3],float V2[3],  float U0[3],float U1[3],float U2[3]);
int tri_tri_intersect_with_isectline(float V0[3],float V1[3],float V2[3],
                                     float U0[3],float U1[3],float U2[3],
									 int *coplanar, float isectpt1[3],float isectpt2[3]);

int NoDivTriTriIsect(double * V0, double *  V1, double *V2,
					 double * U0, double *  U1, double *U2);
