#include <stdio.h>
#include "solve_equation.h"

void calculate_differential_equation(double c1, double c2, double g, matrix y, matrix *dydx){
    matrix dydx2;
    mat_alloc(&dydx2, 2, 1);
    mat_elem(dydx2, 0, 0) = mat_elem(y, 1, 0);
    mat_elem(dydx2, 1, 0) = -c1 * mat_elem(y, 0, 0) + g - c2 * mat_elem(y, 1, 0);
    mat_copy(dydx, dydx2); 
    mat_free(&dydx2);
}


void runge_kutta(double c1, double c2, double g, matrix y_0, double h, matrix *y_1, void (*calculate_differential_equation)(double, double, double, matrix, matrix *)){
    matrix k1, k2, k3, k4;

    mat_alloc(&k1, 2, 1);
    mat_alloc(&k2, 2, 1);
    mat_alloc(&k3, 2, 1);
    mat_alloc(&k4, 2, 1);

    calculate_differential_equation(c1, c2, g, y_0, &k1);

    mat_mul_scalar(&k2, k1, 0.5);
    mat_mul_scalar(&k2, k2, h);
    mat_add(&k2, k2, y_0);
    calculate_differential_equation(c1, c2, g, k2, &k2);

    mat_mul_scalar(&k3, k2, 0.5);
    mat_mul_scalar(&k3, k3, h);
    mat_add(&k3, k3, y_0);
    calculate_differential_equation(c1, c2, g, k3, &k3);

    mat_mul_scalar(&k4, k3, h);
    mat_add(&k4, k4, y_0);
    calculate_differential_equation(c1, c2, g, k4, &k4);

    mat_mul_scalar(&k2, k2, 2);
    mat_mul_scalar(&k3, k3, 2);

    mat_add(&k4, k4, k3);
    mat_add(&k4, k4, k2);
    mat_add(&k4, k4, k1);
    mat_mul_scalar(&k4, k4, h/6);
    mat_add(y_1, y_0, k4);

    mat_free(&k1);
    mat_free(&k2);
    mat_free(&k3);
    mat_free(&k4);
}