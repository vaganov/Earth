#include <math.h>
#include "sphere_private.h"

#include "debug.h"

bool Sphere::arc (double x1, double y1, double z1, double x2, double y2, double z2, double r, bool right, bool clockwise, unsigned int N, double* x, double* y, double* z) {
    double c;
    double X1, XP, X2, Y1, YP, Y2, Z1, ZP, Z2;
    double r0;
    double rx, ry, rz;
    double xn, yn, zn;
    double xc, yc, zc;
    double x0, y0, z0;
    double Nn, Nc;
    double t;
    unsigned int s, S, idx;
    if (N & (N - 1)) { // binary degrees only
        return false;
    }
    cross(x1, y1, z1, x2, y2, z2, xn, yn, zn);
    Nn = xn * xn + yn * yn + zn * zn;
    if (Nn < eps) {
        return false;
    }
    if ((r <= 0) || (r > M_PI_2)) {
        return false;
    }
    if (r == M_PI_2) { // geodesic
        s = N >> 1;
        if (s) {
            x[s] = x1 + x2;
            y[s] = y1 + y2;
            z[s] = z1 + z2;
            normalize(x[s], y[s], z[s]);
            if (right ^ !clockwise) {
                x[s] = -x[s];
                y[s] = -y[s];
                z[s] = -z[s];
            }
        }
        while (s >>= 1) {
            S = 2 * s;
            x[s] = x1 + x[S];
            y[s] = y1 + y[S];
            z[s] = z1 + z[S];
            normalize(x[s], y[s], z[s]);
            for (idx = 3 * s; idx < N - s; idx += S) {
                x[idx] = x[idx - s] + x[idx + s];
                y[idx] = y[idx - s] + y[idx + s];
                z[idx] = z[idx - s] + z[idx + s];
                normalize(x[idx], y[idx], z[idx]);
            }
            x[N - s] = x[N - S] + x2;
            y[N - s] = y[N - S] + y2;
            z[N - s] = z[N - S] + z2;
            normalize(x[N - s], y[N - s], z[N - s]);
        }
    }
    else { // arc
        c = cos(r);
        X1 = x1 * x1;
        XP = x1 * x2;
        X2 = x2 * x2;
        Y1 = y1 * y1;
        YP = y1 * y2;
        Y2 = y2 * y2;
        Z1 = z1 * z1;
        ZP = z1 * z2;
        Z2 = z2 * z2;
        xc = c * (x1 * (Y2 + Z2) + x2 * (Y1 + Z1) - (x1 + x2) * (YP + ZP));
        yc = c * (y1 * (Z2 + X2) + y2 * (Z1 + X1) - (y1 + y2) * (ZP + XP));
        zc = c * (z1 * (X2 + Y2) + z2 * (X1 + Y1) - (z1 + z2) * (XP + YP));
        xc /= Nn;
        yc /= Nn;
        zc /= Nn;
#ifdef _DEBUG
        cout << "c = (" << xc << ", " << yc << ", " << zc << ")\n";
#endif
        Nc = xc * xc + yc * yc + zc * zc;
#ifdef _DEBUG
        cout << "Nc = " << Nc << "\n";
#endif
        if (Nc > 1) {
            return false;
        }
        t = sqrt((1 - Nc) / Nn);
#ifdef _DEBUG
        cout << "t = " << t << "\n";
#endif
        if (right) {
            x0 = xc + t * xn;
            y0 = yc + t * yn;
            z0 = zc + t * zn;
        }
        else {
            x0 = xc - t * xn;
            y0 = yc - t * yn;
            z0 = zc - t * zn;
        }
#ifdef _DEBUG
        cout << "O = (" << x0 << ", " << y0 << ", " << z0 << ")\n";
#endif
        x0 *= c;
        y0 *= c;
        z0 *= c;
        r0 = sin(r);
        s = N >> 1;
        if (s) {
            cross(x0, y0, z0, x1 - x2, y1 - y2, z1 - z2, rx, ry, rz);
            normalize(rx, ry, rz);
            if (clockwise) {
                x[s] = x0 - r0 * rx;
                y[s] = y0 - r0 * ry;
                z[s] = z0 - r0 * rz;
            }
            else {
                x[s] = x0 + r0 * rx;
                y[s] = y0 + r0 * ry;
                z[s] = z0 + r0 * rz;
            }
        }
        while (s >>= 1) {
            S = 2 * s;
            cross(x0, y0, z0, x1 - x[S], y1 - y[S], z1 - z[S], rx, ry, rz);
            normalize(rx, ry, rz);
            if (clockwise) {
                x[s] = x0 - r0 * rx;
                y[s] = y0 - r0 * ry;
                z[s] = z0 - r0 * rz;
            }
            else {
                x[s] = x0 + r0 * rx;
                y[s] = y0 + r0 * ry;
                z[s] = z0 + r0 * rz;
            }
            for (idx = 3 * s; idx < N - s; idx += S) {
                cross(x0, y0, z0, x[idx - s] - x[idx + s], y[idx - s] - y[idx + s], z[idx - s] - z[idx + s], rx, ry, rz);
                normalize(rx, ry, rz);
                if (clockwise) {
                    x[idx] = x0 - r0 * rx;
                    y[idx] = y0 - r0 * ry;
                    z[idx] = z0 - r0 * rz;
                }
                else {
                    x[idx] = x0 + r0 * rx;
                    y[idx] = y0 + r0 * ry;
                    z[idx] = z0 + r0 * rz;
                }
            }
            cross(x0, y0, z0, x[N - S] - x2, y[N - S] - y2, z[N - S] - z2, rx, ry, rz);
            normalize(rx, ry, rz);
            if (clockwise) {
                x[N - s] = x0 - r0 * rx;
                y[N - s] = y0 - r0 * ry;
                z[N - s] = z0 - r0 * rz;
            }
            else {
                x[N - s] = x0 + r0 * rx;
                y[N - s] = y0 + r0 * ry;
                z[N - s] = z0 + r0 * rz;
            }
        }
    }
    return true;
}

