// Copyright 2022 Pronina Tatiana

#ifndef MODULES_TASK_3_PRONINA_T_MATRIX_MULTIPLICATION_PRONINA_T_MATRIX_MULTIPLICATION_H_
#define MODULES_TASK_3_PRONINA_T_MATRIX_MULTIPLICATION_PRONINA_T_MATRIX_MULTIPLICATION_H_

#include <vector>

struct SparseMatrix {
  // Non-zero coefficients of the matrix
  std::vector<double> val;
  // Pointer to the index of the non-zero coefficient of the array val
  std::vector<int> col_ptr;
  // Lowercase indexes for each column of the matrix
  std::vector<int> row_index;
  int columns = 0, rows = 0, non_zero = 0;

  friend const std::vector<double> operator*(const SparseMatrix& _A,
    const SparseMatrix& _B);
};

SparseMatrix CCS(const std::vector<double>& _newMatrix,
  const int _newColumns, const int _newRows);

std::vector<double> Multiply(SparseMatrix _A, SparseMatrix _B);
std::vector<double> RandMatrix(const int _columns, const int _rows);

#endif  // MODULES_TASK_3_PRONINA_T_MATRIX_MULTIPLICATION_PRONINA_T_MATRIX_MULTIPLICATION_H_
