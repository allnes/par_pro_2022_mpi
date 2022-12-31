// Copyright 2022 Andrey Chubenko
#ifndef MODULES_TASK_1_CHUBENKO_A_VECTOR_DISORDER_CHUBENKO_A_VECTOR_DISORDER_H_
#define MODULES_TASK_1_CHUBENKO_A_VECTOR_DISORDER_CHUBENKO_A_VECTOR_DISORDER_H_

#include <vector>
#include <string>

std::vector<int> getRandomVector(int sz);
int getParallelNDisorder(const std::vector<int> &global_vec,
    int count_size_vector);
int getSequentialNDisorder(const std::vector<int> &vec);

#endif  // MODULES_TASK_1_CHUBENKO_A_VECTOR_DISORDER_CHUBENKO_A_VECTOR_DISORDER_H_
