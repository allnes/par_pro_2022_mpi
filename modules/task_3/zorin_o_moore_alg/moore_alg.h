// Copyright 2022 Zorin Oleg
#ifndef MODULES_TASK_3_ZORIN_O_MOORE_ALG_MOORE_ALG_H_
#define MODULES_TASK_3_ZORIN_O_MOORE_ALG_MOORE_ALG_H_

#include <vector>
#include <deque>

#define INF 1000000
#define EXIT_PROCESS -1

int moore_algorithm(const std::vector<std::vector<int>> &adjacency_matrix, int start, int end, int size);

#endif  // MODULES_TASK_3_ZORIN_O_MOORE_ALG_MOORE_ALG_H_
