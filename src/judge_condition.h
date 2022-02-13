#include <stdio.h>
#include "matrix.h"
#include "locus_list.h"
#include "draw_object.h"

void judge_locus(int n_points, matrix *points, int *status, linked_list list, int PREV_X, int PREV_Y, int ENEMY_X, int ENEMY_Y, int *RAY_X_list, int *RAY_Y_list, int window_height);
void judge_status(linked_list *list, int *status, int *count, int *count_circle, int *PREV_X, int *PREV_Y, int ENEMY_X, int ENEMY_Y);