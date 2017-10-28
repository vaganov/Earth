#ifndef _ROUTE_H
#define _ROUTE_H

class Route {
    static const unsigned int sizeBlock = 1024;
    unsigned int size;
    static double alpha;
    static unsigned int N_cap;
    double w;
    unsigned int N;
    double xCur, yCur, zCur;
    double *x, *y, *z;
    double **xWl, **yWl, **zWl, **xWr, **yWr, **zWr;
    unsigned int *Wlen;
    double **xC, **yC, **zC;
public:
    Route (double width); // создать трассу шириной width (в радианах)
    ~Route (void);
    bool addPoint (double phi, double psi); // добавить точку с координатами (phi, psi)
    void draw (void); // нарисовать трассу
private:
    bool resize (bool);
};

#endif

