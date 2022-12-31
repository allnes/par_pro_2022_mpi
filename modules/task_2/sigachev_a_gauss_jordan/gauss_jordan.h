// Copyright 2022 Sigachev Anton
#ifndef MODULES_TASK_2_SIGACHEV_A_GAUSS_JORDAN_GAUSS_JORDAN_H_
#define MODULES_TASK_2_SIGACHEV_A_GAUSS_JORDAN_GAUSS_JORDAN_H_

int getNumRows(int total, int size, int rank);
double* parallelGaussJordan(int n, int nrows, int* rows, double* a);

#endif  // MODULES_TASK_2_SIGACHEV_A_GAUSS_JORDAN_GAUSS_JORDAN_H_
