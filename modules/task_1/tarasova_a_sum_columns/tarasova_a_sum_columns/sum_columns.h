// Copyright 2022 Tarasova Anastasia
#ifndef MODULES_TASK_1_TARASOVA_A_SUM_COLUMNS_TARASOVA_A_SUM_COLUMNS_SUM_COLUMNS_H_
#define MODULES_TASK_1_TARASOVA_A_SUM_COLUMNS_TARASOVA_A_SUM_COLUMNS_SUM_COLUMNS_H_
#include <mpi.h>
#include <random>

std::vector<int> CreateMatrix(const int rows, const int cols);
std::vector<int> GetSumCols(std::vector<int>& matrix, const int rows, const int cols);
std::vector<int> GetSumColsParallel(std::vector<int>& matrix, const int rows, const int cols);

#endif
//MODULES_TASK_1_TARASOVA_A_SUM_COLUMNS_TARASOVA_A_SUM_COLUMNS_SUM_COLUMNS_H_
