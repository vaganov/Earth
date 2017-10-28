#include <math.h>
#include "sphere_private.h"

double Sphere::rho (double x1, double y1, double z1, double x2, double y2, double z2) {
    double c = dot(x1, y1, z1, x2, y2, z2);
    return acos(c);
}

double Sphere::rho (double x1, double y1, double z1, double x2, double y2, double z2, double r, bool right, bool clockwise) {
    double r0;
    double S, C;
    double c;
    double alpha;
    if (r > 0) {
        r0 = sin(r);
        S = r0 * r0;
        C = 1 - S;
        c = (dot(x1, y1, z1, x2, y2, z2) - C) / S;
        alpha = acos(c);
        if (right ^ clockwise) {
            return alpha * r0;
        }
        else {
            return (2 * M_PI - alpha) * r0;
        }
    }
    else {
        return 0;
    }
}

