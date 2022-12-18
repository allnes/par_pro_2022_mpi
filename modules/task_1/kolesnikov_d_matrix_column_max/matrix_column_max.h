// Copyright 2022 Kolesnikov Denis


#ifndef MODULES_TASK_1_KOLESNIKOV_D_MATRIX_COLUMN_MAX_MATRIX_COLUMN_MAX_H_
#define MODULES_TASK_1_KOLESNIKOV_D_MATRIX_COLUMN_MAX_MATRIX_COLUMN_MAX_H_

#include <mpi.h>

#include <vector>
#include <random>

using std::vector;

int CoordLin(int x, int y, int size_x);
vector<int> GenRndMtrx(int size_x, int size_y);
vector<int> MaxByColumnPrl(const vector<int>& matrix, int size_x, int size_y);
vector<int> MaxByColumnSeq(const vector<int>& matrix, int size_x, int size_y);

#endif  // MODULES_TASK_1_KOLESNIKOV_D_MATRIX_COLUMN_MAX_MATRIX_COLUMN_MAX_H_
