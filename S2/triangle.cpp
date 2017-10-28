#include "sphere_private.h"

bool Sphere::triangle (double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3, unsigned int N, Triangle* T) {
    double c;
    unsigned int m, _m;
    unsigned int m1, m2;
    double *ax, *ay, *az, *bx, *by, *bz, *cx, *cy, *cz, *n1x, *n1y, *n1z, *n2x, *n2y, *n2z, *rAx, *rAy, *rAz, *rBx, *rBy, *rBz, *r1x, *r1y, *r1z, *r2x, *r2y, *r2z, *p;
    bool A;
    Triangle t;
    c = dot(x1, y1, z1, x2, y2, z2);
    if (c < 0) {
        return false;
    }
    c = dot(x2, y2, z2, x3, y3, z3);
    if (c < 0) {
        return false;
    }
    c = dot(x3, y3, z3, x1, y1, z1);
    if (c < 0) {
        return false;
    }
    if (N) {
        N++;
        while (N & (N - 1)) {
            N++;
        }
        ax = new double[N];
        ay = new double[N];
        az = new double[N];
        bx = new double[N];
        by = new double[N];
        bz = new double[N];
        cx = new double[N];
        cy = new double[N];
        cz = new double[N];
        n1x = new double[N];
        n1y = new double[N];
        n1z = new double[N];
        n2x = new double[N];
        n2y = new double[N];
        n2z = new double[N];
        rAx = new double[N];
        rAy = new double[N];
        rAz = new double[N];
        rBx = new double[N];
        rBy = new double[N];
        rBz = new double[N];
        if (!ax || !ay || !az || !bx || !by || !bz || !cx || !cy || !cz || !n1x || !n1y || !n1z || !n2x || !n2y || !n2z || !rAx || !rAy || !rAz || !rBx || !rBy || !rBz) {
            if (ax) {
                delete[] ax;
            }
            if (ay) {
                delete[] ay;
            }
            if (az) {
                delete[] az;
            }
            if (bx) {
                delete[] bx;
            }
            if (by) {
                delete[] by;
            }
            if (bz) {
                delete[] bz;
            }
            if (cx) {
                delete[] cx;
            }
            if (cy) {
                delete[] cy;
            }
            if (cz) {
                delete[] cz;
            }
            if (n1x) {
                delete[] n1x;
            }
            if (n1y) {
                delete[] n1y;
            }
            if (n1z) {
                delete[] n1z;
            }
            if (n2x) {
                delete[] n2x;
            }
            if (n2y) {
                delete[] n2y;
            }
            if (n2z) {
                delete[] n2z;
            }
            if (rAx) {
                delete[] rAx;
            }
            if (rAy) {
                delete[] rAy;
            }
            if (rAz) {
                delete[] rAz;
            }
            if (rBx) {
                delete[] rBx;
            }
            if (rBy) {
                delete[] rBy;
            }
            if (rBz) {
                delete[] rBz;
            }
            return false;
        }
        r1x = rAx;
        r1y = rAy;
        r1z = rAz;
        r2x = rBx;
        r2y = rBy;
        r2z = rBz;
        A = geodesic(x1, y1, z1, x2, y2, z2, N, cx, cy, cz) &&
            geodesic(x2, y2, z2, x3, y3, z3, N, ax, ay, az) &&
            geodesic(x3, y3, z3, x1, y1, z1, N, bx, by, bz);
        if (A) {
            for (m = 1; m < N; m++) {
                _m = N - m;
                cross(ax[m], ay[m], az[m], bx[_m], by[_m], bz[_m], n1x[m], n1y[m], n1z[m]);
                cross(cx[m], cy[m], cz[m], ax[_m], ay[_m], az[_m], n2x[m], n2y[m], n2z[m]);
            }
            t.vx2.x = x1;
            t.vx2.y = y1;
            t.vx2.z = z1;
            t.vx3.x = bx[N - 1];
            t.vx3.y = by[N - 1];
            t.vx3.z = bz[N - 1];
            for (m2 = 1; m2 < N - 1; m2++) {
                t.vx1 = t.vx2;
                t.vx2 = t.vx3;
                t.vx3.x = cx[m2];
                t.vx3.y = cy[m2];
                t.vx3.z = cz[m2];
                *T++ = t;
                cross(n1x[1], n1y[1], n1z[1], n2x[m2], n2y[m2], n2z[m2], r1x[m2], r1y[m2], r1z[m2]);
                normalize(r1x[m2], r1y[m2], r1z[m2]);
                if (dot(x1, y1, z1, r1x[m2], r1y[m2], r1z[m2]) < 0) {
                    r1x[m2] = -r1x[m2];
                    r1y[m2] = -r1y[m2];
                    r1z[m2] = -r1z[m2];
                }
                t.vx1 = t.vx2;
                t.vx2 = t.vx3;
                t.vx3.x = r1x[m2];
                t.vx3.y = r1y[m2];
                t.vx3.z = r1z[m2];
                *T++ = t;
            }
            t.vx1 = t.vx2;
            t.vx2 = t.vx3;
            t.vx3.x = cx[N - 1];
            t.vx3.y = cy[N - 1];
            t.vx3.z = cz[N - 1];
            *T++ = t;
            t.vx1 = t.vx2;
            t.vx2 = t.vx3;
            t.vx3.x = ax[1];
            t.vx3.y = ay[1];
            t.vx3.z = az[1];
            *T++ = t;
            t.vx1 = t.vx2;
            t.vx2 = t.vx3;
            t.vx3.x = x2;
            t.vx3.y = y2;
            t.vx3.z = z2;
            *T++ = t;
            for (m1 = 1; m1 < N - 1; m1++) {
                t.vx2.x = bx[N - m1];
                t.vx2.y = by[N - m1];
                t.vx2.z = bz[N - m1];
                t.vx3.x = bx[N - m1 - 1];
                t.vx3.y = by[N - m1 - 1];
                t.vx3.z = bz[N - m1 - 1];
                for (m2 = 1; m2 < N - 1 - m1; m2++) {
                    t.vx1 = t.vx2;
                    t.vx2 = t.vx3;
                    t.vx3.x = r1x[m2];
                    t.vx3.y = r1y[m2];
                    t.vx3.z = r1z[m2];
                    *T++ = t;
                    cross(n1x[m1 + 1], n1y[m1 + 1], n1z[m1 + 1], n2x[m2], n2y[m2], n2z[m2], r2x[m2], r2y[m2], r2z[m2]);
                    normalize(r2x[m2], r2y[m2], r2z[m2]);
                    if (dot(x1, y1, z1, r2x[m2], r2y[m2], r2z[m2]) < 0) {
                        r2x[m2] = -r2x[m2];
                        r2y[m2] = -r2y[m2];
                        r2z[m2] = -r2z[m2];
                    }
                    t.vx1 = t.vx2;
                    t.vx2 = t.vx3;
                    t.vx3.x = r2x[m2];
                    t.vx3.y = r2y[m2];
                    t.vx3.z = r2z[m2];
                    *T++ = t;
                }
                t.vx1 = t.vx2;
                t.vx2 = t.vx3;
                t.vx3.x = r1x[N - 1 - m1];
                t.vx3.y = r1y[N - 1 - m1];
                t.vx3.z = r1z[N - 1 - m1];
                *T++ = t;
                t.vx1 = t.vx2;
                t.vx2 = t.vx3;
                t.vx3.x = ax[m1 + 1];
                t.vx3.y = ay[m1 + 1];
                t.vx3.z = az[m1 + 1];
                *T++ = t;
                t.vx1 = t.vx2;
                t.vx2 = t.vx3;
                t.vx3.x = ax[m1];
                t.vx3.y = ay[m1];
                t.vx3.z = az[m1];
                *T++ = t;
                p = r1x;
                r1x = r2x;
                r2x = p;
                p = r1y;
                r1y = r2y;
                r2y = p;
                p = r1z;
                r1z = r2z;
                r2z = p;
            }
            t.vx1.x = bx[1];
            t.vx1.y = by[1];
            t.vx1.z = bz[1];
            t.vx2.x = x3;
            t.vx2.y = y3;
            t.vx2.z = z3;
            t.vx3.x = ax[N - 1];
            t.vx3.y = ay[N - 1];
            t.vx3.z = az[N - 1];
            *T++ = t;
        }
        delete[] ax;
        delete[] ay;
        delete[] az;
        delete[] bx;
        delete[] by;
        delete[] bz;
        delete[] cx;
        delete[] cy;
        delete[] cz;
        delete[] n1x;
        delete[] n1y;
        delete[] n1z;
        delete[] n2x;
        delete[] n2y;
        delete[] n2z;
        delete[] rAx;
        delete[] rAy;
        delete[] rAz;
        delete[] rBx;
        delete[] rBy;
        delete[] rBz;
        return A;
    }
    else {
        t.vx1.x = x1;
        t.vx1.y = y1;
        t.vx1.z = z1;
        t.vx2.x = x2;
        t.vx2.y = y2;
        t.vx2.z = z2;
        t.vx3.x = x3;
        t.vx3.y = y3;
        t.vx3.z = z3;
        *T++ = t;
        return true;
    }
}

