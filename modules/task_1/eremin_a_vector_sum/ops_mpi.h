// Copyright 2022 Eremin Aleksandr
#ifndef MODULES_TASK_1_EREMIN_A_VECTOR_SUM_OPS_MPI_H_
#define MODULES_TASK_1_EREMIN_A_VECTOR_SUM_OPS_MPI_H_

#include <vector>
#include <string>

std::vector<int> random(int size);

int sum(std::vector<int> V);
int sumParallel(std::vector<int> Vector, int size);

#endif  // MODULES_TASK_1_EREMIN_A_VECTOR_SUM_OPS_MPI_H_
