// Copyright 2022 Bushmakin Maksim
#ifndef MODULES_TASK_1_BUSHMAKIN_M_MATRIX_EL_SUM_MATRIX_EL_SUM_H_
#define MODULES_TASK_1_BUSHMAKIN_M_MATRIX_EL_SUM_MATRIX_EL_SUM_H_

#include <string>
#include <vector>

std::vector<int> genRandomIntMatrix(int m, int n);
int sequentialMatSum(const std::vector<int>& matrix, int m, int n);
int parallelMatSum(const std::vector<int>& matrix, int m, int n);

#endif  // MODULES_TASK_1_BUSHMAKIN_M_MATRIX_EL_SUM_MATRIX_EL_SUM_H_
