#include <math.h>
#include "sphere_private.h"

bool Sphere::loxodrome (double phi1, double psi1, double phi2, double psi2, unsigned int N, double* x, double* y, double* z, int winds) {
    unsigned int i, _i;
    psi2 += winds * 2 * M_PI;
    for (i = 1; i < N; i++) {
        _i = N - i;
        XYZ((_i * phi1 + i * phi2) / N, (_i * psi1 + i * psi2) / N, x[i], y[i], z[i]);
    }
    return true;
}

