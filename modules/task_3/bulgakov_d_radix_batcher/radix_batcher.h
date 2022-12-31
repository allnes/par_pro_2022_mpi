// Copyright 2022 Bulgakov Daniil

#ifndef MODULES_TASK_3_BULGAKOV_D_RADIX_BATCHER_RADIX_BATCHER_H_
#define MODULES_TASK_3_BULGAKOV_D_RADIX_BATCHER_RADIX_BATCHER_H_

#include <mpi.h>
#include <vector>
#include <random>

#include "../../modules/task_3/bulgakov_d_radix_batcher/batcher_merge.h"
#include "../../modules/task_3/bulgakov_d_radix_batcher/radix_sort.h"

std::vector<double> genvec(int sz);
std::vector<double> parallel_radix_batcher_sort(std::vector<double> to_sort_vec);

#endif  // MODULES_TASK_3_BULGAKOV_D_RADIX_BATCHER_RADIX_BATCHER_H_
