// Copyright 2022 Bataev Ivan
#ifndef MODULES_TASK_1_BATAEV_I_VECTOR_ALTERNATIONS_SIGNS_VECTOR_ALTERNATIONS_SIGNS_H_
#define MODULES_TASK_1_BATAEV_I_VECTOR_ALTERNATIONS_SIGNS_VECTOR_ALTERNATIONS_SIGNS_H_

#include <vector>
#include <string>

std::vector<int> getRandomVector(int size, int left, int right);
void printVector(const std::vector<int>& v, const std::string& prefix = "");
int getNumAlterSignsSequential(std::vector<int> v);
int getNumAlterSignsParallel(std::vector<int> gv);

#endif  // MODULES_TASK_1_BATAEV_I_VECTOR_ALTERNATIONS_SIGNS_VECTOR_ALTERNATIONS_SIGNS_H_
