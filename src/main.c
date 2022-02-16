#ifndef __APPLE__  // OSがMacでない (= WindowsやLinux)
#include <GL/glut.h>  // Windows, Linuxの場合のヘッダ
#else
#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>  // Macの場合のヘッダ
#endif

#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "matrix.h"
#include "apply_status.h"
// #include "draw_object.h"
#include "judge_condition.h"

#define epsilon 1.0e-12

#define MAX_POINTS 5

int window_width = 500;   // ウィンドウの横幅
int window_height = 500;  // ウィンドウの高さ

int n_points;
matrix points;

double TIME = 0.0;
const double RECT_SIZE = 300.0;


int enemy_status;
int ray_status;
int count_status;

int PREV_X = -10;
int PREV_Y = -10;

int PRESS_BUTTON = -1;
int COLOR = 0;

int ENEMY_X = 250;
int ENEMY_Y = 250;

int RAY_X_list[4];
int RAY_Y_list[4];
int RAY_X;
int RAY_Y;

int status = 0;

int count = 0;
int count_circle = 0;

double h, k, m, c, g, c1, c2;

linked_list list;

void display() {
    // ウィンドウ表示内容のクリア
    glClear(GL_COLOR_BUFFER_BIT);

    n_points =list.size;

    judge_locus(n_points, &points, &status, list, PREV_X, PREV_Y, ENEMY_X, ENEMY_Y, RAY_X_list, RAY_Y_list, window_height);
    judge_status(&list, &status, &count, &count_circle, &PREV_X, &PREV_Y, ENEMY_X, ENEMY_Y);

    draw_locus(status, n_points, &points);
    draw_number(&status, &enemy_status, &ray_status, count_circle, ENEMY_X, ENEMY_Y);
    draw_cursor(status, PREV_X, PREV_Y, window_height);
    draw_ray(status, ray_status, RAY_X_list, RAY_Y_list);


    // ENEMYの描画
    if (status > -5) {
        draw_enemy(status, ENEMY_X, ENEMY_Y);
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
    window_width = width;
    window_height = height;
}


void timer(int value) {
    // srand((unsigned int)time(NULL));

    // ENEMYの動作振り分け
    apply_enemy_status(&enemy_status, &count_status, &ray_status, &ENEMY_X, &ENEMY_Y, &RAY_X, &RAY_Y, RAY_X_list, RAY_Y_list);

    // RAYの動作振り分け
    apply_ray_status(&enemy_status, &count_status, &ray_status, &ENEMY_X, &ENEMY_Y, &RAY_X, &RAY_Y, RAY_X_list, RAY_Y_list, c1, c2, g, h);
    
    // 再描画
    glutPostRedisplay();

    // 100ミリ秒後にtimer関数を引数0で自分自身を呼び出す
    glutTimerFunc(10, timer, 0);
}

void mouse(int button, int state, int x, int y) {
    // ボタンが押されたら押されたボタンを記憶しておく
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        PRESS_BUTTON = button;
        if (status>=0) {
            PREV_X = x;
            PREV_Y = y;
        }
        COLOR = 0;
    }

    // ボタンが離されたらボタンの記憶をクリアする
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        PRESS_BUTTON = -1;

        if (status>=0) {
            reset_list(&list, &PREV_X, &PREV_Y);           
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
            if (status>=0) {
                PREV_X = x;
                PREV_Y = y;
                add_node(&list, PREV_X, window_height - PREV_Y - 1);

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

void init() {
    
    m = 60;
    k = 10;
    h = 0.5;
    c = 2.5;
    g = 9.8;
    c1 = k/m;
    c2 = c/m;

    enemy_status=0;
    ray_status=10;
    count_status=0;

    init_ray(&RAY_X, &RAY_Y, RAY_X_list, RAY_Y_list);
    init_list(&list); 
}

int main(int argc, char **argv) {
    // GLUTの初期化
    glutInit(&argc, argv);
    // GLUTの機能の有効化
    glutInitDisplayMode(GLUT_DOUBLE);
    // ウィンドウのサイズを設定
    glutInitWindowSize(window_width, window_height);
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
    // glutKeyboardFunc(keyboard);
    // OpenGLの初期化処理  
    init();
    // 描画ループの開始
    glutMainLoop();

    // リストの破棄
    free_list(&list);
}