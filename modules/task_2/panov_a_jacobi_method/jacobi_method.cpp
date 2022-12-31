// Copyright 2022 Panov Alexey
#include <mpi.h>
#include <cmath>
#include <iostream>
#include <algorithm>
#include "../../../modules/task_2/panov_a_jacobi_method/jacobi_method.h"

double getDiffVectorNorm(const Vector& x, const Vector& tempX) {
    double norm = std::abs(x[0] - tempX[0]);
    for (int i = 1; i < x.size(); i++) {
        if (std::abs(x[i] - tempX[i]) > norm)
            norm = std::abs(x[i] - tempX[i]);
    }
    return norm;
}

Vector calculateJacobiSequentially(
    const Matrix& A,
    const Vector& b
) {
    const int n = A.size();
    Vector x = Vector(n, .0);

    double norm = .0;

    do {
        Vector tempX = b;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (i != j) tempX[i] -= A[i][j] * x[j];
            }
            tempX[i] /= A[i][i];
        }

        norm = getDiffVectorNorm(x, tempX);
        x = tempX;
    } while (norm > EPSILON);

    return x;
}

Vector calculateJacobiParallel(
    const Matrix& A,
    const Vector& b
) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int n = A.size();
    Vector x = Vector(n, .0);
    double norm = .0;

    const int step = (n / size) + ((n % size) != 0);

    const int nWithShift = step * size;
    const int localFrom = step * rank;
    const int localTo = std::min(step * (rank + 1), n);

    MPI_Bcast(x.data(), x.size(), MPI_DOUBLE, 0, MPI_COMM_WORLD);

    do {
        Vector tempX = b;
        tempX.resize(nWithShift);

        for (int i = localFrom; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (i != j) tempX[i] -= A[i][j] * x[j];
            }
            tempX[i] /= A[i][i];
        }

        Vector nextX(nWithShift);
        MPI_Allgather(tempX.data() + localFrom, step, MPI_DOUBLE, nextX.data(), step, MPI_DOUBLE, MPI_COMM_WORLD);

        if (rank == 0) {
            norm = getDiffVectorNorm(x, tempX);
        }

        MPI_Bcast(&norm, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        nextX.resize(n);
        x = nextX;
    } while (norm > EPSILON);

    return x;
}
