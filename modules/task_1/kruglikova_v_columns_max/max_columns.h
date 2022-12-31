// Copyright 2022 Kruglikova Valeriia
#ifndef MODULES_TASK_1_KRUGLIKOVA_V_COLUMNS_MAX_MAX_COLUMNS_H_
#define MODULES_TASK_1_KRUGLIKOVA_V_COLUMNS_MAX_MAX_COLUMNS_H_

#include<mpi.h>
#include<vector>

std::vector<int> getMatrix(int n, int m);
std::vector<int> getSequentialMax(const std::vector<int>& mat, int n, int m);
std::vector<int> getParallelMax(const std::vector<int>& mat, int n, int m);
std::vector<int> getTransposeMtx(const std::vector<int>& mat, int n, int m);

#endif  // MODULES_TASK_1_KRUGLIKOVA_V_COLUMNS_MAX_MAX_COLUMNS_H_
