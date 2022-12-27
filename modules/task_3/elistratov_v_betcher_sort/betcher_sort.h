// Copyright 2022 Elistratov Vladimir
#ifndef MODULES_TASK_3_ELISTRATOV_V_BETCHER_SORT_BETCHER_SORT_H_
#define MODULES_TASK_3_ELISTRATOV_V_BETCHER_SORT_BETCHER_SORT_H_
#include <mpi.h>

#include <algorithm>
#include <ctime>
#include <iostream>
#include <random>
#include <vector>

std::vector<int> OddSpliter(std::vector<int> vec1, std::vector<int> vec2,
                            std::vector<int> res);
std::vector<int> EvenSpliter(std::vector<int> vec1, std::vector<int> vec2,
                             std::vector<int> res);
std::vector<int> SimpleComparator(std::vector<int> res, std::vector<int> even,
                                  std::vector<int> odd);
std::vector<int> genRand(int n);
bool degree_2(int n);
std::vector<int> SequentialRadixSort(std::vector<int> vec);
std::vector<int> BetcherMerge(std::vector<int> vec, int n);

#endif  // MODULES_TASK_3_ELISTRATOV_V_BETCHER_SORT_BETCHER_SORT_H_
