#include "sphere_private.h"

bool Sphere::between (double x1, double y1, double z1, double x2, double y2, double z2, double xA, double yA, double zA, double xB, double yB, double zB) {
    double nx, ny, nz;
    cross(xA, yA, zA, xB, yB, zB, nx, ny, nz);
    return between(x1, y1, z1, x2, y2, z2, nx, ny, nz);
}

bool Sphere::between (double x1, double y1, double z1, double x2, double y2, double z2, double nx, double ny, double nz) {
    return dot(x1, y1, z1, nx, ny, nz) * dot(x2, y2, z2, nx, ny, nz) < 0;
}

