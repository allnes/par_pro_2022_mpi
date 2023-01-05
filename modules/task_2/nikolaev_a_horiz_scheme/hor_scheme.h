// Copyright 2022 Nikolaev Alexander
#ifndef MODULES_TASK_2_NIKOLAEV_A_HORIZ_SCHEME_HOR_SCHEME_H_
#define MODULES_TASK_2_NIKOLAEV_A_HORIZ_SCHEME_HOR_SCHEME_H_

#include <vector>
#include <string>

std::vector<int> getRandomMatrix(int  n, int m);
std::vector<int> getRandomVector(int vecSize);
std::vector<int> getMultVectorParallel(const std::vector<int>& pMatrix,
                                const std::vector<int>& pVector, int n, int m);
std::vector<int> getMultVectorSequential(const std::vector<int>& pMatrix,
                                const std::vector<int>& pVector, int n, int m);

#endif  // MODULES_TASK_2_NIKOLAEV_A_HORIZ_SCHEME_HOR_SCHEME_H_
