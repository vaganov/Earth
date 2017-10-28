#include <math.h>
#include "sphere_private.h"

void Sphere::XYZ (double phi, double psi, double& x, double& y, double& z) {
    double cosphi = cos(phi);
    x = cosphi * cos(psi);
    y = cosphi * sin(psi);
    z = sin(phi);
}

void Sphere::S2 (double x, double y, double z, double& phi, double& psi) {
    phi = asin(z);
    psi = atan(y / x);
    if (x < 0) {
        if (psi > 0) {
            psi -= M_PI;
        }
        else {
            psi += M_PI;
        }
    }
}

