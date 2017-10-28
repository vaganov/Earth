#include "sphere_private.h"
#include "fractal_aux.h"

bool Sphere::fractal (const double* x, const double* y, const double* z, unsigned int N, Triangle* p) {
    typedef struct {
        unsigned int T;
        unsigned int e1;
        unsigned int e2;
        unsigned int aux;
        bool left;
    } Stack_record;
    unsigned int i;
    unsigned int cur_vx, nxt_vx, aux_vx, new_vx;
    unsigned int e1_vx, e2_vx;
    double X, Y, Z;
    unsigned int delta;
    bool yet;
    bool outl, outr;
    bool out;
    unsigned int lastl, lastr;
    if (N < 3) {
        return false;
    }
    Fractal::Triangle* tri = new Fractal::Triangle[N - 1];
    Fractal::Triangle T;
    unsigned int cur_tri, nxt_tri = 1, neigh_tri;
    unsigned int* edge = new unsigned int[N];
    Stack_record* stack = new Stack_record[N - 2];
    Stack_record record;
    unsigned int SP = 0;
    Triangle t;
    if (!tri || !edge || !stack) {
        if (tri) {
            delete[] tri;
        }
        if (edge) {
            delete[] edge;
        }
        if (stack) {
            delete[] stack;
        }
        return false;
    }
    edge[0] = nxt_tri;
    edge[N / 3] = nxt_tri;
    edge[2 * N / 3] = nxt_tri;
    T.A = 0;
    T.B = N / 3;
    T.C = 2 * N / 3;
    T.BC = 0;
    T.CA = 0;
    T.AB = 0;
    tri[nxt_tri++] = T;
    yet = N > 3;
    cur_vx = 0;
    while (yet) {
        yet = false;
        do {
            cur_tri = edge[cur_vx];
            T = tri[cur_tri];
            if (cur_vx == T.A) {
                nxt_vx = T.B;
                aux_vx = T.C;
            }
            if (cur_vx == T.B) {
                nxt_vx = T.C;
                aux_vx = T.A;
            }
            if (cur_vx == T.C) {
                nxt_vx = T.A;
                aux_vx = T.B;
            }
            if (nxt_vx) {
                delta = nxt_vx - cur_vx;
            }
            else {
                delta = N - cur_vx;
            }
            if (delta > 1) {
                if (nxt_vx) {
                    new_vx = (cur_vx + nxt_vx) / 2;
                }
                else {
                    new_vx = (cur_vx + N ) / 2;
                }
                X = x[new_vx];
                Y = y[new_vx];
                Z = z[new_vx];
                if (between(X, Y, Z, x[aux_vx], y[aux_vx], z[aux_vx], x[cur_vx], y[cur_vx], z[cur_vx], x[nxt_vx], y[nxt_vx], z[nxt_vx])) {
                    edge[cur_vx] = nxt_tri;
                    edge[new_vx] = nxt_tri;
                    Fractal::against(tri[cur_tri], aux_vx, nxt_tri);
                    T.A = cur_vx;
                    T.B = new_vx;
                    T.C = nxt_vx;
                    T.BC = 0;
                    T.CA = cur_tri;
                    T.AB = 0;
                    tri[nxt_tri++] = T;
                }
                else {
                    out = true;
                    e1_vx = cur_vx;
                    e2_vx = nxt_vx;
                    do {
                        outl = between(X, Y, Z, x[e2_vx], y[e2_vx], z[e2_vx], x[e1_vx], y[e1_vx], z[e1_vx], x[aux_vx], y[aux_vx], z[aux_vx]);
                        outr = between(X, Y, Z, x[e1_vx], y[e1_vx], z[e1_vx], x[e2_vx], y[e2_vx], z[e2_vx], x[aux_vx], y[aux_vx], z[aux_vx]);
                        if (outl && outr) {
                            delete[] tri;
                            delete[] edge;
                            delete[] stack;
                            return false;
                        }
                        if (outl || outr) {
                            record.T = cur_tri;
                            record.e1 = e1_vx;
                            record.e2 = e2_vx;
                            record.aux = aux_vx;
                            record.left = outl;
                            stack[SP++] = record;
                            if (outl) {
                                cur_tri = Fractal::against(T, e2_vx);
                            }
                            if (outr) {
                                cur_tri = Fractal::against(T, e1_vx);
                            }
                            if (cur_tri) {
                                T = tri[cur_tri];
                                if (outl) {
                                    e2_vx = aux_vx;
                                    aux_vx = Fractal::third(T, e1_vx, aux_vx);
                                }
                                if (outr) {
                                    e1_vx = aux_vx;
                                    aux_vx = Fractal::third(T, e2_vx, aux_vx);
                                }
                            }
                            else {
                                delete[] tri;
                                delete[] edge;
                                delete[] stack;
                                return false;
                            }
                        }
                        else {
                            out = false;
                        }
                    } while (out);
                    neigh_tri = Fractal::against(tri[cur_tri], e2_vx);
                    if (neigh_tri) {
                        Fractal::neigh(tri[neigh_tri], cur_tri, nxt_tri);
                    }
                    if (edge[aux_vx] == cur_tri) {
                        edge[aux_vx] = nxt_tri;
                    }
                    T.A = new_vx;
                    T.B = aux_vx;
                    T.C = e1_vx;
                    T.BC = neigh_tri;
                    T.AB = cur_tri;
                    lastl = nxt_tri;
                    tri[nxt_tri] = T;
                    neigh_tri = Fractal::against(tri[cur_tri], e1_vx);
                    T.B = e2_vx;
                    T.C = aux_vx;
                    T.BC = neigh_tri;
                    T.CA = nxt_tri;
                    lastr = cur_tri;
                    tri[cur_tri] = T;
                    nxt_tri++;
                    while (SP) {
                        record = stack[--SP];
                        cur_tri = record.T;
                        aux_vx = record.aux;
                        if (record.left) {
                            neigh_tri = Fractal::against(tri[cur_tri], record.e1);
                            T.B = record.e2;
                            T.C = aux_vx;
                            T.BC = neigh_tri;
                            T.CA = lastr;
                            tri[lastr].AB = cur_tri;
                            lastr = cur_tri;
                        }
                        else {
                            neigh_tri = Fractal::against(tri[cur_tri], record.e2);
                            T.B = aux_vx;
                            T.C = record.e1;
                            T.BC = neigh_tri;
                            T.AB = lastl;
                            tri[lastl].CA = cur_tri;
                            lastl = cur_tri;
                        }
                        tri[cur_tri] = T;
                    }
                    tri[lastl].CA = 0;
                    tri[lastr].AB = 0;
                    edge[cur_vx] = lastl;
                    edge[new_vx] = lastr;
                    if (edge[nxt_vx] == cur_tri) {
                        edge[nxt_vx] = lastr;
                    }
                    if (edge[aux_vx] == cur_tri) {
                        edge[aux_vx] = lastl;
                    }
                }
                if (delta > 2) {
                    yet = true;
                }
            }
            cur_vx = nxt_vx;
        } while (cur_vx);
    }
    for (i = 1; i < nxt_tri; i++) {
        T = tri[i];
        t.vx1.x = x[T.A];
        t.vx1.y = y[T.A];
        t.vx1.z = z[T.A];
        t.vx2.x = x[T.B];
        t.vx2.y = y[T.B];
        t.vx2.z = z[T.B];
        t.vx3.x = x[T.C];
        t.vx3.y = y[T.C];
        t.vx3.z = z[T.C];
        *p++ = t;
    }
    delete[] tri;
    delete[] edge;
    delete[] stack;
    return true;
}

