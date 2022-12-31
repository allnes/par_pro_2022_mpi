// Copyright 2022 Ivlev A
#ifndef  MODULES_TASK_1_IVLEV_A_MIN_VAL_BY_ROWS_MIN_VAL_BY_ROWS_H_
#define  MODULES_TASK_1_IVLEV_A_MIN_VAL_BY_ROWS_MIN_VAL_BY_ROWS_H_

#include <vector>
#include <string>

int* getRandomMatrix(int m, int n);

int min_in_vec(int* vec, int size);

int* getMatrixMinbyRow(int* global_matrix, int row_num, int column_num);

int* getParallelMin(int* global_matrix, int row_num, int column_num);


#endif  // MODULES_TASK_1_IVLEV_A_MIN_VAL_BY_ROWS_MIN_VAL_BY_ROWS_H_
