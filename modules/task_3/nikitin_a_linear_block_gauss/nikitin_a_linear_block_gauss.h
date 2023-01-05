// Copyright 2022 Nikitin Alexander
#ifndef MODULES_TASK_3_NIKITIN_A_LINEAR_BLOCK_GAUSS_NIKITIN_A_LINEAR_BLOCK_GAUSS_H_
#define MODULES_TASK_3_NIKITIN_A_LINEAR_BLOCK_GAUSS_NIKITIN_A_LINEAR_BLOCK_GAUSS_H_

#include <vector>
using std::vector;
void gaussCore();

vector<double> randomMatrix(int rows, int columns);
vector<double> operationSequential(const vector<double>& image, int rows,
                                   int columns);
vector<double> operationParallel(const vector<double>& image, int matrix_size);

#endif  // MODULES_TASK_3_NIKITIN_A_LINEAR_BLOCK_GAUSS_NIKITIN_A_LINEAR_BLOCK_GAUSS_H_
