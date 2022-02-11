#include <stdio.h>
#include <math.h>

#include "apply_status.h"

matrix y_0, y_1, y_2;

void apply_enemy_status(int *enemy_status, int *count_status, int *ray_status, int *ENEMY_X, int *ENEMY_Y, int *RAY_X, int *RAY_Y, int *RAY_X_list, int *RAY_Y_list)
{
    switch (*enemy_status)
    {
    case 0:
        *enemy_status = rand() % 5 - 2;
        break;

    case 1:
        if (*ENEMY_X > 450)
        {
            *enemy_status = -1;
            *count_status = 50;
        }

        if (*count_status < 150)
        {
            *ENEMY_X += 1;
            *count_status += 1;
        }
        else
        {
            *enemy_status = 10;
            *count_status = 0;
        }
        break;

    case -1:
        if (*ENEMY_X < 50)
        {
            *enemy_status = 1;
            *count_status = 50;
        }

        if (*count_status < 150)
        {
            *ENEMY_X -= 1;
            *count_status += 1;
        }
        else
        {
            *enemy_status = 10;
            *count_status = 0;
        }
        break;

    case 2:
        if (*ENEMY_Y > 450)
        {
            *enemy_status = -2;
            *count_status = 50;
        }

        if (*count_status < 150)
        {
            *ENEMY_Y += 1;
            *count_status += 1;
        }
        else
        {
            *enemy_status = 10;
            *count_status = 0;
        }
        break;

    case -2:
        if (*ENEMY_Y < 50)
        {
            *enemy_status = 2;
            *count_status = 50;
        }

        if (*count_status < 150)
        {
            *ENEMY_Y -= 1;
            *count_status += 1;
        }
        else
        {
            *enemy_status = 10;
            *count_status = 0;
        }
        break;

    case 10:
        *ray_status = rand() % 6;
        *enemy_status = -10;

        *RAY_X = 0;
        *RAY_Y = 0;

        for (int i = 0; i < 4; i++)
        {
            RAY_X_list[i] = -10;
            RAY_Y_list[i] = -10;
        }
        break;

    default:
        printf("enemy_status");
    }
}

