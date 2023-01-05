// Copyright Anna Goncharova
#ifndef MODULES_TASK_3_GONCHAROVA_A_MOORS_ALGORITM_MOORS_ALGORITM_H_
#define MODULES_TASK_3_GONCHAROVA_A_MOORS_ALGORITM_MOORS_ALGORITM_H_

#include <vector>

std::vector<int> getRandomGraph(int size);
std::vector<int> Transpose(const std::vector<int>& g, int n);
std::vector<int> ParallelMoor(const std::vector<int>& g, int source,
                                int* flag = nullptr);
std::vector<int> Moors_algorithm(const std::vector<int>& g, int source,
                                int* flag = nullptr);

#endif  // MODULES_TASK_3_GONCHAROVA_A_MOORS_ALGORITM_MOORS_ALGORITM_H_
