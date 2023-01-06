// Copyright 2023 Pikin Ilya
#ifndef MODULES_TASK_3_PIKIN_I_MERGE_Q_SORT_MERGE_Q_SORT_H_
#define MODULES_TASK_3_PIKIN_I_MERGE_Q_SORT_MERGE_Q_SORT_H_

#include <mpi.h>
#include <vector>

std::vector<double> getRandVec(int size);
void merge(double* res, double* a, double* b, int size_a, int size_b);
void quickSort(double* ar, int n);
void parallelQuickSort(double* ar, int n);

#endif  // MODULES_TASK_3_PIKIN_I_MERGE_Q_SORT_MERGE_Q_SORT_H_
