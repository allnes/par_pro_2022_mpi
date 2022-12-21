// Copyright 2022 Chernova Anna

#pragma once

#include <mpi.h>
#include <vector>
#include <iostream>
#include <random>
#include <algorithm>
using std::vector;
using std::cout;

vector<int> randArr(int size);

void oddEvenMerge(vector<int> *arr, int n, int lo = 0, int r = 1);
vector<int> merge(vector<vector<int>> vectorOfVal);
void shellSort(vector<int> *arr);
vector<int> shellSortMPI(vector<int> arr, int size);
