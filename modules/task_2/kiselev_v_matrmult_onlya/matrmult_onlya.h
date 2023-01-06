// Copyright 2023 Kiselev Vladislav
#ifndef MODULES_TASK_2_KISELEV_V_MATRMULT_ONLYA_MATRMULT_ONLYA_H_
#define MODULES_TASK_2_KISELEV_V_MATRMULT_ONLYA_MATRMULT_ONLYA_H_

#include <vector>

std::vector<int> GenRandMatrix(int m, int n);

std::vector<int> MultSequential(const std::vector<int>& MatrixA,
                                const std::vector<int>& MatrixB, int m1, int n1,
                                int m2, int n2);
std::vector<int> MultParallel(const std::vector<int>& MatrixA,
                              const std::vector<int>& MatrixB, int m1, int n1,
                              int m2, int n2);

#endif  // MODULES_TASK_2_KISELEV_V_MATRMULT_ONLYA_MATRMULT_ONLYA_H_
