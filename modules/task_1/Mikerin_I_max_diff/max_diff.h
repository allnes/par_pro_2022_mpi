// Copyright 2022 Mikerin Ilya
#ifndef MODULES_TASK_1_MIKERIN_I_MAX_DIFF_MAX_DIFF_H_
#define MODULES_TASK_1_MIKERIN_I_MAX_DIFF_MAX_DIFF_H_


#include <vector>
#include <string>

std::vector<int> getRandomVector(int  sz);
int getParallelOperations(std::vector<int> global_vec,
                          int count_size_vector);
int getSequentialOperations(std::vector<int> vec);

#endif  // MODULES_TASK_1_MIKERIN_I_MAX_DIFF_MAX_DIFF_H_
