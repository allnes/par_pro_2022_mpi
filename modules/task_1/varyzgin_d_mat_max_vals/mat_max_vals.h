// Copyright 2023 Varyzgin Dmitriy
#ifndef MODULES_TASK_1_VARYZGIN_D_MAT_MAX_VALS_MAT_MAX_VALS_H_
#define MODULES_TASK_1_VARYZGIN_D_MAT_MAX_VALS_MAT_MAX_VALS_H_

#include <vector>

std::vector<int> genRandomMatrix(int m, int n);

std::vector<int> getMaxValsSequential(int m, int n,
                                      const std::vector<int>& matrix);

std::vector<int> getMaxValsParallel(int m, int n,
                                    const std::vector<int>& matrix);

void printVecElements(const std::vector<int>& vec);

#endif  // MODULES_TASK_1_VARYZGIN_D_MAT_MAX_VALS_MAT_MAX_VALS_H_
