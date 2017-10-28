#include <math.h>
#include "sphere_private.h"

bool Sphere::coord (double x, double y, double& X, double& Y, double& Z) {
    double N = x * x + y * y;
    double z;
    if (N <= 1) {
        z = sqrt(1 - N);
        X = P[0] * x + P[1] * y + P[2] * z;
        Y = P[4] * x + P[5] * y + P[6] * z;
        Z = P[8] * x + P[9] * y + P[10] * z;
        return true;
    }
    else {
        return false;
    }
}

