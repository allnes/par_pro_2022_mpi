// Copyright 2018 Nesterov Alexander
#pragma once
#include <string>
#include <vector>

std::vector<int> getRandomVector(int sz);
int getParallelOperations(std::vector<int> global_vec, int count_size_vector,
                          const std::string& ops);
int getSequentialOperations(std::vector<int> vec, const std::string& ops);
