// Copyright 2018 Nesterov Alexander
#ifndef MODULES_TASK_1_CHERNOVA_A_LETTERS_COUNT_H_
#define MODULES_TASK_1_CHERNOVA_A_LETTERS_COUNT_H_

#include <string>
#include <vector>

std::vector<int> getRandomVector(int sz);
int getParallelOperations(std::vector<int> global_vec, int count_size_vector,
                          const std::string& ops);
int getSequentialOperations(std::vector<int> vec, const std::string& ops);

#endif  // MODULES_TASK_1_CHERNOVA_A_LETTERS_COUNT_H_