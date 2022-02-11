#ifndef __APPLE__  // OSがMacでない (= WindowsやLinux)
#include <GL/glut.h>  // Windows, Linuxの場合のヘッダ
#else
#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>  // Macの場合のヘッダ
#endif

#include <stdio.h>
#include <math.h>

#include "draw_object.h"

int count_wing = 0;
int wing_status = 1;
char *count_circle_num = "0123456789";
char *get = "GET!";

void draw_ball(int x_0, int y_0, float r){ //楕円
    int x,y;
    double rate;

    glColor3d(1.0, 0.0, 0.0);
    glBegin(GL_POLYGON); 
    for (int i = 0; i < PART; i++) { // 座標を計算 
        rate = (double)i / PART; 
        x = r * cos(2.0 * M_PI * rate)+x_0; 
        y = r * sin(2.0 * M_PI * rate)+y_0; 
        glVertex3f(x, y, 0.0); 
    }
    glEnd();

    glColor3d(0.0, 0.0, 0.0);
    glBegin(GL_POLYGON); 
    for (int i = 0; i < PART; i++) { // 座標を計算 
        rate = (double)i / PART; 
        x = 0.4 *r * cos(2.0 * M_PI * rate)+x_0; 
        y = 0.4 *r * sin(2.0 * M_PI * rate)+y_0; 
        glVertex3f(x, y, 0.0); 
    }
    glEnd();

    glRectd(x_0-r, y_0-2, x_0+r, y_0+2);

    glColor3d(1.0, 1.0, 1.0);
    glBegin(GL_POLYGON); 
    for (int i = 0; i < PART; i++) { // 座標を計算 
        rate = (double)i / PART; 
        x = 0.2 *r * cos(2.0 * M_PI * rate)+x_0; 
        y = 0.2 *r * sin(2.0 * M_PI * rate)+y_0; 
        glVertex3f(x, y, 0.0); 
    }
    glEnd();
}

void draw_locus(int status, int n_points, matrix *points){
    color_line(status);
    glLineWidth(5.0);
    glBegin(GL_LINE_STRIP);
    for (int i=0; i<n_points; i++){
        glVertex2d(mat_elem(*points, i, 0), mat_elem(*points, i, 1));
    }
    glEnd();
    glLineWidth(1.0);

    mat_free(points);
}

void draw_number(int *status,int *enemy_status, int *ray_status, int count_circle, int ENEMY_X, int ENEMY_Y){
    if (count_circle<6){
        glColor3f(1.0, 0.0, 0.0); 
        glRasterPos2f(ENEMY_X-4, ENEMY_Y+20); 
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, count_circle_num[count_circle]); 
    }else{
        glColor3f(1.0, 0.0, 0.0); 
        glRasterPos2f(ENEMY_X-20, ENEMY_Y+20); 
        for (int i=0; i<strlen(get); i++){
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, get[i]);
        }
        *status = -5;
        *enemy_status = -5;
        *ray_status = -5;
    }
}

void draw_cursor(int status, int PREV_X, int PREV_Y, int window_height){
    color_cursor(status);
    glBegin(GL_TRIANGLES);
    glVertex2d(PREV_X, window_height - PREV_Y - 1-10);
    glVertex2d(PREV_X+10, window_height - PREV_Y - 1+10);
    glVertex2d(PREV_X-10, window_height - PREV_Y - 1+10);
    glEnd();
}

void draw_ray(int status, int ray_status, int *RAY_X_list, int *RAY_Y_list){
    if (ray_status>0){
        color_ray(status);
        glPointSize(10.0);
        glBegin(GL_POINTS);
        for (int i=0; i<4; i++){
            glVertex2d(RAY_X_list[i], RAY_Y_list[i]);
        }        
        glEnd();
        glPointSize(1.0);
    }
}

void draw_enemy(int status, int ENEMY_X, int ENEMY_Y){
    // 身体
    color_enemy(status);
    glRectd(ENEMY_X-10, ENEMY_Y-15, ENEMY_X+10, ENEMY_Y+15);

    // 翼
    glBegin(GL_TRIANGLES);

    if (count_wing > 10) {
        wing_status = -1;
    }
    if (count_wing < -15){
        wing_status = 1;
    }
    count_wing += wing_status;

    glBegin(GL_TRIANGLES);
    glVertex2d(ENEMY_X+10, ENEMY_Y+11);
    glVertex2d(ENEMY_X+10+30*cos((double)count_wing/30), ENEMY_Y+11+30*sin((double)count_wing/30));
    glVertex2d(ENEMY_X+10+20*cos((double)count_wing/30+M_PI/6), ENEMY_Y+11+20*sin((double)count_wing/30+M_PI/6));
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex2d(ENEMY_X-10, ENEMY_Y+11);
    glVertex2d(ENEMY_X-10-30*cos(-(double)count_wing/30), ENEMY_Y+11-30*sin(-(double)count_wing/30));
    glVertex2d(ENEMY_X-10-20*cos(-(double)count_wing/30-M_PI/6), ENEMY_Y+11-20*sin(-(double)count_wing/30-M_PI/6));
    glEnd();


    // 目
    glColor3d(1.0, 1.0, 1.0);
    glRectd(ENEMY_X-8, ENEMY_Y+14, ENEMY_X-2, ENEMY_Y+10);
    glRectd(ENEMY_X+8, ENEMY_Y+14, ENEMY_X+2, ENEMY_Y+10);

    glColor3d(0.0, 0.0, 0.2);
    glRectd(ENEMY_X-6, ENEMY_Y+14, ENEMY_X-4, ENEMY_Y+11);
    glRectd(ENEMY_X+6, ENEMY_Y+14, ENEMY_X+4, ENEMY_Y+11);


    // 口
    glRectd(ENEMY_X-8, ENEMY_Y+7, ENEMY_X+8, ENEMY_Y-13);

    // 歯
    glColor3d(1.0, 1.0, 1.0);
    glBegin(GL_TRIANGLES);
    glVertex2d(ENEMY_X+8, ENEMY_Y-12);
    glVertex2d(ENEMY_X+4, ENEMY_Y-12);
    glVertex2d(ENEMY_X+6, ENEMY_Y-5);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex2d(ENEMY_X-8, ENEMY_Y-12);
    glVertex2d(ENEMY_X-4, ENEMY_Y-12);
    glVertex2d(ENEMY_X-6, ENEMY_Y-5);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex2d(ENEMY_X+8, ENEMY_Y+7);
    glVertex2d(ENEMY_X+4, ENEMY_Y+7);
    glVertex2d(ENEMY_X+6, ENEMY_Y);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex2d(ENEMY_X-8, ENEMY_Y+7);
    glVertex2d(ENEMY_X-4, ENEMY_Y+7);
    glVertex2d(ENEMY_X-6, ENEMY_Y);
    glEnd();
}