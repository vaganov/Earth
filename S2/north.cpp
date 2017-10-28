#include <math.h>
#include "sphere_private.h"

void Sphere::north (void) {
    double XN = P[8];
    double YN = P[9];
    double N = XN * XN + YN * YN;
    double r;
    double c, s;
    double P_tmp[2][3];
    if (N > 0) {
        r = sqrt(N);
        c = YN / r;
        s = XN / r;
        P_tmp[0][0] = c * P[0] - s * P[1];
        P_tmp[0][1] = c * P[4] - s * P[5];
        P_tmp[0][2] = c * P[8] - s * P[9];
        P_tmp[1][0] = s * P[0] + c * P[1];
        P_tmp[1][1] = s * P[4] + c * P[5];
        P_tmp[1][2] = s * P[8] + c * P[9];
        P[0] = P_tmp[0][0];
        P[4] = P_tmp[0][1];
        P[8] = P_tmp[0][2];
        P[1] = P_tmp[1][0];
        P[5] = P_tmp[1][1];
        P[9] = P_tmp[1][2];
    }
}

