#ifndef _SPHERE_PRIVATE_H
#define _SPHERE_PRIVATE_H

#include <sphere.h>

namespace Sphere {

void rot_M (double, double, double, double, double, double[3][3]); // матрица вращения вокруг заданной точки на угол с заданными косинусом и синусом
double dot (double, double, double, double, double, double); // скалярное произведение
void cross (double, double, double, double, double, double, double&, double&, double&); // векторное произведение
bool normalize (double&, double&, double&); // нормализация вектора (деление на модуль)
bool between (double, double, double, double, double, double, double, double, double, double, double, double); // определение пересечения двух (сферических) отрезков
bool between (double, double, double, double, double, double, double, double, double); // определение пересечения (сферического) отрезка и плоскости
double rho (double, double, double, double, double, double, double, bool, bool); // расстояние между точками по дуге заданного радиуса
bool triangle (double, double, double, double, double, double, double, double, double, unsigned int, Triangle*); // триангуляция (сферического) треугольника; количество треугольников разбиения равно квадрату количества отрезков разбиения стороны
bool inside_polygon (double, double, const double*, const double*, unsigned int, bool&); // определяет, лежит ли точка внутри многоугольника (заданы географические координаты вершин)
bool inside_polygon (double, double, double, const double*, const double*, const double*, unsigned int, bool&); // определяет, лежит ли точка внутри многоугольника (заданы декартовы координаты вершин)
bool inside_polygon (double, double, double, const double*, const double*, const double*, const double*, const double*, const double*, unsigned int); // определяет, лежит ли точка внутри многоугольника (заданы декартовы координаты вершин и нормали к сторонам)
void* polygon_rec (void*); // рекурсивный алгоритм триангуляции
bool fractal (const double*, const double*, const double*, unsigned int, Triangle*); // "фрактальный" (линейный) алгоритм триангуляции

extern double P0[3][3]; // матрица поворота сферы в момент начала вращения
extern double x0_rot, y0_rot, z0_rot; // начальная точка вращения
extern const double eps; // точность вычислений

}

#endif

