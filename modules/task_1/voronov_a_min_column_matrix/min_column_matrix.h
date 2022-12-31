// Copyright 2022 Voronov Alexander
#ifndef MODULES_TASK_1_VORONOV_A_MIN_COLUMN_MATRIX_MIN_COLUMN_MATRIX_H_
#define MODULES_TASK_1_VORONOV_A_MIN_COLUMN_MATRIX_MIN_COLUMN_MATRIX_H_

#include <vector>

std::vector <int> GetRandomMatrix(int rows, int column);

std::vector <int> GetTransposeMatrix(std::vector <int> Matrix, int rows, int columns);

std::vector <int> GetSequentialMinValueColumn(std::vector <int> Matrix, int rows, int columns);

std::vector <int> GetParallelMinValueColumn(std::vector <int> Matrix, int rows, int columns);

#endif  // MODULES_TASK_1_VORONOV_A_MIN_COLUMN_MATRIX_MIN_COLUMN_MATRIX_H_

