#ifndef __APPLE__  // OSがMacでない (= WindowsやLinux)
#include <GL/glut.h>  // Windows, Linuxの場合のヘッダ
#else
#include <GLUT/glut.h>  // Macの場合のヘッダ
#endif

#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "matrix.h"

#define epsilon 1.0e-12
#define PART 100 // 分割数

#define MAX_POINTS 5

int WINDOW_WIDTH = 500;   // ウィンドウの横幅
int WINDOW_HEIGHT = 500;  // ウィンドウの高さ

int n_points;
matrix points;

typedef struct list_node_ {
    int x, y;
    struct list_node_ *next;
} list_node;

typedef struct {
    list_node *root;
    int size;
} linked_list;

linked_list list;


double TIME = 0.0;
const double RECT_SIZE = 300.0;

int PREV_X = -10;
int PREV_Y = -10;
int PRESS_BUTTON = -1;
int COLOR = 0;

int ENEMY_X = 250;
int ENEMY_Y = 250;
int ENEMY_STATUS = 0;

int RAY_X_list[4] = {0, 0, 0, 0};
int RAY_Y_list[4] = {0, 0, 0, 0};
int RAY_X = -10;
int RAY_Y = -10;
int RAY_STATUS = 0;

int STATUS = 0;

int WING_STATUS = 1;

int count = 0;
int count_circle = 0;
int count_status = 0;
int count_wing = 0;

char *count_circle_num = "0123456789";
char *get = "GET!";

double h, k, m, c, g, c1, c2;

matrix y_0, y_1, y_2;

double rate;

// 色を決定
void color_line() {
    if (STATUS < 0){
        glColor3d(1.0, 0.0, 0.0);
    }else if (STATUS == 0) {
        glColor3d(0.6 , 0.8 , 1.0);
    }else {
        glColor3d(1.0, 1.0, 0.6);
    }
}

void color_cursor() {
    if (STATUS < 0){
        glColor3d(1.0, 0.0, 0.0);
    }else if (STATUS == 0) {
        glColor3d(0.6, 0.6, 1.0);
    }else {
        glColor3d(1.0, 1.0, 0.6);
    }
}

void color_enemy() {
    if (STATUS < 0){
        glColor3d(1.0, 0.0, 0.0);
    }else if (STATUS == 0) {
        glColor3d(0.4, 0.4, 0.8);
    }else {
        glColor3d(0.0, 0.5, 0.5);
    }
}

void color_ray() {
    if (STATUS < 0){
        glColor3d(1.0, 0.0, 0.0);
    }else if (STATUS == 0) {
        glColor3d(0.0, 0.0, 1.0);
    }else {
        glColor3d(0.0, 0.5, 0.5);
    }
}

// リストの作成
void init_list(linked_list *list) {
    list->root = NULL;
    list->size = 0;
}

// リストを再設定
void reset_list() {
    PREV_X = -10;
    PREV_Y = -10;

    // 先頭から順にノードをfreeしていく
    list_node *iter = list.root;
    while (iter != NULL) {
        // 先に次のノードのポインタを別の変数にコピーしてからfreeする
        list_node *next = iter->next;
        free(iter);
        iter = next;
    }

    // リストの初期化
    init_list(&list);      
}

// rootを先頭要素とする連結リストの末尾に新しいノードを追加
void add_node(linked_list *list, int x, int y) {
    // 新しいノードの作成
    list_node *node = (list_node*)malloc(sizeof(list_node));
    node->x = x;
    node->y = y;
    node->next = NULL;

    // 末尾要素を探す
    if (list->root == NULL) {
        // まだ要素がなければ，先頭要素を新規ノードにする
        list->root = node;
    } else {
        // あるノードの次のノード (next) がNULLになるものを探す
        list_node *iter = list->root;
        while (iter->next != NULL) {
            iter = iter->next;
        }

        // 見つかったら末尾要素を新規ノードで置き換える
        iter->next = node;
    }

    // リストの要素数を1増やす
    list->size += 1;
}

// リストのデータを全て削除する
void free_list(linked_list *list) {
    // 先頭から順にノードをfreeしていく
    list_node *iter = list->root;
    while (iter != NULL) {
        // 先に次のノードのポインタを別の変数にコピーしてからfreeする
        list_node *next = iter->next;
        free(iter);
        iter = next;
    }

    // 最後にリスト自体をfreeする
    free(list);
}

