#include <stdio.h>
#include "judge_condition.h"

void judge_locus(int n_points, matrix *points, int *status, linked_list list, int PREV_X, int PREV_Y, int ENEMY_X, int ENEMY_Y, int *RAY_X_list, int *RAY_Y_list, int window_height){
    int r_ok=0;
    int l_ok=0;
    int u_ok=0;
    int d_ok=0;
    int ok = 0;
    
    mat_alloc(points, n_points, 2);
    list_node *iter = list.root;

    // 接触判定，囲まれていることを判定
    for (int i=0; i<n_points; i++){
        mat_elem(*points, i, 0) = (double)iter->x;
        mat_elem(*points, i, 1) =  (double)iter->y;
        
        iter = iter->next;

        if (fabs(mat_elem(*points, i, 0)-ENEMY_X)<10 && fabs(mat_elem(*points, i, 1)-ENEMY_Y)<15) {
            *status = -1;
        }

        for (int j=0; j<4; j++){
            if (fabs(mat_elem(*points, i, 0)-RAY_X_list[j])<5 && fabs(mat_elem(*points, i, 1)-RAY_Y_list[j])<5) {
            *status = -1;
            }
        }

        

        if (fabs(mat_elem(*points, i, 1)-ENEMY_Y)<20) {
            if (mat_elem(*points, i, 0)<ENEMY_X) {
                l_ok += 1;
            }
            else{
                r_ok += 1;
            }
        }

        if (fabs(mat_elem(*points, i, 0)-ENEMY_X)<20) {
            if (mat_elem(*points, i, 1)<ENEMY_Y) {
                d_ok += 1;
            }
            else{
                u_ok += 1;
            }
        }
        if (i < n_points/2) {

            if (fabs(mat_elem(*points, i, 0)-PREV_X)<20 && fabs(mat_elem(*points, i, 1)-(window_height - PREV_Y - 1))<10) {
                ok += 1;
            }
        }
    }


    if (l_ok > 0 && r_ok > 0 && u_ok > 0 && d_ok > 0 && ok > 0 && ok > 0) {
        *status = 1;
    }

    free(iter);
}

void judge_status(linked_list *list, int *status, int *count, int *count_circle, int *PREV_X, int *PREV_Y, int ENEMY_X, int ENEMY_Y){
    // statusの処理
    if (*status<0 && *status>-5) {
        *count += 1;

        if (*count > 30) {
            reset_list(list, PREV_X, PREV_Y);

            *count = 0;
            *status = 0;

            if (*count_circle>0){
                *count_circle -= 1;
            }
        }    
    }
    if (*status == 1) {
        *count += 1;
        if (*count > 5) { 
            reset_list(list, PREV_X, PREV_Y);

            *count = 0;
            *status = 0;
            *count_circle += 1;
        }
    }
    if (*status == -5){
        *count += 1;

        draw_ball(ENEMY_X, ENEMY_Y, 21.0);

        if (*count>100){
            *status = -6;
        }
    }
}


