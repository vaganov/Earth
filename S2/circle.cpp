#include <math.h>
#include "sphere_private.h"

bool Sphere::circle (double x0, double y0, double z0, double r, unsigned int N, double* x, double* y, double* z) {
    double xn, yn, zn;
    double rx, ry, rz;
    double c;
    double r0;
    unsigned int s, S, idx;
    if (N & (N - 1)) { // binary degrees only
        return false;
    }
    if ((r <= 0) || (r > M_PI_2)) {
        return false;
    }
    if ((fabs(x0) < eps) && (fabs(y0) < eps)) {
        xn = z0;
        yn = 0;
        zn = -x0;
    }
    else {
        xn = y0;
        yn = -x0;
        zn = 0;
    }
    cross(x0, y0, z0, xn, yn, zn, rx, ry, rz);
    normalize(xn, yn, zn);
    normalize(rx, ry, rz);
    c = cos(r);
    x0 *= c;
    y0 *= c;
    z0 *= c;
    r0 = sin(r);
    x[0] = x0 - r0 * xn;
    y[0] = y0 - r0 * yn;
    z[0] = z0 - r0 * zn;
    s = N >> 1;
    if (s) {
        x[s] = x0 + r0 * xn;
        y[s] = y0 + r0 * yn;
        z[s] = z0 + r0 * zn;
    }
    s >>= 1;
    if (s) {
        x[s] = x0 - r0 * rx;
        y[s] = y0 - r0 * ry;
        z[s] = z0 - r0 * rz;
        x[3 * s] = x0 + r0 * rx;
        y[3 * s] = y0 + r0 * ry;
        z[3 * s] = z0 + r0 * rz;
    }
    if (r == M_PI_2) { // equator
        while (s >>= 1) {
            S = 2 * s;
            for (idx = s; idx < N - s; idx += S) {
                x[idx] = x[idx - s] + x[idx + s];
                y[idx] = y[idx - s] + y[idx + s];
                z[idx] = z[idx - s] + z[idx + s];
                normalize(x[idx], y[idx], z[idx]);
            }
            x[N - s] = x[N - S] + x[0];
            y[N - s] = y[N - S] + y[0];
            z[N - s] = z[N - S] + z[0];
            normalize(x[N - s], y[N - s], z[N - s]);
        }
    }
    else { // circle
        while (s >>= 1) {
            S = 2 * s;
            for (idx = s; idx < N - s; idx += S) {
                cross(x0, y0, z0, x[idx - s] - x[idx + s], y[idx - s] - y[idx + s], z[idx - s] - z[idx + s], rx, ry, rz);
                normalize(rx, ry, rz);
                x[idx] = x0 + r0 * rx;
                y[idx] = y0 + r0 * ry;
                z[idx] = z0 + r0 * rz;
            }
            cross(x0, y0, z0, x[N - S] - x[0], y[N - S] - y[0], z[N - S] - z[0], rx, ry, rz);
            normalize(rx, ry, rz);
            x[N - s] = x0 + r0 * rx;
            y[N - s] = y0 + r0 * ry;
            z[N - s] = z0 + r0 * rz;
        }
    }
    return true;
}

