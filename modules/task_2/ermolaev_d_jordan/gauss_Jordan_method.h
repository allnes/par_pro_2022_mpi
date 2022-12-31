// Copyright 2022 Ermolaev Danila
#ifndef MODULES_TASK_2_ERMOLAEV_D_JORDAN_GAUSS_JORDAN_METHOD_H_
#define MODULES_TASK_2_ERMOLAEV_D_JORDAN_GAUSS_JORDAN_METHOD_H_

#include <vector>
#include <string>
#include <numeric>
#include <random>
#include <algorithm>
#include <iostream>

double* getRandomMatrix(double *matrix, double x);

double* getRandomMatrixWithBvector(int* vec_x, double* matrix, int x);

void getDivisionInString(double* strings, double* divider, int num, int x,
    int part);

double* getParallelGausJordan(double* str, int sz);

void* swapString(double* matrixB, int k, int l, int sz);

void PrintMatrix(double* matrix, int sz);
#endif  // MODULES_TASK_2_ERMOLAEV_D_JORDAN_GAUSS_JORDAN_METHOD_H_
