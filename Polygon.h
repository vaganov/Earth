#ifndef _POLYGON_H
#define _POLYGON_H

#include <sphere.h>

class Polygon {
    static const unsigned int sizeBlock = 1024;
    unsigned int size;
    unsigned int N;
    unsigned int M;
    double* x;
    double* y;
    double* z;
    bool loop;
    double phi0;
    double psi0;
    double phiCur;
    double psiCur;
    Sphere::Triangle* T;
public:
    Polygon (void);
    ~Polygon (void);
    void start (double phi, double psi); // задать начальную точку с географическими координатами (phi, psi)
    bool geodesic (double phi, double psi); // добавить точку с географическими координатами (phi, psi), соединив ее геодезической с предыдущей
    bool geodesic (void); // замкнуть контур, соединив последнюю точку геодезической с первой
    bool loxodrome (double phi, double psi); // добавить точку с географическими координатами (phi, psi), соединив ее локсодромой с предыдущей
    bool loxodrome (void); // замкнуть контур, соединив последнюю точку локсодромой с первой
    bool arc (double phi, double psi, double r, bool right, bool clockwise); // добавить точку с географическими координатами (phi, psi), соединив ее дугой радиуса r с предыдущей; концы дуги и центр составляют правую или левую тройку в зависимости от right; дуга идет по или против часовой стрелки в зависимости от clockwise
    bool arc (double r, bool right, bool clockwise); // замкнуть контур, соединив последнюю точку дугой радиуса r с первой; концы дуги и центр составляют правую или левую тройку в зависимости от right; дуга идет по или против часовой стрелки в зависимости от clockwise
    void drawContour (void); // нарисовать границу
    void drawFill (void); // нарисовать заливку
private:
    bool resize (void); // увеличить размер на sizeBlock
    bool fill (void); // рассчитать триангуляцию
};

#endif

