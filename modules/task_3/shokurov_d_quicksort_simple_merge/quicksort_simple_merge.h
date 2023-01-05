// Copyright 2022 Shokurov Daniil
#ifndef MODULES_TASK_3_SHOKUROV_D_QUICKSORT_SIMPLE_MERGE_QUICKSORT_SIMPLE_MERGE_H_
#define MODULES_TASK_3_SHOKUROV_D_QUICKSORT_SIMPLE_MERGE_QUICKSORT_SIMPLE_MERGE_H_

#include <random>
#include <vector>
#include <utility>

std::pair<int, int> split(int* arr, int n);
void single_quick_sort(int* arr, int n);
void merge(int* arr1, int size1, int size2);
void parallel_quick_sort(int* arr, size_t n);
bool check(int* arr, int n);
#endif  // MODULES_TASK_3_SHOKUROV_D_QUICKSORT_SIMPLE_MERGE_QUICKSORT_SIMPLE_MERGE_H_
