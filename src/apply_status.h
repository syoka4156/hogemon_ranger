#include <stdio.h>
#include <math.h>
#include "solve_equation.h"
#include "matrix.h"

matrix y_0, y_1, y_2;

void apply_enemy_status(int *enemy_status, int *count_status, int *ray_status, int *ENEMY_X, int *ENEMY_Y, int *RAY_X, int *RAY_Y, int *RAY_X_list, int *RAY_Y_list);
void move_enemy_right(int *enemy_status, int *count_status, int *ENEMY_X);
void move_enemy_left(int *enemy_status, int *count_status, int *ENEMY_X);
void move_enemy_up(int *enemy_status, int *count_status, int *ENEMY_Y);
void move_enemy_down(int *enemy_status, int *count_status, int *ENEMY_Y);
void make_enemy_attack(int *enemy_status, int *ray_status, int *RAY_X, int *RAY_Y, int *RAY_X_list, int *RAY_Y_list);

void apply_ray_status(int *enemy_status, int *count_status, int *ray_status, int *ENEMY_X, int *ENEMY_Y, int *RAY_X, int *RAY_Y, int *RAY_X_list, int *RAY_Y_list, double c1, double c2, double g, double h);
void no_attack(int *enemy_status, int *count_status, int *ray_status);
void straight_attack(int *enemy_status, int *count_status, int *ray_status, int *ENEMY_X, int *ENEMY_Y, int *RAY_X, int *RAY_Y, int *RAY_X_list, int *RAY_Y_list);
void circle_attack(int *enemy_status, int *count_status, int *ray_status, int *ENEMY_X, int *ENEMY_Y, int *RAY_X, int *RAY_Y, int *RAY_X_list, int *RAY_Y_list);
void spreading_circle_attack(int *enemy_status, int *count_status, int *ray_status, int *ENEMY_X, int *ENEMY_Y, int *RAY_X, int *RAY_Y, int *RAY_X_list, int *RAY_Y_list);
void vibration_attack(int *enemy_status, int *count_status, int *ray_status, int *ENEMY_X, int *ENEMY_Y, int *RAY_X, int *RAY_Y, int *RAY_X_list, int *RAY_Y_list, double c1, double c2, double g, double h);
void vibration_circle_attack(int *enemy_status, int *count_status, int *ray_status, int *ENEMY_X, int *ENEMY_Y, int *RAY_X, int *RAY_Y, int *RAY_X_list, int *RAY_Y_list, double c1, double c2, double g, double h);

void init_ray(int *RAY_X, int *RAY_Y, int *RAY_X_list, int *RAY_Y_list);