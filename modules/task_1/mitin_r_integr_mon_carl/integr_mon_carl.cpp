// Copyright 2022 Mitin Roman
#include "../../modules/task_1/mitin_r_integr_mon_carl/integr_mon_carl.h"
#include <random>

std::pair<double, double> get_rand_point(double a, double b, double h_min, double h_max) {
    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    static std::random_device device;
    static std::mt19937_64 r(device() * rank);

    std::uniform_real_distribution<double> uniform_dist1(a, b);
    std::uniform_real_distribution<double> uniform_dist2(h_min, h_max);

    std::pair<double, double> point;
    point.first = uniform_dist1(r);
    point.second = uniform_dist2(r);

    return point;
}

