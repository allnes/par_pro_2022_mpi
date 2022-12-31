#pragma once
// Copyright 2022 Korobeynikova Alisa
#ifndef MODULES_TEST_TASKS_TEST_MPI_OPS_MPI_H_
#define MODULES_TEST_TASKS_TEST_MPI_OPS_MPI_H_

#include <random>
#include <vector>

std::vector<int> taskDistrib(const int proc_num, const int task_num);
std::vector<int> parallelSort(const std::vector<int>& global_vec);

#endif  // MODULES_TEST_TASKS_TEST_MPI_OPS_MPI_H_
