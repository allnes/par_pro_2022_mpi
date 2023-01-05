// Copyright 2022 Mitin Roman
#ifndef MODULES_TASK_1_MITIN_R_INTEGR_MON_CARL_INTEGR_MON_CARL_H_
#define MODULES_TASK_1_MITIN_R_INTEGR_MON_CARL_INTEGR_MON_CARL_H_

#include <mpi.h>
#include <cinttypes>
#include <utility>

std::pair<double, double> get_rand_point(double a, double b, double h_min, double h_max);

template<typename func_t>
int64_t integrate_monte_carlo_seq(func_t func, double a, double b, double h_max, int64_t num_points) {
    int64_t count_under_points = 0;

    for (int64_t i = 0; i < num_points; i++) {
        auto point = get_rand_point(a, b, 0, h_max);
        if (func(point.first) > point.second) {
            count_under_points++;
        }
    }

    // double rectangle_area = (b - a) * h_max;
    // double result = static_cast<double>(count_under_points) / num_points * rectangle_area;

    return count_under_points;
}

template<typename func_t>
double integrate_monte_carlo(func_t func, double a, double b, double h_max, int64_t num_points) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    num_points = ((num_points + size - 1) / size) * size;

    int64_t local_points_count =
        integrate_monte_carlo_seq(func, a, b, h_max, num_points / size);

    int64_t global_points_count;

    MPI_Allreduce(&local_points_count, &global_points_count, 1,
        MPI_LONG_LONG, MPI_SUM, MPI_COMM_WORLD);


    double rect_area = (b - a) * h_max;
    double result = static_cast<double>(global_points_count)/ num_points * rect_area;

    return result;
}


#endif  // MODULES_TASK_1_MITIN_R_INTEGR_MON_CARL_INTEGR_MON_CARL_H_