void f(double c1, double c2, double g, matrix y, matrix *dydx){
    matrix dydx2;
    mat_alloc(&dydx2, 2, 1);
    mat_elem(dydx2, 0, 0) = mat_elem(y, 1, 0);
    mat_elem(dydx2, 1, 0) = -c1 * mat_elem(y, 0, 0) + g - c2 * mat_elem(y, 1, 0);
    mat_copy(dydx, dydx2); 
    mat_free(&dydx2);
}


void runge_kutta(double c1, double c2, double g, matrix y_0, double h, matrix *y_1, void (*f)(double, double, double, matrix, matrix *)){
    matrix k1, k2, k3, k4;

    mat_alloc(&k1, 2, 1);
    mat_alloc(&k2, 2, 1);
    mat_alloc(&k3, 2, 1);
    mat_alloc(&k4, 2, 1);

    f(c1, c2, g, y_0, &k1);

    mat_muls(&k2, k1, 0.5);
    mat_muls(&k2, k2, h);
    mat_add(&k2, k2, y_0);
    f(c1, c2, g, k2, &k2);

    mat_muls(&k3, k2, 0.5);
    mat_muls(&k3, k3, h);
    mat_add(&k3, k3, y_0);
    f(c1, c2, g, k3, &k3);

    mat_muls(&k4, k3, h);
    mat_add(&k4, k4, y_0);
    f(c1, c2, g, k4, &k4);

    mat_muls(&k2, k2, 2);
    mat_muls(&k3, k3, 2);

    mat_add(&k4, k4, k3);
    mat_add(&k4, k4, k2);
    mat_add(&k4, k4, k1);
    mat_muls(&k4, k4, h/6);
    mat_add(y_1, y_0, k4);

    mat_free(&k1);
    mat_free(&k2);
    mat_free(&k3);
    mat_free(&k4);
}

// ボールを描画
void oval(int x_0, int y_0, float r){ //楕円
    int x,y;

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

void display() {

    // double x, y, t;
    // ウィンドウ表示内容のクリア
    glClear(GL_COLOR_BUFFER_BIT);

    n_points =list.size;
    mat_alloc(&points, n_points, 2);

    list_node *iter = list.root;

    int r_ok=0;
    int l_ok=0;
    int u_ok=0;
    int d_ok=0;
    int ok = 0;

    // 接触判定，囲まれていることを判定
    for (int i=0; i<n_points; i++){
        mat_elem(points, i, 0) = (double)iter->x;
        mat_elem(points, i, 1) =  (double)iter->y;
        
        iter = iter->next;

        if (fabs(mat_elem(points, i, 0)-ENEMY_X)<10 && fabs(mat_elem(points, i, 1)-ENEMY_Y)<15) {
            STATUS = -1;
        }

        for (int j=0; j<4; j++){
            if (fabs(mat_elem(points, i, 0)-RAY_X_list[j])<5 && fabs(mat_elem(points, i, 1)-RAY_Y_list[j])<5) {
            STATUS = -1;
            }
        }

        

        if (fabs(mat_elem(points, i, 1)-ENEMY_Y)<20) {
            if (mat_elem(points, i, 0)<ENEMY_X) {
                l_ok += 1;
            }
            else{
                r_ok += 1;
            }
        }

        if (fabs(mat_elem(points, i, 0)-ENEMY_X)<20) {
            if (mat_elem(points, i, 1)<ENEMY_Y) {
                d_ok += 1;
            }
            else{
                u_ok += 1;
            }
        }
        if (i < n_points/2) {

            if (fabs(mat_elem(points, i, 0)-PREV_X)<20 && fabs(mat_elem(points, i, 1)-(WINDOW_HEIGHT - PREV_Y - 1))<10) {
                ok += 1;
            }
        }
    }


    if (l_ok > 0 && r_ok > 0 && u_ok > 0 && d_ok > 0 && ok > 0 && ok > 0) {
        STATUS = 1;
    }

    free(iter);


    // STATUSの処理
    if (STATUS<0 && STATUS>-5) {
        count++;    

        if (count > 30) {
            reset_list();

            count = 0;
            STATUS = 0;

            if (count_circle>0){
                count_circle--;
            }
        }    
    }
    if (STATUS == 1) {
        count++;
        if (count > 5) { 
            reset_list();

            count = 0;
            STATUS = 0;
            count_circle++;
        }
    }
    if (STATUS == -5){
        count++;

        oval(ENEMY_X, ENEMY_Y, 21.0);

        if (count>100){
            STATUS = -6;
        }
    }


    // 線を描画
    color_line();
    glLineWidth(5.0);
    glBegin(GL_LINE_STRIP);
    for (int i=0; i<n_points; i++){
        glVertex2d(mat_elem(points, i, 0), mat_elem(points, i, 1));
    }
    glEnd();
    glLineWidth(1.0);

    mat_free(&points);

    
    // 数字の描画
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
        STATUS = -5;
        ENEMY_STATUS = -5;
        RAY_STATUS = -5;
        
    }
    
    // カーソルの描画
    color_cursor();
    glBegin(GL_TRIANGLES);
    glVertex2d(PREV_X, WINDOW_HEIGHT - PREV_Y - 1-10);
    glVertex2d(PREV_X+10, WINDOW_HEIGHT - PREV_Y - 1+10);
    glVertex2d(PREV_X-10, WINDOW_HEIGHT - PREV_Y - 1+10);
    glEnd();

    // RAYの描画
    if (RAY_STATUS>0){
        color_ray();
        glPointSize(10.0);
        glBegin(GL_POINTS);
        for (int i=0; i<4; i++){
            glVertex2d(RAY_X_list[i], RAY_Y_list[i]);
        }        
        glEnd();
        glPointSize(1.0);
    }

    // ENEMYの描画
    if (STATUS > -5) {
        // 身体
        color_enemy();
        glRectd(ENEMY_X-10, ENEMY_Y-15, ENEMY_X+10, ENEMY_Y+15);

        // 翼
        glBegin(GL_TRIANGLES);

        if (count_wing > 10) {
            WING_STATUS = -1;
        }
        if (count_wing < -15){
            WING_STATUS = 1;
        }
        count_wing += WING_STATUS;

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

    // バッファの入れ替え
    glutSwapBuffers();
}

void reshape(int width, int height) {
    // OpenGLウィンドウの描画範囲を設定
    // 下記は描画範囲が[0, width] x [0, height]となるように設定している
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, (double)width, 0.0, (double)height);
    WINDOW_WIDTH = width;
    WINDOW_HEIGHT = height;
}

