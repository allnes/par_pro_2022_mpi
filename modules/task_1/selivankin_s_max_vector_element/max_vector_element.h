// Copyright 2022 Selivankin Sergey
#ifndef MODULES_TASK_1_SELIVANKIN_S_MAX_VECTOR_ELEMENT_MAX_VECTOR_ELEMENT_H_
#define MODULES_TASK_1_SELIVANKIN_S_MAX_VECTOR_ELEMENT_MAX_VECTOR_ELEMENT_H_

#include <vector>

std::vector<int> getRandomVector(int size);
int getMaxVectorElemParallel(std::vector<int> global_vec, int count_size_vector);
int getMaxVectorElemSequence(std::vector<int> vec);

#endif  // MODULES_TASK_1_SELIVANKIN_S_MAX_VECTOR_ELEMENT_MAX_VECTOR_ELEMENT_H_
