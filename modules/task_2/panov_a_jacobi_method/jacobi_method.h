// Copyright 2022 Panov Alexey
#ifndef MODULES_TASK_2_PANOV_A_JACOBI_METHOD_JACOBI_METHOD_H_
#define MODULES_TASK_2_PANOV_A_JACOBI_METHOD_JACOBI_METHOD_H_

#include <vector>

using Vector = std::vector<double>;
using Matrix = std::vector<Vector>;

const double EPSILON = 0.00001;

Vector calculateJacobiSequentially(
    const Matrix& A,
    const Vector& b
);

Vector calculateJacobiParallel(
    const Matrix& A,
    const Vector& b
);

#endif  // MODULES_TASK_2_PANOV_A_JACOBI_METHOD_JACOBI_METHOD_H_
