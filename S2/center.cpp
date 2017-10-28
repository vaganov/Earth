#include <math.h>
#include "sphere_private.h"

void Sphere::center (double phi, double psi) {
    double x, y, z;
    double xn, yn, zn;
    double c, sv, s;
    double T1, T2, T3;
    double t1, t2, t3;
    double M[3][3];
    double P_tmp[3][3];
    double XN, YN;
    double N;
    double r;
    XYZ(phi, psi, x, y, z);
    cross(x, y, z, X0, Y0, Z0, xn, yn, zn);
    if (normalize(xn, yn, zn)) {
        c = dot(x, y, z, X0, Y0, Z0);
        if (c >= 1) {
            sv = 0;
            s = 0;
        }
        else {
            sv = 1 - c;
            s = sqrt(1 - c * c);
        }
        T1 = sv * yn * zn;
        T2 = sv * xn * zn;
        T3 = sv * xn * yn;
        t1 = s * xn;
        t2 = s * yn;
        t3 = s * zn;
        M[0][0] = c + sv * xn * xn;
        M[0][1] = T3 - t3;
        M[0][2] = T2 + t2;
        M[1][0] = T3 + t3;
        M[1][1] = c + sv * yn * yn;
        M[1][2] = T1 - t1;
        M[2][0] = T2 - t2;
        M[2][1] = T1 + t1;
        M[2][2] = c + sv * zn * zn;
        P_tmp[0][0] = M[0][0] * P[0] + M[0][1] * P[1] + M[0][2] * P[2];
        P_tmp[0][1] = M[0][0] * P[4] + M[0][1] * P[5] + M[0][2] * P[6];
        P_tmp[0][2] = M[0][0] * P[8] + M[0][1] * P[9] + M[0][2] * P[10];
        P_tmp[1][0] = M[1][0] * P[0] + M[1][1] * P[1] + M[1][2] * P[2];
        P_tmp[1][1] = M[1][0] * P[4] + M[1][1] * P[5] + M[1][2] * P[6];
        P_tmp[1][2] = M[1][0] * P[8] + M[1][1] * P[9] + M[1][2] * P[10];
        P_tmp[2][0] = M[2][0] * P[0] + M[2][1] * P[1] + M[2][2] * P[2];
        P_tmp[2][1] = M[2][0] * P[4] + M[2][1] * P[5] + M[2][2] * P[6];
        P_tmp[2][2] = M[2][0] * P[8] + M[2][1] * P[9] + M[2][2] * P[10];
        P[0] = P_tmp[0][0];
        P[4] = P_tmp[0][1];
        P[8] = P_tmp[0][2];
        P[1] = P_tmp[1][0];
        P[5] = P_tmp[1][1];
        P[9] = P_tmp[1][2];
        P[2] = P_tmp[2][0];
        P[6] = P_tmp[2][1];
        P[10] = P_tmp[2][2];
        X0 = P[2];
        Y0 = P[6];
        Z0 = P[10];
    }
    XN = P[8];
    YN = P[9];
    N = XN * XN + YN * YN;
    if (N) {
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

