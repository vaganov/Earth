#ifndef _FRACTAL_AUX_H
#define _FRACTAL_AUX_H

namespace Sphere {

namespace Fractal {

typedef struct {
    unsigned int A;
    unsigned int B;
    unsigned int C;
    unsigned int BC;
    unsigned int CA;
    unsigned int AB;
} Triangle;

unsigned int against (Triangle, unsigned int);
void against (Triangle&, unsigned int, unsigned int);
unsigned int third (Triangle, unsigned int, unsigned int);
void neigh (Triangle&, unsigned int, unsigned int);

}

}

#endif

