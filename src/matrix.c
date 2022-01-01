#include "matrix.h"

bool is_valid(matrix mat)
{
    return mat.rows > 0 && mat.cols > 0 && mat.elems != NULL;
}

bool is_same_size(matrix mat1, matrix mat2)
{
    return mat1.rows == mat2.rows && mat1.cols == mat2.cols;
}

bool mat_alloc(matrix *mat, int rows, int cols)
{
    if (rows <= 0 || cols <= 0)
        return false;

    mat->elems = (double *)malloc(rows * cols * sizeof(double));

    if (!mat->elems)
    {
        return false;
    }
    else
    {
        mat->rows = rows;
        mat->cols = cols;
        return true;
    }
}

void mat_free(matrix *mat)
{
    if (mat->elems != NULL)
    {
        free(mat->elems);
        mat->rows = 0;
        mat->cols = 0;
        mat->elems = NULL;
    }
}

void mat_print(matrix mat)
{
    if (!is_valid(mat))
    {
        fprintf(stderr, "Matrix is NULL or zero size!\n");
        return;
    }

    for (int row_idx = 0; row_idx < mat.rows; row_idx++)
    {
        for (int col_idx = 0; col_idx < mat.cols; col_idx++)
        {
            printf("%6.4f%s", mat_elem(mat, row_idx, col_idx), (col_idx == mat.cols - 1) ? "\n" : "  ");
        }
    }
}

bool mat_copy(matrix *dst, matrix src)
{

    if (!is_same_size(src, *dst))
        return false;
    if (!is_valid(src) || !is_valid(*dst))
        return false;

    for (int i = 0; i < src.rows * src.cols; i++)
    {
        dst->elems[i] = src.elems[i];
    }

    return true;
}

bool mat_add(matrix *res, matrix mat1, matrix mat2)
{
    if (!is_valid(*res) || !is_valid(mat1) || !is_valid(mat2))
        return false;
    if (!is_same_size(mat1, mat2) || !is_same_size(*res, mat1))
        return false;

    for (int idx = 0; idx < res->rows * res->cols; idx++)
    {
        const double val1 = mat1.elems[idx];
        const double val2 = mat2.elems[idx];
        res->elems[idx] = val1 + val2;
    }

    return true;
}

bool mat_sub(matrix *res, matrix mat1, matrix mat2)
{
    if (!is_valid(*res) || !is_valid(mat1) || !is_valid(mat2))
        return false;
    if (!is_same_size(mat1, mat2) || !is_same_size(*res, mat1))
        return false;

    for (int idx = 0; idx < res->rows * res->cols; idx++)
    {
        const double val1 = mat1.elems[idx];
        const double val2 = mat2.elems[idx];
        res->elems[idx] = val1 - val2;
    }

    return true;
}

bool mat_mul(matrix *res, matrix mat1, matrix mat2)
{

    if (!is_valid(*res) || !is_valid(mat1) || !is_valid(mat2))
        return false;
    if (res->rows != mat1.rows || mat1.cols != mat2.rows || res->cols != mat2.cols)
        return false;

    matrix mat_temp;
    if (!mat_alloc(&mat_temp, mat1.rows, mat2.cols))
        return false;

    for (int row_idx = 0; row_idx < mat1.rows; row_idx++)
    {
        for (int col_idx = 0; col_idx < mat2.cols; col_idx++)
        {
            mat_elem(mat_temp, row_idx, col_idx) = 0.0;
            for (int idx = 0; idx < mat1.cols; idx++)
            {
                mat_elem(mat_temp, row_idx, col_idx) += mat_elem(mat1, row_idx, idx) * mat_elem(mat2, idx, col_idx);
            }
        }
    }

    mat_copy(res, mat_temp);
    mat_free(&mat_temp);

    return true;
}

bool mat_mul_scalar(matrix *res, matrix mat, double scalar)
{
    if (!is_valid(*res) || !is_valid(mat))
        return false;
    if (!is_same_size(*res, mat))
        return false;

    for (int idx = 0; idx < mat.rows * mat.cols; idx++)
    {
        res->elems[idx] = mat.elems[idx] * scalar;
    }

    return true;
}

bool mat_trans(matrix *res, matrix mat)
{
    if (res->rows != mat.cols || res->cols != mat.rows)
        return false;

    matrix mat_temp;
    mat_alloc(&mat_temp, res->rows, res->cols);

    for (int row_idx = 0; row_idx < res->rows; row_idx++)
    {
        for (int col_idx = 0; col_idx < res->cols; col_idx++)
        {
            mat_elem(mat_temp, row_idx, col_idx) = mat_elem(mat, col_idx, row_idx);
        }
    }

    mat_copy(res, mat_temp);
    mat_free(&mat_temp);

    return true;
}

