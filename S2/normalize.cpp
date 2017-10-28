#include <math.h>
#include "sphere_private.h"

bool Sphere::normalize (double& x, double& y, double& z) {
    double N = x * x + y * y + z * z;
    double r;
    if (N > 0) {
        r = sqrt(N);
        x /= r;
        y /= r;
        z /= r;
        return true;
    }
    else {
        return false;
    }
}

