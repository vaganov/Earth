#include "fractal_aux.h"

unsigned int Sphere::Fractal::against (Triangle T, unsigned int edge) {
    if (edge == T.A) {
        return T.BC;
    }
    if (edge == T.B) {
        return T.CA;
    }
    if (edge == T.C) {
        return T.AB;
    }
}

void Sphere::Fractal::against (Triangle& T, unsigned int edge, unsigned int nxt) {
    if (edge == T.A) {
        T.BC = nxt;
    }
    if (edge == T.B) {
        T.CA = nxt;
    }
    if (edge == T.C) {
        T.AB = nxt;
    }
}

unsigned int Sphere::Fractal::third (Triangle T, unsigned int edge1, unsigned int edge2) {
    if (edge1 == T.A) {
        if (edge2 == T.B) {
            return T.C;
        }
        if (edge2 == T.C) {
            return T.B;
        }
    }
    if (edge1 == T.B) {
        if (edge2 == T.C) {
            return T.A;
        }
        if (edge2 == T.A) {
            return T.C;
        }
    }
    if (edge1 == T.C) {
        if (edge2 == T.A) {
            return T.B;
        }
        if (edge2 == T.B) {
            return T.A;
        }
    }
}

void Sphere::Fractal::neigh (Triangle& T, unsigned int old_neigh, unsigned int new_neigh) {
    if (T.AB == old_neigh) {
        T.AB = new_neigh;
    }
    if (T.BC == old_neigh) {
        T.BC = new_neigh;
    }
    if (T.CA == old_neigh) {
        T.CA = new_neigh;
    }
}

