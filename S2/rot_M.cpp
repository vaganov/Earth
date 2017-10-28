#include "sphere_private.h"

void Sphere::rot_M (double x, double y, double z, double cos, double sin, double M[3][3]) {
    double sv = 1 - cos;
    double T1 = sv * y * z;
    double T2 = sv * x * z;
    double T3 = sv * x * y;
    double t1 = sin * x;
    double t2 = sin * y;
    double t3 = sin * z;
    M[0][0] = cos + sv * x * x;
    M[0][1] = T3 - t3;
    M[0][2] = T2 + t2;
    M[1][0] = T3 + t3;
    M[1][1] = cos + sv * y * y;
    M[1][2] = T1 - t1;
    M[2][0] = T2 - t2;
    M[2][1] = T1 + t1;
    M[2][2] = cos + sv * z * z;
}

