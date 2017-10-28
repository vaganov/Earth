#ifndef _SPHERE_H
#define _SPHERE_H

namespace Sphere {

typedef struct {
    double x;
    double y;
    double z;
} R3; // точка трехмерного пространства

typedef struct {
    R3 vx1;
    R3 vx2;
    R3 vx3;
} Triangle; // треугольник в трехмерном пространстве

void XYZ (double phi, double psi, double& x, double& y, double& z); // перевести географические координаты (phi, psi) в декартовы координаты (x, y, z)
void S2 (double x, double y, double z, double& phi, double& psi); // перевести декартовы координаты (x, y, z) в географические координаты (phi, psi)
bool coord (double x, double y, double& X, double& Y, double& Z); // перевести экранные координаты (x, y) в декартовы координаты (X, Y, Z)

void start_rot (double x, double y); // начать вращение сферы за точку с экранными координатами (x, y)
void rot (double x, double y); // вращать сферу, переводя начальную точку в точку с экранными координатами (x, y)
void north (void); // повернуть сферу так, чтобы меридиан, проходящий через центр экрана, располагался вертикально

void center (double, double); // расположить точку с географическими координатами (phi, psi) по центру экрана так, чтобы меридиан, проходящий через центр экрана, располагался вертикально

bool arc (double x1, double y1, double z1, double x2, double y2, double z2, double r, bool right, bool clockwise, unsigned int N, double* x, double* y, double* z); // сохранить координаты N (степень двойки) промежуточных точек дуги радиуса r между точками (x1, y1, z1) и (x2, y2, z2) в массивы x[], y[] и z[]
bool arc (double x1, double y1, double z1, double x2, double y2, double z2, double r, bool right, bool clockwise, unsigned int N, double* coord); // сохранить координаты N (степень двойки) промежуточных точек дуги радиуса r между точками (x1, y1, z1) и (x2, y2, z2) в массив coord[]
bool geodesic (double x1, double y1, double z1, double x2, double y2, double z2, unsigned int N, double* x, double* y, double* z); // сохранить координаты N (степень двойки) промежуточных точек дуги большого круга между точками (x1, y1, z1) и (x2, y2, z2) в массивы x[], y[] и z[]
bool geodesic (double x1, double y1, double z1, double x2, double y2, double z2, unsigned int N, double* coord); // сохранить координаты N (степень двойки) промежуточных точек дуги большого круга между точками (x1, y1, z1) и (x2, y2, z2) в массивы coord[]
bool loxodrome (double phi1, double psi1, double phi2, double psi2, unsigned int N, double* x, double* y, double* z, int winds = 0); // сохранить координаты N промежуточных точек локсодромы между точками (phi1, psi1) и (phi2, psi2) в массивы x[], y[] и z[]
bool circle (double x0, double y0, double z0, double r, unsigned int N, double* x, double* y, double* z); // сохранить координаты N (степень двойки) промежуточных точек окружности радиуса r с центром (x0, y0, z0) в массивы x[], y[] и z[]

bool route_aux0 (double x1, double y1, double z1, double x2, double y2, double z2, double r, double& xa, double& ya, double& za, double& xb, double& yb, double& zb); // вспомогательная функция для рисования трасс
bool route_aux1 (double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3, double r, double& xa, double& ya, double& za, double& xb, double& yb, double& zb); // вспомогательная функция для рисования трасс

double rho (double x1, double y1, double z1, double x2, double y2, double z2); // расстояние (в радианах) между точками (x1, y1, z1) и (x2, y2, z2) по дуге большого круга

bool polygon (const double* phi, const double* psi, unsigned int N, Triangle* T, bool threads = false); // сохранить триангуляцию N-угольника с координатами вершин (phi[0], psi[0]) ... (phi[N - 1], psi[N - 1]) в массив T[], распараллеливая рекурсивные вызовы или нет в зависимости от threads
bool polygon (const double* x, const double* y, const double* z, unsigned int N, Triangle* T, bool threads = false); // сохранить триангуляцию N-угольника с координатами вершин (x[0], y[0], z[0]) ... (x[N - 1], y[N - 1], z[N - 1]) в массив T[], распараллеливая рекурсивные вызовы или нет в зависимости от threads

extern bool north_auto; // автоматический доворот севера

extern double P[]; // матрица поворота (4x4, развертка по столбцам)
extern double X0, Y0, Z0; // декартовы координаты точки касания сферы с плоскостью экрана при текущем положении

}

#endif

