// Copyright 2022 Nikolaev Alexander
#ifndef MODULES_TASK_1_NIKOLAEV_A_VECTOR_AVERAGE_VECTOR_AVERAGE_H_
#define MODULES_TASK_1_NIKOLAEV_A_VECTOR_AVERAGE_VECTOR_AVERAGE_H_

#include <vector>
#include <string>

std::vector<int> getRandomVector(int size);

float getAverageVectorParallel(std::vector<int> vec, int count_size_vector);

float getAverageVectorSequential(std::vector<int> vec, const int GlobVecSize);

#endif  // MODULES_TASK_1_NIKOLAEV_A_VECTOR_AVERAGE_VECTOR_AVERAGE_H_
