#include <stdio.h>
#include <math.h>
#include "solve_equation.h"

void apply_enemy_status(int *enemy_status, int *count_status, int *ray_status, int *ENEMY_X, int *ENEMY_Y, int *RAY_X, int *RAY_Y, int *RAY_X_list, int *RAY_Y_list);
void apply_ray_status(int *enemy_status, int *count_status, int *ray_status, int *ENEMY_X, int *ENEMY_Y, int *RAY_X, int *RAY_Y, int *RAY_X_list, int *RAY_Y_list, double c1, double c2, double g, double h);