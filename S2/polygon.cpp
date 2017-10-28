#define WITH_STL 0

#include <stdlib.h>
#include <pthread.h>
#if WITH_STL
#include <utility>
#include <hash_set>
#include <set>
#endif
#include <math.h>
#include "sphere_private.h"

#include "debug.h"

using namespace std;

namespace Sphere {

typedef struct {
    const double* x;
    const double* y;
    const double* z;
    unsigned int N;
    Triangle* T;
    bool threads;
    bool res;
} polygon_arg;

}

bool Sphere::polygon (const double* phi, const double* psi, unsigned int N, Triangle* T, bool threads) {
    unsigned int i;
    double* x = new double[N];
    double* y = new double[N];
    double* z = new double[N];
    bool P;
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
    P = polygon(x, y, z, N, T, threads);
    delete[] x;
    delete[] y;
    delete[] z;
    return P;
}

bool Sphere::polygon (const double* x, const double* y, const double* z, unsigned int N, Triangle* T, bool threads) {
    polygon_arg arg;
    arg.x = x;
    arg.y = y;
    arg.z = z;
    arg.N = N;
    arg.T = T;
    arg.threads = threads;
    polygon_rec(&arg);
    return arg.res;
}

void* Sphere::polygon_rec (void* p) {
    polygon_arg* arg = (polygon_arg*) p;
    const double* x = arg->x;
    const double* y = arg->y;
    const double* z = arg->z;
    unsigned int N = arg->N;
    Triangle* T = arg->T;
    bool threads = arg->threads;
    polygon_arg argL, argR;
    pthread_t pid;
    unsigned int A, B, X, i;
    double *xL, *yL, *zL, *xR, *yR, *zR;
    const double *px, *py, *pz;
    double *pxL, *pyL, *pzL, *pxR, *pyR, *pzR;
    double *nx, *ny, *nz;
    double nx0, ny0, nz0;
    double dot0, dot_prv, dot_nxt, dotA, dotB;
    double nx_0, ny_0, nz_0, nx_prv, ny_prv, nz_prv, nx_nxt, ny_nxt, nz_nxt;
    bool intersects;
    unsigned int NL, NR;
#if WITH_STL
    pair<unsigned int, unsigned int> diag;
    set<pair<unsigned int, unsigned int> > diags;
    const unsigned int num_diags = N * (N - 3) / 2;
#endif
    if (fractal(x, y, z, N, T)) {
#ifdef _DEBUG
        cout << "polygon_rec(): fractal() @ N = " << N << " succeeded\n";
#endif
        arg->res = true;
        return 0;
    }
    nx = new double[N];
    ny = new double[N];
    nz = new double[N];
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
        arg->res = false;
        return 0;
    }
    for (i = 0; i < N - 1; i++) {
        cross(x[i], y[i], z[i], x[i + 1], y[i + 1], z[i + 1], nx[i], ny[i], nz[i]);
    }
    cross(x[N - 1], y[N - 1], z[N - 1], x[0], y[0], z[0], nx[N - 1], ny[N - 1], nz[N - 1]);
    while (true) {
        A = lrand48() % N;
        B = lrand48() % N;
        if (A > B) {
            X = A;
            A = B;
            B = X;
        }
        if (B - A < 2) {
            continue;
        }
        if ((A == 0) && (B == N - 1)) {
            continue;
        }
#if WITH_STL
        diag = pair<unsigned int, unsigned int>(A, B);
        if (diags.find(diag) == diags.end()) {
            diags.insert(diag);
        }
        else {
            if (diags.size() == num_diags) {
#ifdef _DEBUG
                cout << "polygon_rec(): looks like we've tried all " << N << " * " << N - 3 << " / 2 = " << num_diags << " diagonals, returning false\n";
#endif
                delete[] nx;
                delete[] ny;
                delete[] nz;
                arg->res = false;
                return 0;
            }
            continue;
        }
#endif
#ifdef _DEBUG
        cout << "polygon_rec(): trying A = " << A << ", B = " << B << "\n";
#endif
        intersects = false;
        cross(x[A], y[A], z[A], x[B], y[B], z[B], nx0, ny0, nz0);
        dot0 = dot_prv = dot(x[0], y[0], z[0], nx0, ny0, nz0);
        if (A > 1) {
            for (i = 0; i < A - 1; i++) {
                dot_nxt = dot(x[i + 1], y[i + 1], z[i + 1], nx0, ny0, nz0);
                dotA = dot(x[A], y[A], z[A], nx[i], ny[i], nz[i]);
                dotB = dot(x[B], y[B], z[B], nx[i], ny[i], nz[i]);
                if ((dotA * dotB <= 0) && (dot_prv * dot_nxt <= 0)) {
                    intersects = true;
                    break;
                }
                dot_prv = dot_nxt;
            }
            if (intersects) {
                continue;
            }
        }
        dot_prv = dot(x[A + 1], y[A + 1], z[A + 1], nx0, ny0, nz0);
        for (i = A + 1; i < B - 1; i++) {
            dot_nxt = dot(x[i + 1], y[i + 1], z[i + 1], nx0, ny0, nz0);
            dotA = dot(x[A], y[A], z[A], nx[i], ny[i], nz[i]);
            dotB = dot(x[B], y[B], z[B], nx[i], ny[i], nz[i]);
            if ((dotA * dotB <= 0) && (dot_prv * dot_nxt <= 0)) {
                intersects = true;
                break;
            }
            dot_prv = dot_nxt;
        }
        if (intersects) {
            continue;
        }
        dot_prv = dot(x[B + 1], y[B + 1], z[B + 1], nx0, ny0, nz0);
        for (i = B + 1; i < N - 1; i++) {
            dot_nxt = dot(x[i + 1], y[i + 1], z[i + 1], nx0, ny0, nz0);
            dotA = dot(x[A], y[A], z[A], nx[i], ny[i], nz[i]);
            dotB = dot(x[B], y[B], z[B], nx[i], ny[i], nz[i]);
            if ((dotA * dotB <= 0) && (dot_prv * dot_nxt <= 0)) {
                intersects = true;
                break;
            }
            dot_prv = dot_nxt;
        }
        if (intersects) {
            continue;
        }
        if ((A > 0) && (B < N - 1)) {
            if (B == N - 2) {
                dot_prv = dot(x[N - 1], y[N - 1], z[N - 1], nx0, ny0, nz0);
            }
            dotA = dot(x[A], y[A], z[A], nx[N - 1], ny[N - 1], nz[N - 1]);
            dotB = dot(x[B], y[B], z[B], nx[N - 1], ny[N - 1], nz[N - 1]);
            if ((dotA * dotB <= 0) && (dot_prv * dot0 <= 0)) {
                continue;
            }
        }
        if (inside_polygon(x[A] + x[B], y[A] + y[B], z[A] + z[B], x, y, z, nx, ny, nz, N)) {
#ifdef _DEBUG
            cout << "polygon_rec(): bingo!\n";
#endif
            break;
        }
    }
    delete[] nx;
    delete[] ny;
    delete[] nz;
    NL = N + A - B + 1;
    NR = B - A + 1;
    xL = new double[NL];
    yL = new double[NL];
    zL = new double[NL];
    xR = new double[NR];
    yR = new double[NR];
    zR = new double[NR];
    if (!xL || !yL || !zL || !xR || !yR || !zR) {
        if (xL) {
            delete[] xL;
        }
        if (yL) {
            delete[] yL;
        }
        if (zL) {
            delete[] zL;
        }
        if (xR) {
            delete[] xR;
        }
        if (yR) {
            delete[] yR;
        }
        if (zR) {
            delete[] zR;
        }
        arg->res = false;
        return 0;
    }
    px = x;
    py = y;
    pz = z;
    pxL = xL;
    pyL = yL;
    pzL = zL;
    pxR = xR;
    pyR = yR;
    pzR = zR;
    for (i = 0; i < A; i++) {
        *pxL++ = *px++;
        *pyL++ = *py++;
        *pzL++ = *pz++;
    }
    *pxL++ = *px;
    *pyL++ = *py;
    *pzL++ = *pz;
    for (i = A; i < B; i++) {
        *pxR++ = *px++;
        *pyR++ = *py++;
        *pzR++ = *pz++;
    }
    *pxR++ = *px;
    *pyR++ = *py;
    *pzR++ = *pz;
    for (i = B; i < N; i++) {
        *pxL++ = *px++;
        *pyL++ = *py++;
        *pzL++ = *pz++;
    }
    argL.x = xL;
    argL.y = yL;
    argL.z = zL;
    argL.N = NL;
    argL.T = T;
    argL.threads = threads;
    argR.x = xR;
    argR.y = yR;
    argR.z = zR;
    argR.N = NR;
    argR.T = &T[NL - 2];
    argR.threads = threads;
    if (threads) {
        if (pthread_create(&pid, 0, polygon_rec, &argR)) {
#ifdef _DEBUG
            cout << "polygon_rec(): pthread_create() failed\n";
#endif
            delete[] xL;
            delete[] yL;
            delete[] zL;
            delete[] xR;
            delete[] yR;
            delete[] zR;
            arg->res = false;
            return 0;
        }
        polygon_rec(&argL);
#ifdef _DEBUG
        cout << "polygon_rec(): waiting for thread " << pid << " to finish\n";
#endif
        if (pthread_join(pid, 0)) {
#ifdef _DEBUG
            cout << "polygon_rec(): pthread_join() failed\n";
#endif
            delete[] xL;
            delete[] yL;
            delete[] zL;
            delete[] xR;
            delete[] yR;
            delete[] zR;
            arg->res = false;
            return 0;
        }
#ifdef _DEBUG
        cout << "polygon_rec(): thread " << pid << " finished\n";
#endif
    }
    else {
        polygon_rec(&argL);
        polygon_rec(&argR);
    }
    delete[] xL;
    delete[] yL;
    delete[] zL;
    delete[] xR;
    delete[] yR;
    delete[] zR;
    arg->res = argL.res && argR.res;
    return 0;
}

