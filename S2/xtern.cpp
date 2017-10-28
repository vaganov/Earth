#include <math.h>
#include "sphere_private.h"

namespace Sphere {

bool north_auto = true;
double X0 = 0, Y0 = 0, Z0 = 1;
double P[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
double P0[3][3];
double x0_rot, y0_rot, z0_rot;
const double eps = 1E-12;

}

