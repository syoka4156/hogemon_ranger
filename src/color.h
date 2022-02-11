#ifndef __APPLE__  
#include <GL/glut.h>  
#else
#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h> 
#endif

#include <stdio.h>

void color_line(int status);
void color_cursor(int status);
void color_enemy(int status);
void color_ray(int status);