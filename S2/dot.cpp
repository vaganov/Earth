#include <math.h>
#include "sphere_private.h"

double Sphere::dot (double x1, double y1, double z1, double x2, double y2, double z2) {
    return x1 * x2 + y1 * y2 + z1 * z2;
}

