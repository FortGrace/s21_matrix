#include "s21_matrix.h"

#include <stdlib.h>

int s21_create_matrix(int rows, int columns, matrix_t *result) {
  int return_code = OK;
  double **matrix = NULL;
  if (rows < 1 || columns < 1) {
    return_code = INCORRECT_MATRIX;
  } else if ((matrix = (double **)malloc(rows * sizeof(double *) +
                                         rows * columns * sizeof(double))) ==
             NULL) {
    return_code = INCORRECT_MATRIX;
  } else {
    matrix[0] = (double *)(matrix + rows);
    for (int i = 1; i < rows; i++) matrix[i] = matrix[0] + i * columns;
    {
      for (int r = 0; r < rows; r++) {
        for (int c = 0; c < columns; c++) {
          matrix[r][c] = 0;
        }
        result->rows = rows;
        result->columns = columns;
        result->matrix = matrix;
      }
    }
  }
  return return_code;
}

void s21_remove_matrix(matrix_t *A) {
  if (A->matrix) {
    free(A->matrix);
    A->matrix = NULL;
  }
}

int s21_eq_matrix(matrix_t *A, matrix_t *B) {
  int return_code = FAILURE;
  if (A && B && A->rows == B->rows && A->columns == B->columns) {
    return_code = SUCCESS;
    for (int r = 0; r < A->rows; r++) {
      for (int c = 0; c < A->columns; c++) {
        if (fabs(A->matrix[r][c] - B->matrix[r][c]) > ACCURACY)
          return_code = FAILURE;
      }
    }
  }
  return return_code;
}

int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int return_code = OK;
  if (A->rows != B->rows || A->columns != B->columns) {
    return_code = CALC_ERROR;
  } else {
    s21_create_matrix(A->rows, A->columns, result);
    for (int r = 0; r < A->rows; r++) {
      for (int c = 0; c < A->columns; c++) {
        result->matrix[r][c] = A->matrix[r][c] + B->matrix[r][c];
      }
    }
  }
  return return_code;
}

int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int return_code = OK;
  if (A->rows != B->rows || A->columns != B->columns) {
    return_code = CALC_ERROR;
  } else {
    s21_create_matrix(A->rows, A->columns, result);
    for (int r = 0; r < A->rows; r++) {
      for (int c = 0; c < A->columns; c++) {
        result->matrix[r][c] = A->matrix[r][c] - B->matrix[r][c];
      }
    }
  }
  return return_code;
}

int s21_mult_number(matrix_t *A, double number, matrix_t *result) {
  int return_code = OK;
  s21_create_matrix(A->rows, A->columns, result);
  for (int r = 0; r < A->rows; r++) {
    for (int c = 0; c < A->columns; c++) {
      result->matrix[r][c] = A->matrix[r][c] * number;
    }
  }
  return return_code;
}

int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int return_code = OK;
  if (A->columns != B->rows) {
    return_code = CALC_ERROR;
  } else {
    s21_create_matrix(A->rows, B->columns, result);
    for (int c = 0; c < A->columns; c++) {
      for (int a = 0; a < A->rows; a++) {
        for (int b = 0; b < B->columns; b++) {
          result->matrix[a][b] += A->matrix[a][c] * B->matrix[c][b];
        }
      }
    }
  }
  return return_code;
}

int s21_transpose(matrix_t *A, matrix_t *result) {
  int return_code = OK;
  s21_create_matrix(A->columns, A->rows, result);
  for (int r = 0; r < result->rows; r++) {
    for (int c = 0; c < result->columns; c++) {
      result->matrix[r][c] = A->matrix[c][r];
    }
  }
  return return_code;
}

int s21_calc_complements(matrix_t *A, matrix_t *result) {
  int return_code = OK;
  if (A->rows != A->columns) {
    return_code = CALC_ERROR;
  } else {
    s21_create_matrix(A->rows, A->columns, result);
    if (A->rows == 1) {
      result->matrix[0][0] = A->matrix[0][0];
    } else {
      for (int r = 0; r < A->rows; r++) {
        for (int c = 0; c < A->columns; c++) {
          matrix_t minor;
          s21_get_minor(*A, r, c, &minor);
          double determinant = 0;
          s21_determinant(&minor, &determinant);
          int sign = (r + c + 2) % 2 == 0 ? 1 : -1;
          result->matrix[r][c] = sign * determinant;
          s21_remove_matrix(&minor);
        }
      }
    }
  }
  return return_code;
}

int s21_determinant(matrix_t *A, double *result) {
  int return_code = OK;
  *result = 0;
  if (A->rows != A->columns) {
    return_code = CALC_ERROR;
  } else {
    if (A->rows <= 2) {
      *result = s21_get_determinant(*A);
    } else {
      for (int c = 0; c < A->columns; c++) {
        matrix_t minor;
        s21_get_minor(*A, 0, c, &minor);
        double determinant = 0;
        s21_determinant(&minor, &determinant);
        int sign = (c + 1) % 2 == 0 ? -1 : 1;
        *result += A->matrix[0][c] * sign * determinant;
        s21_remove_matrix(&minor);
      }
    }
  }
  return return_code;
}

int s21_inverse_matrix(matrix_t *A, matrix_t *result) {
  int return_code = OK;
  if (A->rows != A->columns) {
    return_code = CALC_ERROR;
  } else {
    double determinant = 0;
    s21_determinant(A, &determinant);
    if (fabs(determinant) > ACCURACY) {
      matrix_t comp, tran;
      s21_calc_complements(A, &comp);
      s21_transpose(&comp, &tran);
      s21_mult_number(&tran, 1 / determinant, result);
      s21_remove_matrix(&comp);
      s21_remove_matrix(&tran);
    } else {
      return_code = CALC_ERROR;
    }
  }
  return return_code;
}

/* UTILS */

void s21_fill_matrix(matrix_t *matrix, double number) {
  for (int r = 0; r < matrix->rows; r++) {
    for (int c = 0; c < matrix->columns; c++) {
      matrix->matrix[r][c] = number;
    }
  }
}

void s21_fill_matrix_by_list(matrix_t *matrix, double list[]) {
  int i = 0;
  for (int r = 0; r < matrix->rows; r++) {
    for (int c = 0; c < matrix->columns; c++) {
      matrix->matrix[r][c] = list[i];
      i++;
    }
  }
}

void s21_get_minor(matrix_t matrix, int elem_row, int elem_col,
                   matrix_t *result) {
  s21_create_matrix(matrix.rows - 1, matrix.columns - 1, result);
  for (int r = 0, rm = 0; r < matrix.rows; r++) {
    for (int c = 0, cm = 0; c < matrix.columns; c++) {
      if (r != elem_row && c != elem_col) {
        result->matrix[rm][cm] = matrix.matrix[r][c];
        cm++;
        if (cm == result->rows) {
          cm = 0;
          rm++;
        }
      }
    }
  }
}

double s21_get_determinant(matrix_t matrix) {
  double determinant = 0;
  if (matrix.rows == 1) {
    determinant = matrix.matrix[0][0];
  } else {
    determinant = matrix.matrix[0][0] * matrix.matrix[1][1] -
                  matrix.matrix[0][1] * matrix.matrix[1][0];
  }
  return determinant;
}