bool Sphere::arc (double x1, double y1, double z1, double x2, double y2, double z2, double r, bool right, bool clockwise, unsigned int N, double* coord) {
    double c;
    double X1, XP, X2, Y1, YP, Y2, Z1, ZP, Z2;
    double r0;
    double rx, ry, rz;
    double xn, yn, zn;
    double xc, yc, zc;
    double x0, y0, z0;
    double Nn, Nc;
    double t;
    unsigned int s, S, idx;
    unsigned int s3, S3, idx3;
    unsigned int N3;
    double* x = coord;
    double* y = coord + 1;
    double* z = coord + 2;
    if (N & (N - 1)) { // binary degrees only
        return false;
    }
    cross(x1, y1, z1, x2, y2, z2, xn, yn, zn);
    Nn = xn * xn + yn * yn + zn * zn;
    if (Nn < eps) {
        return false;
    }
    if ((r <= 0) || (r > M_PI_2)) {
        return false;
    }
    N3 = 3 * N;
    if (r == M_PI_2) { // geodesic
        s = N >> 1;
        if (s) {
            s3 = 3 * s;
            x[s3] = x1 + x2;
            y[s3] = y1 + y2;
            z[s3] = z1 + z2;
            normalize(x[s3], y[s3], z[s3]);
            if (right ^ !clockwise) {
                x[s3] = -x[s3];
                y[s3] = -y[s3];
                z[s3] = -z[s3];
            }
        }
        while (s >>= 1) {
            S = 2 * s;
            s3 = 3 * s;
            S3 = 3 * S;
            x[s3] = x1 + x[S3];
            y[s3] = y1 + y[S3];
            z[s3] = z1 + z[S3];
            normalize(x[s3], y[s3], z[s3]);
            for (idx = 3 * s; idx < N - s; idx += S) {
                idx3 = 3 * idx;
                x[idx3] = x[idx3 - s3] + x[idx3 + s3];
                y[idx3] = y[idx3 - s3] + y[idx3 + s3];
                z[idx3] = z[idx3 - s3] + z[idx3 + s3];
                normalize(x[idx3], y[idx3], z[idx3]);
            }
            x[N3 - s3] = x[N3 - S3] + x2;
            y[N3 - s3] = y[N3 - S3] + y2;
            z[N3 - s3] = z[N3 - S3] + z2;
            normalize(x[N3 - s3], y[N3 - s3], z[N3 - s3]);
        }
    }
    else { // arc
        c = cos(r);
        X1 = x1 * x1;
        XP = x1 * x2;
        X2 = x2 * x2;
        Y1 = y1 * y1;
        YP = y1 * y2;
        Y2 = y2 * y2;
        Z1 = z1 * z1;
        ZP = z1 * z2;
        Z2 = z2 * z2;
        xc = c * (x1 * (Y2 + Z2) + x2 * (Y1 + Z1) - (x1 + x2) * (YP + ZP));
        yc = c * (y1 * (Z2 + X2) + y2 * (Z1 + X1) - (y1 + y2) * (ZP + XP));
        zc = c * (z1 * (X2 + Y2) + z2 * (X1 + Y1) - (z1 + z2) * (XP + YP));
        xc /= Nn;
        yc /= Nn;
        zc /= Nn;
#ifdef _DEBUG
        cout << "c = (" << xc << ", " << yc << ", " << zc << ")\n";
#endif
        Nc = xc * xc + yc * yc + zc * zc;
#ifdef _DEBUG
        cout << "Nc = " << Nc << "\n";
#endif
        if (Nc > 1) {
            return false;
        }
        t = sqrt((1 - Nc) / Nn);
#ifdef _DEBUG
        cout << "t = " << t << "\n";
#endif
        if (right) {
            x0 = xc + t * xn;
            y0 = yc + t * yn;
            z0 = zc + t * zn;
        }
        else {
            x0 = xc - t * xn;
            y0 = yc - t * yn;
            z0 = zc - t * zn;
        }
#ifdef _DEBUG
        cout << "O = (" << x0 << ", " << y0 << ", " << z0 << ")\n";
#endif
        x0 *= c;
        y0 *= c;
        z0 *= c;
        r0 = sin(r);
        s = N >> 1;
        if (s) {
            cross(x0, y0, z0, x1 - x2, y1 - y2, z1 - z2, rx, ry, rz);
            normalize(rx, ry, rz);
            s3 = 3 * s;
            if (clockwise) {
                x[s3] = x0 - r0 * rx;
                y[s3] = y0 - r0 * ry;
                z[s3] = z0 - r0 * rz;
            }
            else {
                x[s3] = x0 + r0 * rx;
                y[s3] = y0 + r0 * ry;
                z[s3] = z0 + r0 * rz;
            }
        }
        while (s >>= 1) {
            S = 2 * s;
            S3 = 3 * S;
            cross(x0, y0, z0, x1 - x[S3], y1 - y[S3], z1 - z[S3], rx, ry, rz);
            normalize(rx, ry, rz);
            s3 = 3 * s;
            if (clockwise) {
                x[s3] = x0 - r0 * rx;
                y[s3] = y0 - r0 * ry;
                z[s3] = z0 - r0 * rz;
            }
            else {
                x[s3] = x0 + r0 * rx;
                y[s3] = y0 + r0 * ry;
                z[s3] = z0 + r0 * rz;
            }
            for (idx = 3 * s; idx < N - s; idx += S) {
                idx3 = 3 * idx;
                cross(x0, y0, z0, x[idx3 - s3] - x[idx3 + s3], y[idx3 - s3] - y[idx3 + s3], z[idx3 - s3] - z[idx3 + s3], rx, ry, rz);
                normalize(rx, ry, rz);
                if (clockwise) {
                    x[idx3] = x0 - r0 * rx;
                    y[idx3] = y0 - r0 * ry;
                    z[idx3] = z0 - r0 * rz;
                }
                else {
                    x[idx3] = x0 + r0 * rx;
                    y[idx3] = y0 + r0 * ry;
                    z[idx3] = z0 + r0 * rz;
                }
            }
            cross(x0, y0, z0, x[N3 - S3] - x2, y[N3 - S3] - y2, z[N3 - S3] - z2, rx, ry, rz);
            normalize(rx, ry, rz);
            if (clockwise) {
                x[N3 - s3] = x0 - r0 * rx;
                y[N3 - s3] = y0 - r0 * ry;
                z[N3 - s3] = z0 - r0 * rz;
            }
            else {
                x[N3 - s3] = x0 + r0 * rx;
                y[N3 - s3] = y0 + r0 * ry;
                z[N3 - s3] = z0 + r0 * rz;
            }
        }
    }
    return true;
}

