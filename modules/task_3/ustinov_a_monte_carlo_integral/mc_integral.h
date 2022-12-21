// Copyright 2022 Ustinov A.
#ifndef MODULES_TASK_3_USTINOV_A_MONTE_CARLO_INTEGRAL_MC_INTEGRAL_H_
#define MODULES_TASK_3_USTINOV_A_MONTE_CARLO_INTEGRAL_MC_INTEGRAL_H_

#include <mpi.h>
#include <cstdint>
#include <array>
#include <functional>
#include <random>

template <size_t dim>
double multiple_integral_monte_carlo_parallel(
        const std::function<double(std::array<double, dim>)> &f,
        const std::function<bool(std::array<double, dim>)> &in_region,
        const std::array<double[2], dim> &rect,
        int64_t n) {
    int rank;  // number of current process
    int size;  // total number of processes
    int64_t n_per_process;  // number of iterations for current process
    double local_sum = 0.0;  // answer in each process
    double total_sum = 0.0;  // total answer is a sum of local answers
    double multiplier = 1/static_cast<double>(n);  // multiplier in sum
    double current_value;    // term to add to 'local_sum'
    double corrected_value;  // 'current_value' corrected by 'error_sum'
    double error_sum = 0.0;  // current sum of errors of computations
    double corrected_sum;    // 'local_sum' + 'corrected_value'
    std::array<double, dim> random_point;  // random point in rectangle
    std::mt19937_64 generator{std::random_device {}()};
    std::uniform_real_distribution<double> distribution(0.0, 1.0);

    for (size_t i = 0; i < dim; ++i)
        multiplier *= (rect[i][1] - rect[i][0]);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    n_per_process = ((rank + 1) * n) / size - (rank * n) / size;

    for (int64_t it = 0; it < n_per_process; ++it) {
        for (size_t i = 0; i < dim; ++i)
            random_point[i] = rect[i][0] +  // carried
            (rect[i][1] - rect[i][0]) * distribution(generator);
        if (in_region(random_point)) {
            // Cahan's algorithm to increace answer's precision
            current_value = multiplier * f(random_point);
            corrected_value = current_value - error_sum;
            corrected_sum = local_sum + corrected_value;
            error_sum = (corrected_sum - local_sum) - corrected_value;
            local_sum = corrected_sum;
            // local_sum += multiplier * f(random_point);
        }
    }
    MPI_Reduce(&local_sum, &total_sum, 1, MPI_DOUBLE,  // carried
               MPI_SUM, 0, MPI_COMM_WORLD);
    return total_sum;
}
template <size_t dim>
double multiple_integral_monte_carlo_sequential(
        const std::function<double(std::array<double, dim>)> &f,
        const std::function<bool(std::array<double, dim>)> &in_region,
        const std::array<double[2], dim> &rect,
        int64_t n) {
    double total_sum = 0.0;  // result of computations
    double multiplier = 1/static_cast<double>(n);  // multiplier in sum
    double current_value;    // term to add to 'total_sum'
    double corrected_value;  // 'current_value' corrected by 'error_sum'
    double error_sum = 0.0;  // current sum of errors of computations
    double corrected_sum;    // 'total_sum' + 'corrected_value'
    std::array<double, dim> random_point;  // random point in rectangle
    std::mt19937_64 generator{std::random_device {}()};  // RNG
    std::uniform_real_distribution<double> distribution(0.0, 1.0);

    for (size_t i = 0; i < dim; ++i)
        multiplier *= (rect[i][1] - rect[i][0]);

    for (int64_t it = 0; it < n; ++it) {
        for (size_t i = 0; i < dim; ++i)
            random_point[i] = rect[i][0] +  // carried
            (rect[i][1] - rect[i][0]) * distribution(generator);
        if (in_region(random_point)) {
            // Cahan's algorithm to increace answer's precision
            current_value = multiplier * f(random_point);
            corrected_value = current_value - error_sum;
            corrected_sum = total_sum + corrected_value;
            error_sum = (corrected_sum - total_sum) - corrected_value;
            total_sum = corrected_sum;
        }
    }

    return total_sum;
}

#endif  // MODULES_TASK_3_USTINOV_A_MONTE_CARLO_INTEGRAL_MC_INTEGRAL_H_
