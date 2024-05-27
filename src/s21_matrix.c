#include "s21_matrix.h"

int s21_create_matrix(int rows, int columns, matrix_t *result) {
  int status_code = SUCCESS;
  if ((rows <= 0 || columns <= 0) || result == NULL) {
    status_code = FAILURE;
  }
  if (status_code == SUCCESS) {
    result->rows = rows;
    result->columns = columns;
    result->matrix = (double **)malloc(rows * sizeof(double *));
  }
  if (result->matrix == NULL) {
    status_code = FAILURE;
  }
  if (status_code == SUCCESS) {
    for (int i = 0; i < rows; ++i) {
      result->matrix[i] = (double *)malloc(columns * sizeof(double));
    }
    for (int i = 0; i < rows; ++i) {
      for (int j = 0; j < columns; ++j) {
        result->matrix[i][j] = 0;
      }
    }
  }
  return status_code;
}

void s21_remove_matrix(matrix_t *A) {
  if (A != NULL && A->matrix != NULL) {
    for (int i = 0; i < A->rows; ++i) {
      free(A->matrix[i]);
    }
    free(A->matrix);
    A->rows = 0;
    A->columns = 0;
    A->matrix = NULL;
  }
}

int s21_check_matrix(matrix_t A) {
  int check = 1;
  if ((A.rows > 0) && (A.columns > 0) && (A.matrix)) check = 0;
  return check;
}

void s21_print_matrix(matrix_t *A) {
  if (A != NULL) {
    for (int row = 0; row < A->rows; ++row) {
      for (int col = 0; col < A->columns; ++col) {
        printf("%lf", A->matrix[row][col]);
        if (col != A->columns - 1) printf(" ");
      }
      if (row != A->rows - 1) printf("\n");
    }
  }
}

int s21_eq_matrix(matrix_t *A, matrix_t *B) {
  int status_code = SUCCESS_EQ;
  if ((s21_check_matrix(*A)) || (s21_check_matrix(*B)))
    status_code = FAILURE_EQ;
  if ((A->rows != B->rows) || (A->columns != B->columns))
    status_code = FAILURE_EQ;
  if (status_code == SUCCESS_EQ) {
    for (int i = 0; i < A->rows; ++i) {
      for (int j = 0; j < A->columns; ++j) {
        if (fabs(A->matrix[i][j] - B->matrix[i][j]) >= 1e-7) {
          status_code = FAILURE_EQ;
        }
      }
    }
  }
  return status_code;
}

int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int status_code = SUCCESS;
  if ((s21_check_matrix(*A)) || (s21_check_matrix(*B))) status_code = FAILURE;
  if (((A->rows != B->rows) || (A->columns != B->columns)) &&
      (status_code != FAILURE))
    status_code = CALC_ERROR;
  if (status_code == SUCCESS) {
    result->matrix = NULL;
    result->rows = 0;
    result->columns = 0;
    s21_create_matrix(A->rows, A->columns, result);
    for (int row = 0; row < A->rows; row++) {
      for (int col = 0; col < A->columns; col++) {
        result->matrix[row][col] = A->matrix[row][col] + B->matrix[row][col];
      }
    }
  }
  return status_code;
}

int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int status_code = SUCCESS;
  if ((s21_check_matrix(*A)) || (s21_check_matrix(*B))) status_code = FAILURE;
  if (((A->rows != B->rows) || (A->columns != B->columns)) &&
      (status_code != FAILURE))
    status_code = CALC_ERROR;
  if (status_code == SUCCESS) {
    result->matrix = NULL;
    result->rows = 0;
    result->columns = 0;
    s21_create_matrix(A->rows, A->columns, result);
    for (int row = 0; row < A->rows; row++) {
      for (int col = 0; col < A->columns; col++) {
        result->matrix[row][col] = A->matrix[row][col] - B->matrix[row][col];
      }
    }
  }
  return status_code;
}

int s21_mult_number(matrix_t *A, double number, matrix_t *result) {
  int status_code = SUCCESS;
  if (A == NULL || s21_check_matrix(*A))
    status_code = FAILURE;
  else {
    s21_create_matrix(A->rows, A->columns, result);
    for (int row = 0; row < A->rows; row++) {
      for (int col = 0; col < A->columns; col++) {
        result->matrix[row][col] = A->matrix[row][col] * number;
      }
    }
  }
  return status_code;
}

