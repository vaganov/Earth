#include <math.h>
#include "sphere_private.h"

void Sphere::cross (double x1, double y1, double z1, double x2, double y2, double z2, double& nx, double& ny, double& nz) {
    nx = y1 * z2 - y2 * z1;
    ny = z1 * x2 - z2 * x1;
    nz = x1 * y2 - x2 * y1;
}

