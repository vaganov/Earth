#include <math.h>
#include "sphere_private.h"

bool Sphere::inside_polygon (double phi0, double psi0, const double* phi, const double* psi, unsigned int N, bool& I) {
    unsigned int i;
    bool ret;
    double x0, y0, z0;
    double* x = new double[N];
    double* y = new double[N];
    double* z = new double[N];
    if (!x || !y || !z) {
        if (x) {
            delete[] x;
        }
        if (y) {
            delete[] y;
        }
        if (z) {
            delete[] z;
        }
        return false;
    }
    for (i = 0; i < N; i++) {
        XYZ(phi[i], psi[i], x[i], y[i], z[i]);
    }
    XYZ(phi0, psi0, x0, y0, z0);
    ret = inside_polygon(x0, y0, z0, x, y, z, N, I);
    delete[] x;
    delete[] y;
    delete[] z;
    return ret;
}

bool Sphere::inside_polygon (double x0, double y0, double z0, const double* x, const double* y, const double* z, unsigned int N, bool& I) {
    unsigned int i;
    double* nx = new double[N];
    double* ny = new double[N];
    double* nz = new double[N];
    if (!nx || !ny || !nz) {
        if (nx) {
            delete[] nx;
        }
        if (ny) {
            delete[] ny;
        }
        if (nz) {
            delete[] nz;
        }
        return false;
    }
    for (i = 0; i < N - 1; i++) {
        cross(x[i], y[i], z[i], x[i + 1], y[i + 1], z[i + 1], nx[i], ny[i], nz[i]);
    }
    cross(x[N - 1], y[N - 1], z[N - 1], x[0], y[0], z[0], nx[N - 1], ny[N - 1], nz[N - 1]);
    I = inside_polygon (x0, y0, z0, x, y, z, nx, ny, nz, N);
    delete[] nx;
    delete[] ny;
    delete[] nz;
    return true;
}

bool Sphere::inside_polygon (double x0, double y0, double z0, const double* x, const double* y, const double* z, const double* nx, const double* ny, const double* nz, unsigned int N) {
    unsigned int i;
    double S = 0;
    double alpha;
    double nx0, ny0, nz0, nx_prv, ny_prv, nz_prv, nx_nxt, ny_nxt, nz_nxt;
    cross(x[0], y[0], z[0], x0, y0, z0, nx_prv, ny_prv, nz_prv);
    normalize(nx_prv, ny_prv, nz_prv);
    nx0 = nx_prv;
    ny0 = ny_prv;
    nz0 = nz_prv;
    for (i = 1; i < N; i++) {
        cross(x[i], y[i], z[i], x0, y0, z0, nx_nxt, ny_nxt, nz_nxt);
        normalize(nx_nxt, ny_nxt, nz_nxt);
        alpha = acos(dot(nx_prv, ny_prv, nz_prv, nx_nxt, ny_nxt, nz_nxt)); // !
        nx_prv = nx_nxt;
        ny_prv = ny_nxt;
        nz_prv = nz_nxt;
        if (dot(nx[i - 1], ny[i - 1], nz[i - 1], x0, y0, z0) > 0) {
            S += alpha;
        }
        else {
            S -= alpha;
        }
    }
    alpha = acos(dot(nx_prv, ny_prv, nz_prv, nx0, ny0, nz0)); // !
    if (dot(nx[N - 1], ny[N - 1], nz[N - 1], x0, y0, z0) > 0) {
        S += alpha;
    }
    else {
        S -= alpha;
    }
    return (S < -M_PI) || (S > M_PI);
}

