#ifndef __APPLE__  
#include <GL/glut.h>  
#else
#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h> 
#endif

#include <stdio.h>
#include "color.h"

void color_line(int status) {
    if (status < 0){
        glColor3d(1.0, 0.0, 0.0);
    }else if (status == 0) {
        glColor3d(0.6 , 0.8 , 1.0);
    }else {
        glColor3d(1.0, 1.0, 0.6);
    }
}

void color_cursor(int status) {
    if (status < 0){
        glColor3d(1.0, 0.0, 0.0);
    }else if (status == 0) {
        glColor3d(0.6, 0.6, 1.0);
    }else {
        glColor3d(1.0, 1.0, 0.6);
    }
}

void color_enemy(int status) {
    if (status < 0){
        glColor3d(1.0, 0.0, 0.0);
    }else if (status == 0) {
        glColor3d(0.4, 0.4, 0.8);
    }else {
        glColor3d(0.0, 0.5, 0.5);
    }
}

void color_ray(int status) {
    if (status < 0){
        glColor3d(1.0, 0.0, 0.0);
    }else if (status == 0) {
        glColor3d(0.0, 0.0, 1.0);
    }else {
        glColor3d(0.0, 0.5, 0.5);
    }
}