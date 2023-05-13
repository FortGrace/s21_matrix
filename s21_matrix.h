#ifndef SRC_S21_MATRIX_H_
#define SRC_S21_MATRIX_H_

#define FAILURE 0
#define SUCCESS 1
#define OK 0
#define INCORRECT_MATRIX 1
#define CALC_ERROR 2
#define ACCURACY 1e-7

#include <math.h>
#include <stdio.h>

typedef struct matrix_struct {
  double **matrix;
  int rows;
  int columns;
} matrix_t;

// library_functions
int s21_create_matrix(int rows, int columns, matrix_t *result);
void s21_remove_matrix(matrix_t *A);
int s21_eq_matrix(matrix_t *A, matrix_t *B);
int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result);
int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result);
int s21_mult_number(matrix_t *A, double number, matrix_t *result);
int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result);
int s21_transpose(matrix_t *A, matrix_t *result);
int s21_calc_complements(matrix_t *A, matrix_t *result);
int s21_determinant(matrix_t *A, double *result);
int s21_inverse_matrix(matrix_t *A, matrix_t *result);
// utils
void s21_print_matrix(matrix_t matrix);
void s21_fill_matrix(matrix_t *matrix, double number);
void s21_fill_matrix_by_one(matrix_t *matrix);
void s21_fill_matrix_by_list(matrix_t *matrix, double list[]);
void s21_copy_matrix(matrix_t src, matrix_t *des);
void s21_get_minor(matrix_t matrix, int elem_row, int elem_col,
                   matrix_t *result);
double s21_get_determinant(matrix_t matrix);

#endif  //  SRC_S21_MATRIX_H_
