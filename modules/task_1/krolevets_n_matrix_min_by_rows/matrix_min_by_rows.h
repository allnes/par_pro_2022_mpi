// Copyright 2022 me
#ifndef MODULES_TASK_1_KROLEVETS_N_MATRIX_MIN_BY_ROWS_MATRIX_MIN_BY_ROWS_H_
#define MODULES_TASK_1_KROLEVETS_N_MATRIX_MIN_BY_ROWS_MATRIX_MIN_BY_ROWS_H_
#include <mpi.h>

#include <algorithm>
#include <cassert>
#include <iostream>
#include <limits>

void min_by_rows(int* matrix, int* result, int size_x, int size_y);
void min_by_rows_seq(int* matrix, int* result, int size_x, int size_y);
void generate_matrix(int* matrix, int size_x, int size_y);

#endif  // MODULES_TASK_1_KROLEVETS_N_MATRIX_MIN_BY_ROWS_MATRIX_MIN_BY_ROWS_H_
