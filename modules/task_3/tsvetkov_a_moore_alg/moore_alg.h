// Copyright 2022 Tsvetkov

#ifndef MODULES_TASK_3_TSVETKOV_A_MOORE_ALG_MOORE_ALG_H_
#define MODULES_TASK_3_TSVETKOV_A_MOORE_ALG_MOORE_ALG_H_

#include <mpi.h>
#include <vector>

std::vector<int> sequenceMoore(std::vector<int> graph, int start, int n);
std::vector<int> parallelMoore(std::vector<int> graph, int start, int n);

#endif  // MODULES_TASK_3_TSVETKOV_A_MOORE_ALG_MOORE_ALG_H_
