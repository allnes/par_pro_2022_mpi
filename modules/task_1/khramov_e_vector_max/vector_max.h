// Copyright 2022 Khramov Egor
#ifndef MODULES_TASK_1_KHRAMOV_E_VECTOR_MAX_VECTOR_MAX_H_
#define MODULES_TASK_1_KHRAMOV_E_VECTOR_MAX_VECTOR_MAX_H_

#include <vector>

void printVector(const std::vector<int>& vec);

std::vector<int> getRandomVector(int size);

int getMax(std::vector<int> vec);

int getMaxParallel(const std::vector<int>& vec, int vec_size);

#endif  // MODULES_TASK_1_KHRAMOV_E_VECTOR_MAX_VECTOR_MAX_H_
