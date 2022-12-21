  // Copyright 2022 Prokofev Denis
#ifndef MODULES_TASK_1_PROKOFEV_D_MIN_VAL_IN_ROWS_MIN_VAL_IN_ROWS_H_
#define MODULES_TASK_1_PROKOFEV_D_MIN_VAL_IN_ROWS_MIN_VAL_IN_ROWS_H_
#include <mpi.h>
#include <vector>
#include <iostream>
#include <random>
std::vector<int> genMatr(int rows, int cols);
std::vector<int> minValRows(const std::vector<int>& mat, const size_t rows, const size_t cols);
#endif  // MODULES_TASK_1_PROKOFEV_D_MIN_VAL_IN_ROWS_MIN_VAL_IN_ROWS_H_
