#ifndef _MATRIX_H
#define _MATRIX_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define swap(a, b)      \
    do                  \
    {                   \
        double t = (a); \
        a = b;          \
        b = t;          \
    } while (0);

#define mat_elem(mat, row_idx, col_idx) (mat).elems[(row_idx) * (mat).cols + (col_idx)]

#define epsilon 1.0e-12

typedef struct
{
    int rows;
    int cols;
    double *elems;
} matrix;

bool mat_alloc(matrix *mat, int row, int col);
void mat_free(matrix *mat);
bool mat_copy(matrix *dst, matrix src);
bool mat_add(matrix *dst, matrix mat1, matrix mat2);
bool mat_sub(matrix *dst, matrix mat1, matrix mat2);
bool mat_mul(matrix *dst, matrix mat1, matrix mat2);
bool mat_mul_scalar(matrix *dst, matrix mat, double s);
bool mat_trans(matrix *dst, matrix src);
bool mat_identity(matrix *mat);
bool mat_equal(matrix mat1, matrix mat2);
void mat_print(matrix mat);
bool mat_solve(matrix *x, matrix A, matrix b);
bool mat_inverse(matrix *invA, matrix A);

#endif