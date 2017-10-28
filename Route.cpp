#include <math.h>
#include <GL/gl.h>
#include <sphere.h>
#include "Route.h"

#include "debug.h"

double Route::alpha = M_PI / 180;
unsigned int Route::N_cap = 64;

Route::Route (double width) {
    w = width / 2;
    size = 0;
    N = 0;
}

Route::~Route (void) {
    unsigned int i;
    if (N) {
        delete[] x;
        delete[] y;
        delete[] z;
        for (i = 0; i < N - 1; ++i) {
            delete[] xWl[i];
            delete[] yWl[i];
            delete[] zWl[i];
            delete[] xWr[i];
            delete[] yWr[i];
            delete[] zWr[i];
        }
        delete[] xWl;
        delete[] yWl;
        delete[] zWl;
        delete[] xWr;
        delete[] yWr;
        delete[] zWr;
        delete[] Wlen;
        for (i = 0; i < N; ++i) {
            delete[] xC[i];
            delete[] yC[i];
            delete[] zC[i];
        }
        delete[] xC;
        delete[] yC;
        delete[] zC;
    }
}

bool Route::addPoint (double phi, double psi) {
    unsigned int i;
    double X, Y, Z;
    unsigned int len;
    double *xG, *yG, *zG;
    double *xwl, *ywl, *zwl, *xwr, *ywr, *zwr;
    double *pxwl, *pywl, *pzwl, *pxwr, *pywr, *pzwr;
    double *xc, *yc, *zc;
    Sphere::XYZ(phi, psi, X, Y, Z);
    if (N) {
        len = (unsigned int) ceil(Sphere::rho(xCur, yCur, zCur, X, Y, Z) / alpha);
        while (len & (len - 1)) {
            len++;
        }
        xG = new double[len];
        yG = new double[len];
        zG = new double[len];
        if (!xG || !yG || !zG) {
            if (xG) {
                delete[] xG;
            }
            if (yG) {
                delete[] yG;
            }
            if (zG) {
                delete[] zG;
            }
            return false;
        }
        if (!Sphere::geodesic(xCur, yCur, zCur, X, Y, Z, len, xG, yG, zG)) {
            return false;
        }
        xwl = new double[len + 1];
        ywl = new double[len + 1];
        zwl = new double[len + 1];
        xwr = new double[len + 1];
        ywr = new double[len + 1];
        zwr = new double[len + 1];
        if (!xwl || !ywl || !zwl || !xwr || !ywr || !zwr) {
            if (xwl) {
                delete[] xwl;
            }
            if (ywl) {
                delete[] ywl;
            }
            if (zwl) {
                delete[] zwl;
            }
            if (xwr) {
                delete[] xwr;
            }
            if (ywr) {
                delete[] ywr;
            }
            if (zwr) {
                delete[] zwr;
            }
            delete[] xG;
            delete[] yG;
            delete[] zG;
            return false;
        }
        pxwl = xwl;
        pywl = ywl;
        pzwl = zwl;
        pxwr = xwr;
        pywr = ywr;
        pzwr = zwr;
        if (len > 1) {
            Sphere::route_aux0(xCur, yCur, zCur, xG[1], yG[1], zG[1], w, *pxwl++, *pywl++, *pzwl++, *pxwr++, *pywr++, *pzwr++);
            for (i = 1; i < len - 1; i++) {
                Sphere::route_aux0(xG[i], yG[i], zG[i], xG[i + 1], yG[i + 1], zG[i + 1], w, *pxwl++, *pywl++, *pzwl++, *pxwr++, *pywr++, *pzwr++);
            }
            Sphere::route_aux0(xG[len - 1], yG[len - 1], zG[len - 1], X, Y, Z, w, *pxwl++, *pywl++, *pzwl++, *pxwr++, *pywr++, *pzwr++);
            Sphere::route_aux0(X, Y, Z, xG[len - 1], yG[len - 1], zG[len - 1], w, *pxwr++, *pywr++, *pzwr++, *pxwl++, *pywl++, *pzwl++);
        }
        else {
            Sphere::route_aux0(xCur, yCur, zCur, X, Y, Z, w, *pxwl++, *pywl++, *pzwl++, *pxwr++, *pywr++, *pzwr++);
            Sphere::route_aux0(X, Y, Z, xCur, yCur, zCur, w, *pxwr++, *pywr++, *pzwr++, *pxwl++, *pywl++, *pzwl++);
        }
        if (N == size) {
            if (!resize(true)) {
                return false;
            }
        }
        xWl[N - 1] = xwl;
        yWl[N - 1] = ywl;
        zWl[N - 1] = zwl;
        xWr[N - 1] = xwr;
        yWr[N - 1] = ywr;
        zWr[N - 1] = zwr;
        Wlen[N - 1] = len + 1;
    }
    else {
        if (!(resize(false))) {
            return false;
        }
    }
    xc = new double[N_cap];
    yc = new double[N_cap];
    zc = new double[N_cap];
    if (!xc || !yc || !zc) {
        if (xc) {
            delete[] xc;
        }
        if (yc) {
            delete[] yc;
        }
        if (zc) {
            delete[] zc;
        }
        return false;
    }
    if (!Sphere::circle(X, Y, Z, w, N_cap, xc, yc, zc)) {
        delete[] xc;
        delete[] yc;
        delete[] zc;
        return false;
    }
    xC[N] = xc;
    yC[N] = yc;
    zC[N] = zc;
    x[N] = xCur = X;
    y[N] = yCur = Y;
    z[N] = zCur = Z;
    ++N;
    return true;
}

