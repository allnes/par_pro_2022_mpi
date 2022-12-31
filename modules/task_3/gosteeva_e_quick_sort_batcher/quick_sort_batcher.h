// Copyright 2022 Gosteeva Ekaterina

#ifndef MODULES_TASK_3_GOSTEEVA_E_QUICK_SORT_BATCHER_QUICK_SORT_BATCHER_H_
#define MODULES_TASK_3_GOSTEEVA_E_QUICK_SORT_BATCHER_QUICK_SORT_BATCHER_H_

#include <mpi.h>
#include <vector>
#include <random>

std::vector<int> Merge(std::vector<std::vector<int>> vec);
std::vector<int> GetRandomVec(int vec_size);
std::vector<int> QuickSortParrallel(const std::vector<int>& vec);

#endif  // MODULES_TASK_3_GOSTEEVA_E_QUICK_SORT_BATCHER_QUICK_SORT_BATCHER_H_