bool mat_identity(matrix *mat)
{
    if (mat->rows != mat->cols)
        return false;

    for (int row_idx = 0; row_idx < mat->rows; row_idx++)
    {
        for (int col_idx = 0; col_idx < mat->cols; col_idx++)
        {
            if (row_idx == col_idx)
            {
                mat_elem(*mat, row_idx, col_idx) = 1;
            }
            else
            {
                mat_elem(*mat, row_idx, col_idx) = 0;
            }
        }
    }
    return true;
}

bool mat_equal(matrix mat1, matrix mat2)
{
    if (!is_same_size(mat1, mat2))
        return false;

    for (int row_idx = 0; row_idx < mat1.rows; row_idx++)
    {
        for (int col_idx = 0; col_idx < mat1.cols; col_idx++)
        {
            if (fabs(mat_elem(mat1, row_idx, col_idx) - mat_elem(mat2, row_idx, col_idx)) > epsilon)
                return false;
        }
    }
    return true;
}

bool mat_solve(matrix *x, matrix A, matrix b)
{
    if (!is_valid(*x) || !is_valid(A) || !is_valid(b))
        return false;
    if (A.cols != x->rows || A.rows != b.rows || x->cols != b.cols)
        return false;

    matrix A_temp, b_temp;

    if (!mat_alloc(&A_temp, A.rows, A.cols))
        return false;
    if (!mat_alloc(&b_temp, b.rows, b.cols))
        return false;
    mat_copy(&A_temp, A);
    mat_copy(&b_temp, b);

    int pivot = 0;
    for (int i = 0; i < A.rows - 1; i++)
    {
        pivot = i;
        for (int col_idx = i + 1; col_idx < A.cols; col_idx++)
        {
            if (fabs(mat_elem(A_temp, pivot, i)) < fabs(mat_elem(A_temp, col_idx, i)))
            {
                pivot = col_idx;
            }
        }

        if (fabs(mat_elem(A_temp, pivot, i)) < epsilon)
        {
            mat_free(&A_temp);
            mat_free(&b_temp);
            return false;
        }

        for (int col_idx = i; col_idx < A.cols; col_idx++)
        {
            swap(mat_elem(A_temp, pivot, col_idx), mat_elem(A_temp, i, col_idx));
        }
        for (int col_idx = 0; col_idx < b.cols; col_idx++)
        {
            swap(mat_elem(b_temp, pivot, col_idx), mat_elem(b_temp, i, col_idx));
        }

        for (int row_idx = i + 1; row_idx < A.rows; row_idx++)
        {
            double ratio = mat_elem(A_temp, row_idx, i) / mat_elem(A_temp, i, i);

            for (int col_idx = i; col_idx < A.cols; col_idx++)
            {
                mat_elem(A_temp, row_idx, col_idx) -= ratio * mat_elem(A_temp, i, col_idx);
            }
            for (int col_idx = 0; col_idx < b.cols; col_idx++)
            {
                mat_elem(b_temp, row_idx, col_idx) -= ratio * mat_elem(b_temp, i, col_idx);
            }
        }
    }

    for (int row_idx = A.rows - 1; row_idx >= 0; row_idx--)
    {
        for (int col_idx = 0; col_idx < b.cols; col_idx++)
        {
            for (int idx = row_idx + 1; idx < A.rows; idx++)
            {
                mat_elem(b_temp, row_idx, col_idx) -= mat_elem(A_temp, row_idx, idx) * mat_elem(b_temp, idx, col_idx);
            }
            mat_elem(b_temp, row_idx, col_idx) /= mat_elem(A_temp, row_idx, row_idx);
        }
    }

    mat_copy(x, b_temp);
    mat_free(&A_temp);
    mat_free(&b_temp);

    return true;
}

bool mat_inverse(matrix *mat_inv, matrix mat)
{
    if (!is_same_size(*mat_inv, mat) || mat.rows != mat.cols)
        return false;

    matrix mat_temp;

    if (!mat_alloc(&mat_temp, mat.rows, mat.cols))
        return false;
    mat_identity(&mat_temp);

    if (!mat_solve(mat_inv, mat, mat_temp))
    {
        return false;
    }
    mat_free(&mat_temp);

    return true;
}
