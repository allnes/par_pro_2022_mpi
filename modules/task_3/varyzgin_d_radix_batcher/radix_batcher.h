// Copyright 2023 Varyzgin Dmitriy

#ifndef MODULES_TASK_3_VARYZGIN_D_RADIX_BATCHER_RADIX_BATCHER_H_
#define MODULES_TASK_3_VARYZGIN_D_RADIX_BATCHER_RADIX_BATCHER_H_

#include <mpi.h>
#include <vector>
#include <random>
#include <iostream>
#include <cfloat>
#include <cmath>
#include <string>
#include <algorithm>
#include <cstring>

#include "../../modules/task_3/varyzgin_d_radix_batcher/radix_batcher.h"

void radix_sort(double * arr, int size);
std::vector<double> genDobVec(int size);
std::vector<double> radixBatcherParallel(std::vector<double> globVec);

#endif  // MODULES_TASK_3_VARYZGIN_D_RADIX_BATCHER_RADIX_BATCHER_H_
