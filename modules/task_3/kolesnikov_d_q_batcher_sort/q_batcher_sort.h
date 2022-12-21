// Copyright 2022 Kolesnikov Denis
#ifndef MODULES_TASK_3_KOLESNIKOV_D_Q_BATCHER_SORT_Q_BATCHER_SORT_H_
#define MODULES_TASK_3_KOLESNIKOV_D_Q_BATCHER_SORT_Q_BATCHER_SORT_H_

#include <stdio.h>
#include <mpi.h>
#include <random>
#include <iostream>
#include <vector>
using std::vector;
using std::swap;
vector<int> GenRndArr(int size);
void Compare(vector<int>* a, int i, int j);
vector<int> Merge(vector<vector<int>> v);
void BatcherMerge(vector<int>* a, int n, int l = 0, int r = 1);
void SeqQuickSort(vector<int>* data, int l, int r);
vector<int>  PrlQuickSort(vector<int> arr, int size);
#endif  // MODULES_TASK_3_KOLESNIKOV_D_Q_BATCHER_SORT_Q_BATCHER_SORT_H_
