#include <math.h>
#include "sphere_private.h"

bool Sphere::route_aux0 (double x1, double y1, double z1, double x2, double y2, double z2, double r, double& xa, double& ya, double& za, double& xb, double& yb, double& zb) {
    double xn, yn, zn;
    double c, s;
    double xt1, yt1, zt1;
    double xt2, yt2, zt2;
    cross(x1, y1, z1, x2, y2, z2, xn, yn, zn);
    if (!normalize(xn, yn, zn)) {
        return false;
    }
    c = cos(r);
    s = sin(r);
    xt1 = c * x1;
    yt1 = c * y1;
    zt1 = c * z1;
    xt2 = s * xn;
    yt2 = s * yn;
    zt2 = s * zn;
    xa = xt1 + xt2;
    ya = yt1 + yt2;
    za = zt1 + zt2;
    xb = xt1 - xt2;
    yb = yt1 - yt2;
    zb = zt1 - zt2;
    return true;
}

bool Sphere::route_aux1 (double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3, double r, double& xa, double& ya, double& za, double& xb, double& yb, double& zb) {
    double xn1, yn1, zn1;
    double xn2, yn2, zn2;
    double xbix, ybix, zbix;
    double cosa;
    double c, s;
    double xt1, yt1, zt1;
    double xt2, yt2, zt2;
    cross(x1, y1, z1, x2, y2, z2, xn1, yn1, zn1);
    if (!normalize(xn1, yn1, zn1)) {
        return false;
    }
    cross(x2, y2, z2, x3, y3, z3, xn2, yn2, zn2);
    if (!normalize(xn2, yn2, zn2)) {
        return false;
    }
    cosa = dot(xn1, yn1, zn1, xn2, yn2, zn2);
    if (cosa < 1) {
        r /= sqrt((1 - cosa) / 2);
    }
    xbix = xn1 + xn2;
    ybix = yn1 + yn2;
    zbix = zn1 + zn2;
    if (!normalize(xbix, ybix, zbix)) {
        return false;
    }
    c = cos(r);
    s = sin(r);
    xt1 = c * x2;
    yt1 = c * y2;
    zt1 = c * z2;
    xt2 = s * xbix;
    yt2 = s * ybix;
    zt2 = s * zbix;
    xa = xt1 + xt2;
    ya = yt1 + yt2;
    za = zt1 + zt2;
    xb = xt1 - xt2;
    yb = yt1 - yt2;
    zb = zt1 - zt2;
    return true;
}

