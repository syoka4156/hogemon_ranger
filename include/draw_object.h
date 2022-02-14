#ifndef __APPLE__  // OSがMacでない (= WindowsやLinux)
#include <GL/glut.h>  // Windows, Linuxの場合のヘッダ
#else
#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>  // Macの場合のヘッダ
#endif

#include <stdio.h>
#include <math.h>
#include <string.h>

#include "matrix.h"
#include "color.h"

#define PART 100 // 分割数


void draw_ball(int x_0, int y_0, float r);
void draw_locus(int status, int n_points, matrix *points);
void draw_number(int *status,int *enemy_status, int *ray_status, int count_circle, int ENEMY_X, int ENEMY_Y);
void draw_cursor(int status, int PREV_X, int PREV_Y, int window_height);
void draw_ray(int status, int ray_status, int *RAY_X_list, int *RAY_Y_list);
void draw_enemy(int status, int ENEMY_X, int ENEMY_Y);