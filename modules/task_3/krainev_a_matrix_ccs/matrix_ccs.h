// Copyright 2022 Krainev Alexander
#ifndef MODULES_TASK_3_KRAINEV_A_MATRIX_CCS_MATRIX_CCS_H_
#define MODULES_TASK_3_KRAINEV_A_MATRIX_CCS_MATRIX_CCS_H_

#include <mpi.h>
#include <algorithm>
#include <ctime>
#include <iostream>
#include <random>
#include <vector>

struct Matrix {
  int c = 1, r = 1;
  int count;
  std::vector<double> non_zero;
  std::vector<int> row;
  std::vector<int> col;
};

Matrix gen_random_matrix(int r, int c);
std::vector<double> sequential(const Matrix& A, const Matrix& B);
std::vector<double> parallel(const Matrix& _A, const Matrix& _B);

#endif  // MODULES_TASK_3_KRAINEV_A_MATRIX_CCS_MATRIX_CCS_H_
