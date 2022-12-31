// Copyright 2022 Tuzhilkina Polina
#ifndef MODULES_TASK_3_TUZHILKINA_P_SHELLSORT_SHELLSORT_H_
#define MODULES_TASK_3_TUZHILKINA_P_SHELLSORT_SHELLSORT_H_
#include <string>
#include <vector>

std::vector<int> getUnsortedVec(int size);

void shellsortSeq(std::vector<int>* array);

void shellsortPar(std::vector<int>* globVec, int glob_vec_size);

void printVecElements(const std::vector<int>& vec);

#endif  // MODULES_TASK_3_TUZHILKINA_P_SHELLSORT_SHELLSORT_H_
