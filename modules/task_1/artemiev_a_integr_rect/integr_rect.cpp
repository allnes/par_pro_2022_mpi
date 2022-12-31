// Copyright 2022 Artemiev Aleksey
#include "../../../modules/task_1/artemiev_a_integr_rect/integr_rect.h"
#include <mpi.h>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <random>
#include <string>
#include <vector>

double integrateSequential(double (*f)(double), double a, double b, int n) {
    double integralValue = 0.0;

    double h = (b - a) / (static_cast<double>(n));
    double h_half = h / 2;

    int i = 0;
    for (double x = a; i < n; x += h, i++) {
        integralValue += f(x + h_half);
        // std::cout << "seq: x = " << x << '\n';
    }
    integralValue *= h;

    return integralValue;
}

double integrateParallel(double (*f)(double), double a, double b, int n) {
    int comm_size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int actingProcessesCount = std::min(comm_size, n / 2);

    const int q = n / actingProcessesCount;
    const int rem = n % actingProcessesCount;

    double h = (b - a) / (static_cast<double>(n));
    double h_half = h / 2;

    double integralValue = 0.0;
    double localPart = 0.0;

    if (rank < actingProcessesCount) {
        // Every acting process counts sum of square of q rectangles
        // (unused processes has localPart = 0)
        double x = a + (q * h) * rank;
        for (int i = 0; i < q; i++, x += h) {
            localPart += f(x + h_half);
            // std::cout << '\n' << rank << ": x = " << x << '\n';
        }

        // Root process also adds remained part of sum
        if (rank == 0) {
            double x = a + (q * h) * actingProcessesCount;
            for (int i = 0; i < rem; i++, x += h) {
                localPart += f(x + h_half);
                // std::cout << '\n' << rank << ": x = " << x << '\n';
            }
        }
    }

    MPI_Reduce(&localPart, &integralValue, 1, MPI_DOUBLE, MPI_SUM, 0,
               MPI_COMM_WORLD);

    if (rank == 0) integralValue *= h;
    return integralValue;
}