void timer(int value) {
    // srand((unsigned int)time(NULL));

    // ENEMYの動作振り分け
    if (ENEMY_STATUS == 0) {
        ENEMY_STATUS = rand() % 5-2;
    }

    if (ENEMY_STATUS == 1) {
        if (ENEMY_X > 450) {
            ENEMY_STATUS = -1;
            count_status = 50;
        }

        if (count_status<150) {
            ENEMY_X += 1;
            count_status++;
        }        
        else{
            ENEMY_STATUS = 10;
            count_status = 0;
        }
    }

    if (ENEMY_STATUS == -1) {
        if (ENEMY_X < 50) {
            ENEMY_STATUS = 1;
            count_status = 50;
        }

        if (count_status<150) {
            ENEMY_X -= 1;
            count_status++;
        }  
        else{
            ENEMY_STATUS = 10;
            count_status = 0;
        }
    }

    if (ENEMY_STATUS == 2) {
        if (ENEMY_Y > 450) {
            ENEMY_STATUS = -2;
            count_status = 50;
        }

        if (count_status<150) {
            ENEMY_Y += 1;
            count_status++;
        }  
        else{
            ENEMY_STATUS = 10;
            count_status = 0;
        }
    }

    if (ENEMY_STATUS == -2) {
        if (ENEMY_Y < 50) {
            ENEMY_STATUS = 2;
            count_status = 50;
        }

        if (count_status<150) {
            ENEMY_Y -= 1;
            count_status++;
        }  
        else{
            ENEMY_STATUS = 10;
            count_status = 0;
        }
    }

    if (ENEMY_STATUS == 10) {
        RAY_STATUS = rand() % 6;
        ENEMY_STATUS = -10;

        RAY_X = 0;
        RAY_Y = 0;

        for (int i=0; i<4; i++){
            RAY_X_list[i] = -10;
            RAY_Y_list[i] = -10;
        }
    }

    // RAYの動作振り分け
    if (RAY_STATUS == 0) {
        if (count_status<150){
            count_status++;
        }else{
            ENEMY_STATUS = 0;
            RAY_STATUS = 0;
            count_status = 0;
        }      
    }

    if (RAY_STATUS == 1) {
        if (count_status<150){
            RAY_X += 1;
            count_status++;

            RAY_X_list[0] = ENEMY_X + RAY_X*2;
            RAY_Y_list[0] = ENEMY_Y;

            RAY_X_list[1] = ENEMY_X - RAY_X*2;
            RAY_Y_list[1] = ENEMY_Y;

            RAY_X_list[2] = ENEMY_X;
            RAY_Y_list[2] = ENEMY_Y + RAY_X*2;

            RAY_X_list[3] = ENEMY_X;
            RAY_Y_list[3] = ENEMY_Y - RAY_X*2;      

        }else{
            ENEMY_STATUS = 0;
            RAY_STATUS = 0;
            count_status = 0;
        }
        
    }

    if (RAY_STATUS == 2) {
        if (count_status<270){
            count_status++;

            RAY_X_list[0] = ENEMY_X + 50*cos((double)count_status/10);
            RAY_Y_list[0] = ENEMY_Y + 50*sin((double)count_status/10);

            RAY_X_list[1] = ENEMY_X + 50*cos((double)count_status/10 + M_PI/2);
            RAY_Y_list[1] = ENEMY_Y + 50*sin((double)count_status/10 + M_PI/2);

            RAY_X_list[2] = ENEMY_X + 50*cos((double)count_status/10 + M_PI);
            RAY_Y_list[2] = ENEMY_Y + 50*sin((double)count_status/10 + M_PI);

            RAY_X_list[3] = ENEMY_X + 50*cos((double)count_status/10 + M_PI*3/2);
            RAY_Y_list[3] = ENEMY_Y + 50*sin((double)count_status/10 + M_PI*3/2);      

        }else{
            ENEMY_STATUS = 0;
            RAY_STATUS = 0;
            count_status = 0;
        }
    }

    if (RAY_STATUS == 3) {
        if (count_status<180){
            count_status++;

            RAY_X_list[0] = ENEMY_X + 2*count_status*cos((double)count_status/10);
            RAY_Y_list[0] = ENEMY_Y + 2*count_status*sin((double)count_status/10);

            RAY_X_list[1] = ENEMY_X + 2*count_status*cos((double)count_status/10 + M_PI/2);
            RAY_Y_list[1] = ENEMY_Y + 2*count_status*sin((double)count_status/10 + M_PI/2);

            RAY_X_list[2] = ENEMY_X + 2*count_status*cos((double)count_status/10 + M_PI);
            RAY_Y_list[2] = ENEMY_Y + 2*count_status*sin((double)count_status/10 + M_PI);

            RAY_X_list[3] = ENEMY_X + 2*count_status*cos((double)count_status/10 + M_PI*3/2);
            RAY_Y_list[3] = ENEMY_Y + 2*count_status*sin((double)count_status/10 + M_PI*3/2);      

        }else{
            ENEMY_STATUS = 0;
            RAY_STATUS = 0;
            count_status = 0;
        }
    }

    if (RAY_STATUS == 4) {
        if (count_status == 0) {
            mat_alloc(&y_0, 2, 1);
            mat_alloc(&y_1, 2, 1);
            mat_alloc(&y_2, 2, 1);

            mat_elem(y_0, 0, 0) = 30; // 初期位置20
            mat_elem(y_0, 0, 1) = 0; // 初期速度0

            mat_copy(&y_1, y_0);
            mat_copy(&y_2, y_0);
        }
        if (count_status<270){
            count_status++;

            runge_kutta(c1, c2, g, y_2, h, &y_1, f);
            RAY_X_list[0] = ENEMY_X + mat_elem(y_1, 0, 0);
            RAY_Y_list[0] = ENEMY_Y;

            RAY_X_list[1] = ENEMY_X - mat_elem(y_1, 0, 0);
            RAY_Y_list[1] = ENEMY_Y;

            RAY_X_list[2] = ENEMY_X;
            RAY_Y_list[2] = ENEMY_Y + mat_elem(y_1, 0, 0);

            RAY_X_list[3] = ENEMY_X;
            RAY_Y_list[3] = ENEMY_Y - mat_elem(y_1, 0, 0); 

            mat_copy(&y_2, y_1);    

        }else{
            mat_free(&y_0);
            mat_free(&y_1);
            mat_free(&y_2);
            ENEMY_STATUS = 0;
            RAY_STATUS = 0;
            count_status = 0;
        }
    }

    if (RAY_STATUS == 5) {
        if (count_status == 0) {
            mat_alloc(&y_0, 2, 1);
            mat_alloc(&y_1, 2, 1);
            mat_alloc(&y_2, 2, 1);

            mat_elem(y_0, 0, 0) = 30; // 初期位置20
            mat_elem(y_0, 0, 1) = 0; // 初期速度0

            mat_copy(&y_1, y_0);
            mat_copy(&y_2, y_0);
        }
        if (count_status<240){
            count_status++;

            runge_kutta(c1, c2, g, y_2, h, &y_1, f);
            RAY_X_list[0] = ENEMY_X + mat_elem(y_1, 0, 0)*cos((double)count_status/10);
            RAY_Y_list[0] = ENEMY_Y + mat_elem(y_1, 0, 0)*sin((double)count_status/10);

            RAY_X_list[1] = ENEMY_X + mat_elem(y_1, 0, 0)*cos((double)count_status/10 + M_PI/2);
            RAY_Y_list[1] = ENEMY_Y + mat_elem(y_1, 0, 0)*sin((double)count_status/10 + M_PI/2);

            RAY_X_list[2] = ENEMY_X + mat_elem(y_1, 0, 0)*cos((double)count_status/10 + M_PI);
            RAY_Y_list[2] = ENEMY_Y + mat_elem(y_1, 0, 0)*sin((double)count_status/10 + M_PI);

            RAY_X_list[3] = ENEMY_X + mat_elem(y_1, 0, 0)*cos((double)count_status/10 + M_PI*3/2);
            RAY_Y_list[3] = ENEMY_Y + mat_elem(y_1, 0, 0)*sin((double)count_status/10 + M_PI*3/2);   

            mat_copy(&y_2, y_1);    

        }else{
            mat_free(&y_0);
            mat_free(&y_1);
            mat_free(&y_2);
            ENEMY_STATUS = 0;
            RAY_STATUS = 0;
            count_status = 0;
        }
    }

    // 再描画
    glutPostRedisplay();

    // 100ミリ秒後にtimer関数を引数0で自分自身を呼び出す
    glutTimerFunc(10, timer, 0);
}

