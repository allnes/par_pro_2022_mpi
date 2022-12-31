// Copyright 2022 Selivankin Sergey
#ifndef MODULES_TASK_2_SELIVANKIN_S_MEDIAN_FILTER_MEDIAN_FILTER_H_
#define MODULES_TASK_2_SELIVANKIN_S_MEDIAN_FILTER_MEDIAN_FILTER_H_

#include <vector>

std::vector<int> getRandomMatrix(int m, int n);
int clamp(int value, int min, int max);
std::vector<int> getMedianFilterSequence(std::vector<int> mat, int m, int n);
void appendSubMatrixToMatrix(std::vector<int> sub_mat, std::vector<int>* mat, int* begin_i);
std::vector<int> getMedianFilterParallel(std::vector<int> global_mat, int m, int n);

#endif  // MODULES_TASK_2_SELIVANKIN_S_MEDIAN_FILTER_MEDIAN_FILTER_H_
