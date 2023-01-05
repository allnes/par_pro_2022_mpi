// Copyright 2022 Anastasya Anastasya
#ifndef MODULES_TASK_1_ANTONOVA_N_NUM_VIOL_ORDER_VEC_NUM_VIOLATION_ORDER_VECTOR_H_
#define MODULES_TASK_1_ANTONOVA_N_NUM_VIOL_ORDER_VEC_NUM_VIOLATION_ORDER_VECTOR_H_

#include <mpi.h>
#include <vector>

std::vector<int> getRandomVector(int  sz);

int getNumViolationOrderVector(std::vector<int> vec);
int getNumViolationOrderVectorParallel(std::vector<int> global_vec, int count_size_vector);
#endif  // MODULES_TASK_1_ANTONOVA_N_NUM_VIOL_ORDER_VEC_NUM_VIOLATION_ORDER_VECTOR_H_
