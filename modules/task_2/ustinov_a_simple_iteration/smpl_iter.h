// Copyright 2022 Ustinov A.
#ifndef MODULES_TASK_2_USTINOV_A_SIMPLE_ITERATION_SMPL_ITER_H_
#define MODULES_TASK_2_USTINOV_A_SIMPLE_ITERATION_SMPL_ITER_H_

#include <vector>

std::vector<double> simple_iteration_method_parallel(
        const std::vector<double> &A_vector,
        const std::vector<double> &b_vector,
        int n,
        int max_iter_num,
        double epsilon);
std::vector<double> simple_iteration_method_sequential(
        const std::vector<double> &A_vector,
        const std::vector<double> &b_vector,
        int n,
        int max_iter_num,
        double epsilon);
std::vector<double> matrix_vector_product(
        const std::vector<double> &A_vector,
        const std::vector<double> &b_vector,
        int n);
std::vector<double> get_random_vector(size_t n);
std::vector<double> get_random_stable_matrix(size_t n);
#endif  // MODULES_TASK_2_USTINOV_A_SIMPLE_ITERATION_SMPL_ITER_H_
