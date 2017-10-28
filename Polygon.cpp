#include <GL/gl.h>
#include "Polygon.h"

Polygon::Polygon (void) {
    size = sizeBlock;
    x = new double[size];
    y = new double[size];
    z = new double[size];
    T = 0;
}

Polygon::~Polygon (void) {
    delete[] x;
    delete[] y;
    delete[] z;
    if (T) {
        delete[] T;
    }
}

void Polygon::start (double phi, double psi) {
    phi0 = phiCur = phi;
    psi0 = psiCur = psi;
    Sphere::XYZ(phi, psi, x[0], y[0], z[0]);
    N = 1;
    loop = false;
}

bool Polygon::geodesic (double phi, double psi) {
    if (N == size) {
        if (!resize()) {
            return false;
        }
    }
    Sphere::XYZ(phi, psi, x[N], y[N], z[N]);
    N++;
    phiCur = phi;
    psiCur = psi;
    return true;
}

bool Polygon::geodesic (void) {
    loop = true;
    return fill();
}

bool Polygon::loxodrome (double phi, double psi) {
    const unsigned int len = 128;
    unsigned int newN = N + len;
    while (newN >= size) {
        if (!resize()) {
            return false;
        }
    }
    if (!Sphere::loxodrome(phiCur, psiCur, phi, psi, len, &x[N - 1], &y[N - 1], &z[N - 1])) {
        return false;
    }
    N = newN;
    Sphere::XYZ(phi, psi, x[N - 1], y[N - 1], z[N - 1]);
    phiCur = phi;
    psiCur = psi;
    return true;
}

bool Polygon::loxodrome (void) {
    const unsigned int len = 128;
    unsigned int newN = N + len - 1;
    while (newN >= size) {
        if (!resize()) {
            return false;
        }
    }
    if (!Sphere::loxodrome(phiCur, psiCur, phi0, psi0, len, &x[N - 1], &y[N - 1], &z[N - 1])) {
        return false;
    }
    N = newN;
    loop = true;
    return fill();
}

bool Polygon::arc (double phi, double psi, double r, bool right, bool clockwise) {
    const unsigned int len = 128;
    double X, Y, Z;
    unsigned int newN = N + len;
    while (newN >= size) {
        if (!resize()) {
            return false;
        }
    }
    Sphere::XYZ(phi, psi, X, Y, Z);
    if (!Sphere::arc(x[N - 1], y[N - 1], z[N - 1], X, Y, Z, r, right, clockwise, len, &x[N - 1], &y[N - 1], &z[N - 1])) {
        return false;
    }
    N = newN;
    x[N - 1] = X;
    y[N - 1] = Y;
    z[N - 1] = Z;
    phiCur = phi;
    psiCur = psi;
    return true;
}

bool Polygon::arc (double r, bool right, bool clockwise) {
    const unsigned int len = 128;
    unsigned int newN = N + len - 1;
    while (newN >= size) {
        if (!resize()) {
            return false;
        }
    }
    if (!Sphere::arc(x[N - 1], y[N - 1], z[N - 1], x[0], y[0], z[0], r, right, clockwise, len, &x[N - 1], &y[N - 1], &z[N - 1])) {
        return false;
    }
    N = newN;
    loop = true;
    return fill();
}

void Polygon::drawContour (void) {
    unsigned int i;
    if (loop) {
        glBegin(GL_LINE_LOOP);
    }
    else {
        glBegin(GL_LINE_STRIP);
    }
    for (i = 0; i < N; i++) {
        glVertex3f(x[i], y[i], z[i]);
    }
    glEnd();
}

void Polygon::drawFill (void) {
    unsigned int i;
    Sphere::Triangle t;
    Sphere::R3 vx;
    if (T) {
        glBegin(GL_TRIANGLES);
        for (i = 0; i < M; i++) {
            t = T[i];
            vx = t.vx1;
            glVertex3f(vx.x, vx.y, vx.z);
            vx = t.vx2;
            glVertex3f(vx.x, vx.y, vx.z);
            vx = t.vx3;
            glVertex3f(vx.x, vx.y, vx.z);
        }
        glEnd();
    }
}

bool Polygon::resize (void) {
    unsigned int i;
    double *px, *py, *pz;
    unsigned int newSize = size + sizeBlock;
    px = new double[newSize];
    py = new double[newSize];
    pz = new double[newSize];
    if (px && py && pz) {
        size = newSize;
        for (i = 0; i < N; i++) {
            px[i] = x[i];
            py[i] = y[i];
            pz[i] = z[i];
        }
        delete[] x;
        delete[] y;
        delete[] z;
        x = px;
        y = py;
        z = pz;
        return true;
    }
    else {
        if (px) {
            delete[] px;
        }
        if (py) {
            delete[] py;
        }
        if (pz) {
            delete[] pz;
        }
        return false;
    }
}

bool Polygon::fill (void) {
    if (T) {
        delete[] T;
    }
    M = N - 2;
    T = new Sphere::Triangle[M];
    if (T) {
//      return Sphere::polygon(x, y, z, N, T, true);
        return Sphere::polygon(x, y, z, N, T);
    }
    else {
        return false;
    }
}

