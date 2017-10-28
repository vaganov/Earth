#include <math.h>
#include "sphere_private.h"

#include "debug.h"

void Sphere::start_rot (double x, double y) {
    double z;
    double N;
    double r;
    N = x * x + y * y;
    if (N > 1) {
        r = sqrt(N);
        x /= r;
        y /= r;
        z = 0;
    }
    else {
        z = sqrt(1 - N);
    }
    x0_rot = x;
    y0_rot = y;
    z0_rot = z;
    P0[0][0] = P[0];
    P0[0][1] = P[4];
    P0[0][2] = P[8];
    P0[1][0] = P[1];
    P0[1][1] = P[5];
    P0[1][2] = P[9];
    P0[2][0] = P[2];
    P0[2][1] = P[6];
    P0[2][2] = P[10];
}

void Sphere::rot (double x, double y) {
    double z;
    double N;
    double r;
    double xn, yn, zn;
    double c, s;
    double M[3][3];
    double XN, YN, ZN;
    double R;
    double D, d;
    double XN1, YN1, ZN1;
    double xn1, yn1, zn1;
    double xn2, yn2, zn2;
    double xn3, yn3, zn3;
    double P1[3][3];
    N = x * x + y * y;
    if (N > 1) {
        r = sqrt(N);
        x /= r;
        y /= r;
        z = 0;
    }
    else {
        z = sqrt(1 - N);
    }
    cross(x0_rot, y0_rot, z0_rot, x, y, z, xn, yn, zn);
    if (normalize(xn, yn, zn)) {
        c = dot(x0_rot, y0_rot, z0_rot, x, y, z);
        if (c >= 1) {
            s = 0;
        }
        else {
            s = sqrt(1 - c * c);
        }
        rot_M(xn, yn, zn, c, s, M);
        P[0] = M[0][0] * P0[0][0] + M[0][1] * P0[1][0] + M[0][2] * P0[2][0];
        P[4] = M[0][0] * P0[0][1] + M[0][1] * P0[1][1] + M[0][2] * P0[2][1];
        P[8] = M[0][0] * P0[0][2] + M[0][1] * P0[1][2] + M[0][2] * P0[2][2];
        P[1] = M[1][0] * P0[0][0] + M[1][1] * P0[1][0] + M[1][2] * P0[2][0];
        P[5] = M[1][0] * P0[0][1] + M[1][1] * P0[1][1] + M[1][2] * P0[2][1];
        P[9] = M[1][0] * P0[0][2] + M[1][1] * P0[1][2] + M[1][2] * P0[2][2];
        P[2] = M[2][0] * P0[0][0] + M[2][1] * P0[1][0] + M[2][2] * P0[2][0];
        P[6] = M[2][0] * P0[0][1] + M[2][1] * P0[1][1] + M[2][2] * P0[2][1];
        P[10] = M[2][0] * P0[0][2] + M[2][1] * P0[1][2] + M[2][2] * P0[2][2];
        if (north_auto) {
            XN = P[8];
            YN = P[9];
            ZN = P[10];
            R = dot(x, y, z, XN, YN, ZN);
            N = y * y + z * z;
            D = N - R * R;
            if (D >= 0) {
                d = sqrt(D);
                XN1 = 0;
                YN1 = (R * y + z * d) / N;
                ZN1 = (R * z - y * d) / N;
                cross(x, y, z, XN, YN, ZN, xn1, yn1, zn1);
                normalize(xn1, yn1, zn1);
                cross(x, y, z, XN1, YN1, ZN1, xn2, yn2, zn2);
                normalize(xn2, yn2, zn2);
                c = dot(xn1, yn1, zn1, xn2, yn2, zn2);
                if (c >= 1) {
                    s = 0;
                }
                else {
                    cross(XN, YN, ZN, XN1, YN1, ZN1, xn3, yn3, zn3);
                    if (dot(x, y, z, xn3, yn3, zn3) > 0) {
                        s = sqrt(1 - c * c);
                    }
                    else {
                        s = -sqrt(1 - c * c);
                    }
                }
                rot_M(x, y, z, c, s, M);
                P1[0][0] = M[0][0] * P[0] + M[0][1] * P[1] + M[0][2] * P[2];
                P1[0][1] = M[0][0] * P[4] + M[0][1] * P[5] + M[0][2] * P[6];
                P1[0][2] = M[0][0] * P[8] + M[0][1] * P[9] + M[0][2] * P[10];
                P1[1][0] = M[1][0] * P[0] + M[1][1] * P[1] + M[1][2] * P[2];
                P1[1][1] = M[1][0] * P[4] + M[1][1] * P[5] + M[1][2] * P[6];
                P1[1][2] = M[1][0] * P[8] + M[1][1] * P[9] + M[1][2] * P[10];
                P1[2][0] = M[2][0] * P[0] + M[2][1] * P[1] + M[2][2] * P[2];
                P1[2][1] = M[2][0] * P[4] + M[2][1] * P[5] + M[2][2] * P[6];
                P1[2][2] = M[2][0] * P[8] + M[2][1] * P[9] + M[2][2] * P[10];
                P[0] = P1[0][0];
                P[4] = P1[0][1];
                P[8] = P1[0][2];
                P[1] = P1[1][0];
                P[5] = P1[1][1];
                P[9] = P1[1][2];
                P[2] = P1[2][0];
                P[6] = P1[2][1];
                P[10] = P1[2][2];
            }
#ifdef _DEBUG
            else {
                cout << "rot(): fixed point too far\n";
            }
#endif
        }
        X0 = P[2];
        Y0 = P[6];
        Z0 = P[10];
    }
}