int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int status_code = SUCCESS;
  if (A == NULL || B == NULL || (s21_check_matrix(*A)) ||
      (s21_check_matrix(*B)))
    status_code = FAILURE;
  if (A->columns != B->rows && status_code != FAILURE) status_code = CALC_ERROR;
  if (status_code == SUCCESS) {
    s21_create_matrix(A->rows, B->columns, result);
    for (int row = 0; row < A->rows; row++) {
      for (int col = 0; col < B->columns; col++) {
        for (int var = 0; var < A->columns; var++) {
          result->matrix[row][col] += A->matrix[row][var] * B->matrix[var][col];
        }
      }
    }
  }
  return status_code;
}

int s21_transpose(matrix_t *A, matrix_t *result) {
  int status_code = SUCCESS;
  if (A == NULL || (s21_check_matrix(*A)))
    status_code = FAILURE;
  else {
    s21_create_matrix(A->columns, A->rows, result);
    for (int row = 0; row < A->rows; row++) {
      for (int col = 0; col < A->columns; col++) {
        result->matrix[col][row] = A->matrix[row][col];
      }
    }
  }
  return status_code;
}

int s21_determinant(matrix_t *A, double *result) {
  int status_code = SUCCESS;
  *result = 0;
  if (A != NULL && A->matrix != NULL && A->rows > 0 && A->columns > 0) {
    if (A->columns == A->rows) {
      if (A->columns == 1) {
        *result = A->matrix[0][0];
      } else if (A->columns == 2) {
        *result = A->matrix[0][0] * A->matrix[1][1] -
                  A->matrix[0][1] * A->matrix[1][0];
      } else {
        for (int ind = 0; ind < A->columns; ind++) {
          matrix_t minor;
          s21_create_matrix(A->rows - 1, A->columns - 1, &minor);
          for (int row = 1; row < A->rows; row++) {
            int col_minor = 0;
            for (int col = 0; col < A->columns; col++) {
              if (col != ind) {
                minor.matrix[row - 1][col_minor] = A->matrix[row][col];
                col_minor++;
              }
            }
          }
          double minor_det;
          s21_determinant(&minor, &minor_det);
          *result += pow(-1, ind) * A->matrix[0][ind] * minor_det;
          s21_remove_matrix(&minor);
        }
      }
    } else {
      status_code = CALC_ERROR;
    }
  } else {
    status_code = FAILURE;
  }
  return status_code;
}

int s21_calc_complements(matrix_t *A, matrix_t *result) {
  int status_code = SUCCESS;
  if (A != NULL && A->matrix != NULL && A->rows > 0 && A->columns > 0) {
    if (A->rows == A->columns) {
      if (A->columns == 1) {
        s21_create_matrix(A->columns, A->rows, result);
        result->matrix[0][0] = A->matrix[0][0];
      } else {
        s21_create_matrix(A->columns, A->rows, result);
        for (int row = 0; row < A->rows; row++) {
          for (int col = 0; col < A->columns; col++) {
            matrix_t minor;
            s21_create_matrix(A->rows - 1, A->columns - 1, &minor);
            int counter = 0;
            for (int ro = 0; ro < A->rows; ro++) {
              if (ro != row) {
                for (int co = 0; co < A->columns; co++) {
                  if (co != col) {
                    minor.matrix[(int)(counter / minor.rows)]
                                [counter % minor.rows] = A->matrix[ro][co];
                    counter++;
                  }
                }
              }
            }
            double det;
            s21_determinant(&minor, &det);
            result->matrix[row][col] = pow(-1, row + col) * det;
            s21_remove_matrix(&minor);
          }
        }
      }
    } else {
      status_code = CALC_ERROR;
    }
  } else {
    status_code = FAILURE;
  }
  return status_code;
}

int s21_inverse_matrix(matrix_t *A, matrix_t *result) {
  int code_result = 0;
  if (A != NULL && A->matrix != NULL && A->rows > 0 && A->columns > 0) {
    if (A->rows == A->columns) {
      double determinant = 0;
      s21_determinant(A, &determinant);
      if (determinant != 0) {
        matrix_t matrix_calc = {NULL, 0, 0};
        matrix_t matrix_transpose = {NULL, 0, 0};
        s21_calc_complements(A, &matrix_calc);
        s21_transpose(&matrix_calc, &matrix_transpose);
        s21_mult_number(&matrix_transpose, 1 / determinant, result);
        s21_remove_matrix(&matrix_calc);
        s21_remove_matrix(&matrix_transpose);
      } else {
        code_result = CALC_ERROR;
      }
    } else {
      code_result = CALC_ERROR;
    }
  } else {
    code_result = FAILURE;
  }
  return code_result;
}