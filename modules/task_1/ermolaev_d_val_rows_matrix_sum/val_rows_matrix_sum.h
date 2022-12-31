  // Copyright 2022 Ermolaev Danila
#ifndef  MODULES_TASK_1_ERMOLAEV_D_VAL_ROWS_MATRIX_SUM_VAL_ROWS_MATRIX_SUM_H_
#define  MODULES_TASK_1_ERMOLAEV_D_VAL_ROWS_MATRIX_SUM_VAL_ROWS_MATRIX_SUM_H_

#include <vector>
#include <string>
#include <numeric>
#include <random>
#include <algorithm>
#include <iostream>

int* getRandomMatrix(int x, int y);

void getSequentialOperation(int* matrix, int* result, int x, int y);

void getParallelOperation(int* matrix, int* result, int size_x, int size_y);

#endif  // MODULES_TASK_1_ERMOLAEV_D_VAL_ROWS_MATRIX_SUM_VAL_ROWS_MATRIX_SUM_H_
