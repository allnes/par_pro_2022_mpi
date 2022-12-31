// Copyright 2022 Bochkarev Vladimir
#ifndef MODULES_TASK_3_BOCHKAREV_V_RADIX_SORT_RADIX_SORT_H_
#define MODULES_TASK_3_BOCHKAREV_V_RADIX_SORT_RADIX_SORT_H_

#include <vector>

std::vector<int> getRandomVector(int size);
void countSort(int* v, int d, int size);
std::vector<int> getSequentialOperations(std::vector<int> vector);
std::vector<int> mergeAll(std::vector<std::vector<int> > m);
std::vector<int> getParallelOperations(std::vector<int> vector, int vec_size);

#endif  // MODULES_TASK_3_BOCHKAREV_V_RADIX_SORT_RADIX_SORT_H_
