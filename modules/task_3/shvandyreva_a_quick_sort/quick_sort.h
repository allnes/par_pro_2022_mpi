// Copyright 2022 Shvandyreva Alina
#ifndef MODULES_TASK_3_SHVANDYREVA_A_QUICK_SORT_QUICK_SORT_H_
#define MODULES_TASK_3_SHVANDYREVA_A_QUICK_SORT_QUICK_SORT_H_

#include <vector>

std::vector<int> getRandomVector(int size);
void quickSortImplementation(std::vector<int>* array, int a, int b);
std::vector<int> quickSortSequential(std::vector<int> arr);
std::vector<int> mergeSort(std::vector<int> arr1, std::vector<int> arr2);
std::vector<int> quickSortParallel(const std::vector<int> arr);

#endif  // MODULES_TASK_3_SHVANDYREVA_A_QUICK_SORT_QUICK_SORT_H_
