#include <math.h>
#include "sphere_private.h"

bool Sphere::geodesic (double x1, double y1, double z1, double x2, double y2, double z2, unsigned int N, double* x, double* y, double* z) {
    return arc(x1, y1, z1, x2, y2, z2, M_PI_2, true, false, N, x, y, z);
}

bool Sphere::geodesic (double x1, double y1, double z1, double x2, double y2, double z2, unsigned int N, double* coord) {
    return arc(x1, y1, z1, x2, y2, z2, M_PI_2, true, false, N, coord);
}

