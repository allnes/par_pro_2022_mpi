// Copyright 2022 Kochetov Maksim
#ifndef MODULES_TASK_1_KOCHETOV_M_VECTOR_MIN_VALUE_VECTOR_MIN_VALUE_H_
#define MODULES_TASK_1_KOCHETOV_M_VECTOR_MIN_VALUE_VECTOR_MIN_VALUE_H_

#include <string>
#include <vector>

std::vector<int> genRandomVector(int vecSize);

double getVectorMinSequential(const std::vector<int>& vec);

double getVectorMinParralel(const std::vector<int>& globVec, const int vecSize);

#endif  // MODULES_TASK_1_KOCHETOV_M_VECTOR_MIN_VALUE_VECTOR_MIN_VALUE_H_
