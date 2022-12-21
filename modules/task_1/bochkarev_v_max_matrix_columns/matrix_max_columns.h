// Copyright 2022 Bochkarev Vladimir
#ifndef MODULES_TASK_1_BOCHKAREV_V_MAX_MATRIX_COLUMNS_MATRIX_MAX_COLUMNS_H_
#define MODULES_TASK_1_BOCHKAREV_V_MAX_MATRIX_COLUMNS_MATRIX_MAX_COLUMNS_H_

#include <vector>
#include <string>

std::vector<int> getRandomMatrix(int row, int col);
int getSequentialOperations(std::vector<int> vector);
std::vector<int> getParallelOperations(std::vector<int> matrix, int count_row, int count_col);

#endif  // MODULES_TASK_1_BOCHKAREV_V_MAX_MATRIX_COLUMNS_MATRIX_MAX_COLUMNS_H_