void mouse(int button, int state, int x, int y) {
    // ボタンが押されたら押されたボタンを記憶しておく
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        PRESS_BUTTON = button;
        if (STATUS>=0) {
            PREV_X = x;
            PREV_Y = y;
        }
        COLOR = 0;
    }

    // ボタンが離されたらボタンの記憶をクリアする
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        PRESS_BUTTON = -1;

        if (STATUS>=0) {
            reset_list();            
        }   
    }

    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        glColor3d(0.0, 1.0, 0.0);
    }
}

void motion(int x, int y) {
    // あまり頻繁に描画内容が更新されないように前の位置からの
    // 距離が一定以上になっているかを判定する
    int dx = PREV_X - x;
    int dy = PREV_Y - y;
    
    if (dx * dx + dy * dy > 25.0) {
        if (PRESS_BUTTON == GLUT_LEFT_BUTTON) {
            if (STATUS>=0) {
                PREV_X = x;
                PREV_Y = y;
                add_node(&list, PREV_X, WINDOW_HEIGHT - PREV_Y - 1);

                if (list.size > 60) {
                    list_node *node;
                    node = list.root;
                    list.root = list.root->next;
                    free(node);
                    list.size -= 1;
                }
            }
        }
    }
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 114) {
        glColor3d(1.0, 0.0, 0.0);
    }else if (key == 103) {
        glColor3d(0.0, 1.0, 0.0);
    }else if (key == 98) {
        glColor3d(0.0, 0.0, 1.0);
    }else if (key == 27) {
        exit(1);
    }
}

void init() {
    
    m = 60;
    k = 10;
    h = 0.5;
    c = 2.5;
    g = 9.8;
    c1 = k/m;
    c2 = c/m;

    // リストの初期化
    init_list(&list); 
}

int main(int argc, char **argv) {
    // GLUTの初期化
    glutInit(&argc, argv);
    // GLUTの機能の有効化
    glutInitDisplayMode(GLUT_DOUBLE);
    // ウィンドウのサイズを設定
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    // ウィンドウの作成 (引数はウィンドウのタイトル)
    glutCreateWindow("GLUT: Timer");
    // 描画に使う関数の登録
    glutDisplayFunc(display);
    // ウィンドウのサイズ変更時に呼ばれる関数の登録
    glutReshapeFunc(reshape);
    // 100ミリ秒後にtimer関数を引数0で呼び出す
    glutTimerFunc(10, timer, 0);
    // マウス関係のコールバック関数
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    // キーボード関係のコールバック関数
    glutKeyboardFunc(keyboard);
    // OpenGLの初期化処理  
    init();
    // 描画ループの開始
    glutMainLoop();

    // リストの破棄
    free_list(&list);
}