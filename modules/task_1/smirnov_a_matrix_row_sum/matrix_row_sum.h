// Copyright 2018 Nesterov Alexander
#ifndef MODULES_TASK_1_SMIRNOV_A_MATRIX_ROW_SUM_MATRIX_ROW_SUM_H_
#define MODULES_TASK_1_SMIRNOV_A_MATRIX_ROW_SUM_MATRIX_ROW_SUM_H_

#include <vector>
#include <string>

std::vector<int> getRandomMatrix(int countRows, int countColumns);
std::vector<int> calculateParallelMatrixRowSum(std::vector<int> global_vec, int countRows, int countColumns);
std::vector<int> calculateSequentialMatrixRowSum(const std::vector<int>& matrix, int countRows, int countColumns);
int calculateSumRow(std::vector<int> row);
#endif  // MODULES_TASK_1_SMIRNOV_A_MATRIX_ROW_SUM_MATRIX_ROW_SUM_H_
