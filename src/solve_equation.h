#include <stdio.h>
#include "matrix.h"

void calculate_differential_equation(double c1, double c2, double g, matrix y, matrix *dydx);
void runge_kutta(double c1, double c2, double g, matrix y_0, double h, matrix *y_1, void (*calculate_differential_equation)(double, double, double, matrix, matrix *));