void Route::draw (void) {
    unsigned int i, j;
    unsigned int len;
    if (N > 1) {
        for (i = 0; i < N - 1; ++i) {
            len = Wlen[i];
            glBegin(GL_QUAD_STRIP);
            for (j = 0; j < len; ++j) {
                glVertex3f(xWl[i][j], yWl[i][j], zWl[i][j]);
                glVertex3f(xWr[i][j], yWr[i][j], zWr[i][j]);
            }            
            glEnd();
        }
    }
    if (N) {
        for (i = 0; i < N; ++i) {
            glBegin(GL_TRIANGLE_FAN);
            glVertex3f(x[i], y[i], z[i]);
            for (j = 0; j < N_cap; ++j) {
                glVertex3f(xC[i][j], yC[i][j], zC[i][j]);
            }
            glVertex3f(xC[i][0], yC[i][0], zC[i][0]);
            glEnd();
        }
    }
}

bool Route::resize (bool gge) {
    unsigned int i;
    unsigned int newSize = size + sizeBlock;
    double* newx = new double[newSize];
    double* newy = new double[newSize];
    double* newz = new double[newSize];
    double** newxWl = new double*[newSize];
    double** newyWl = new double*[newSize];
    double** newzWl = new double*[newSize];
    double** newxWr = new double*[newSize];
    double** newyWr = new double*[newSize];
    double** newzWr = new double*[newSize];
    unsigned int* newWlen = new unsigned int[newSize];
    double** newxC = new double*[newSize];
    double** newyC = new double*[newSize];
    double** newzC = new double*[newSize];
    if (!newx || !newy || !newz || !newxWl || !newyWl || !newzWl || !newxWr || !newyWr || !newzWr || !newWlen || !newxC || !newyC || !newzC) {
        if (newx) {
            delete[] newx;
        }
        if (newy) {
            delete[] newy;
        }
        if (newz) {
            delete[] newz;
        }
        if (newxWl) {
            delete[] newxWl;
        }
        if (newyWl) {
            delete[] newyWl;
        }
        if (newzWl) {
            delete[] newzWl;
        }
        if (newxWr) {
            delete[] newxWr;
        }
        if (newyWr) {
            delete[] newyWr;
        }
        if (newzWr) {
            delete[] newzWr;
        }
        if (newWlen) {
            delete[] newWlen;
        }
        if (newxC) {
            delete[] newxC;
        }
        if (newyC) {
            delete[] newyC;
        }
        if (newzC) {
            delete[] newzC;
        }
        return false;
    }
    if (gge) {
        for (i = 0; i < N; ++i) {
            newx[i] = x[i];
            newy[i] = y[i];
            newz[i] = z[i];
            newWlen[i] = Wlen[i];
            newxC[i] = xC[i];
            newyC[i] = yC[i];
            newzC[i] = zC[i];
        }
        for (i = 0; i < N - 1; ++i) {
            newxWl[i] = xWl[i];
            newyWl[i] = yWl[i];
            newzWl[i] = zWl[i];
            newxWr[i] = xWr[i];
            newyWr[i] = yWr[i];
            newzWr[i] = zWr[i];
        }
        delete[] x;
        delete[] y;
        delete[] z;
        delete[] xWl;
        delete[] yWl;
        delete[] zWl;
        delete[] xWr;
        delete[] yWr;
        delete[] zWr;
        delete[] Wlen;
        delete[] xC;
        delete[] yC;
        delete[] zC;
    }
    x = newx;
    y = newy;
    z = newz;
    xWl = newxWl;
    yWl = newyWl;
    zWl = newzWl;
    xWr = newxWr;
    yWr = newyWr;
    zWr = newzWr;
    Wlen = newWlen;
    xC = newxC;
    yC = newyC;
    zC = newzC;
    return true;
}