void apply_ray_status(int *enemy_status, int *count_status, int *ray_status, int *ENEMY_X, int *ENEMY_Y, int *RAY_X, int *RAY_Y, int *RAY_X_list, int *RAY_Y_list, double c1, double c2, double g, double h)
{
    switch (*ray_status)
    {
    case 0:
        if (*count_status < 150)
        {
            *count_status += 1;
        }
        else
        {
            *enemy_status = 0;
            *ray_status = 0;
            *count_status = 0;
        }
        break;

    case 1:
        if (*count_status < 150)
        {
            *RAY_X += 1;
            *count_status += 1;

            RAY_X_list[0] = *ENEMY_X + *RAY_X * 2;
            RAY_Y_list[0] = *ENEMY_Y;

            RAY_X_list[1] = *ENEMY_X - *RAY_X * 2;
            RAY_Y_list[1] = *ENEMY_Y;

            RAY_X_list[2] = *ENEMY_X;
            RAY_Y_list[2] = *ENEMY_Y + *RAY_X * 2;

            RAY_X_list[3] = *ENEMY_X;
            RAY_Y_list[3] = *ENEMY_Y - *RAY_X * 2;
        }
        else
        {
            *enemy_status = 0;
            *ray_status = 0;
            *count_status = 0;
        }
        break;

    case 2:
        if (*count_status < 270)
        {
            *count_status += 1;

            RAY_X_list[0] = *ENEMY_X + 50 * cos((double)(*count_status) / 10);
            RAY_Y_list[0] = *ENEMY_Y + 50 * sin((double)(*count_status) / 10);

            RAY_X_list[1] = *ENEMY_X + 50 * cos((double)(*count_status) / 10 + M_PI / 2);
            RAY_Y_list[1] = *ENEMY_Y + 50 * sin((double)(*count_status) / 10 + M_PI / 2);

            RAY_X_list[2] = *ENEMY_X + 50 * cos((double)(*count_status) / 10 + M_PI);
            RAY_Y_list[2] = *ENEMY_Y + 50 * sin((double)(*count_status) / 10 + M_PI);

            RAY_X_list[3] = *ENEMY_X + 50 * cos((double)(*count_status) / 10 + M_PI * 3 / 2);
            RAY_Y_list[3] = *ENEMY_Y + 50 * sin((double)(*count_status) / 10 + M_PI * 3 / 2);
        }
        else
        {
            *enemy_status = 0;
            *ray_status = 0;
            *count_status = 0;
        }
        break;

    case 3:
        if (*count_status < 180)
        {
            *count_status += 1;

            RAY_X_list[0] = *ENEMY_X + 2 * (*count_status) * cos((double)(*count_status) / 10);
            RAY_Y_list[0] = *ENEMY_Y + 2 * (*count_status) * sin((double)(*count_status) / 10);

            RAY_X_list[1] = *ENEMY_X + 2 * (*count_status) * cos((double)(*count_status) / 10 + M_PI / 2);
            RAY_Y_list[1] = *ENEMY_Y + 2 * (*count_status) * sin((double)(*count_status) / 10 + M_PI / 2);

            RAY_X_list[2] = *ENEMY_X + 2 * (*count_status) * cos((double)(*count_status) / 10 + M_PI);
            RAY_Y_list[2] = *ENEMY_Y + 2 * (*count_status) * sin((double)(*count_status) / 10 + M_PI);

            RAY_X_list[3] = *ENEMY_X + 2 * (*count_status) * cos((double)(*count_status) / 10 + M_PI * 3 / 2);
            RAY_Y_list[3] = *ENEMY_Y + 2 * (*count_status) * sin((double)(*count_status) / 10 + M_PI * 3 / 2);
        }
        else
        {
            *enemy_status = 0;
            *ray_status = 0;
            *count_status = 0;
        }
        break;

    case 4:
    // *ray_status=3;
        if (*count_status == 0)
        {
            mat_alloc(&y_0, 2, 1);
            mat_alloc(&y_1, 2, 1);
            mat_alloc(&y_2, 2, 1);

            mat_elem(y_0, 0, 0) = 30; // 初期位置20
            mat_elem(y_0, 0, 1) = 0;  // 初期速度0

            mat_copy(&y_1, y_0);
            mat_copy(&y_2, y_0);
        }
        if (*count_status < 270)
        {
            *count_status += 1;

            runge_kutta(c1, c2, g, y_2, h, &y_1, calculate_differential_equation);
            RAY_X_list[0] = (*ENEMY_X) + mat_elem(y_1, 0, 0);
            RAY_Y_list[0] = (*ENEMY_Y);

            RAY_X_list[1] = (*ENEMY_X) - mat_elem(y_1, 0, 0);
            RAY_Y_list[1] = (*ENEMY_Y);

            RAY_X_list[2] = (*ENEMY_X);
            RAY_Y_list[2] = (*ENEMY_Y) + mat_elem(y_1, 0, 0);

            RAY_X_list[3] = (*ENEMY_X);
            RAY_Y_list[3] = (*ENEMY_Y) - mat_elem(y_1, 0, 0);

            mat_copy(&y_2, y_1);
        }
        else
        {
            mat_free(&y_0);
            mat_free(&y_1);
            mat_free(&y_2);
            *enemy_status = 0;
            *ray_status = 0;
            *count_status = 0;
        }
        break;

    case 5:
    // *ray_status=3;
        if (*count_status == 0)
        {
            mat_alloc(&y_0, 2, 1);
            mat_alloc(&y_1, 2, 1);
            mat_alloc(&y_2, 2, 1);

            mat_elem(y_0, 0, 0) = 30; // 初期位置20
            mat_elem(y_0, 0, 1) = 0;  // 初期速度0

            mat_copy(&y_1, y_0);
            mat_copy(&y_2, y_0);
        }
        if (*count_status < 240)
        {
            *count_status += 1;

            runge_kutta(c1, c2, g, y_2, h, &y_1, calculate_differential_equation);
            RAY_X_list[0] = (*ENEMY_X) + mat_elem(y_1, 0, 0) * cos((double)(*count_status) / 10);
            RAY_Y_list[0] = (*ENEMY_Y) + mat_elem(y_1, 0, 0) * sin((double)(*count_status) / 10);

            RAY_X_list[1] = (*ENEMY_X) + mat_elem(y_1, 0, 0) * cos((double)(*count_status) / 10 + M_PI / 2);
            RAY_Y_list[1] = (*ENEMY_Y) + mat_elem(y_1, 0, 0) * sin((double)(*count_status) / 10 + M_PI / 2);

            RAY_X_list[2] = (*ENEMY_X) + mat_elem(y_1, 0, 0) * cos((double)(*count_status) / 10 + M_PI);
            RAY_Y_list[2] = (*ENEMY_Y) + mat_elem(y_1, 0, 0) * sin((double)(*count_status) / 10 + M_PI);

            RAY_X_list[3] = (*ENEMY_X) + mat_elem(y_1, 0, 0) * cos((double)(*count_status) / 10 + M_PI * 3 / 2);
            RAY_Y_list[3] = (*ENEMY_Y) + mat_elem(y_1, 0, 0) * sin((double)(*count_status) / 10 + M_PI * 3 / 2);

            mat_copy(&y_2, y_1);
        }
        else
        {
            mat_free(&y_0);
            mat_free(&y_1);
            mat_free(&y_2);
            *enemy_status = 0;
            *ray_status = 0;
            *count_status = 0;
        }
        break;

    default:
        printf("ray_status");
    }
}
