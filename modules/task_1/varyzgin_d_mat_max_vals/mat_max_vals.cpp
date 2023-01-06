// Copyright 2023 Varyzgin Dmitriy
#include "../../../modules/task_1/varyzgin_d_mat_max_vals/mat_max_vals.h"

#include <mpi.h>

#include <algorithm>
#include <iostream>
#include <random>
#include <string>
#include <vector>

std::vector<int> genRandomMatrix(int m, int n) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::vector<int> matrix(m * n);
    for (int i = 0; i < m * n; i++) matrix[i] = gen() % 10000;
    return matrix;
}

std::vector<int> getMaxValsSequential(int m, int n,
                                      const std::vector<int>& matrix) {
    std::vector<int> res;

    for (int i = 0; i < m; i++) {
        int max = matrix[i * n];
        for (int j = 0; j < n; j++)
            if (matrix[i * n + j] > max) max = matrix[i * n + j];
        res.push_back(max);
    }

    return res;
}

std::vector<int> getMaxValsParallel(int m, int n,
                                    const std::vector<int>& matrix) {
    int comm_size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int processes = std::min(m, comm_size);
    // Every working process computes tasks tasks except rank = 0
    // (it computes (task + other) tasks)
    const int tasks = m / processes;
    const int other = m % processes;

    // Sending data to other processes
    if (rank == 0) {
        for (int r = 1; r < processes; r++)
            MPI_Send(matrix.data() + n * (tasks + other) + (r - 1) * n * tasks,
                     n * tasks, MPI_INT, r, 0, MPI_COMM_WORLD);
    }
    std::vector<int> receivedMatPart(tasks * n);
    std::vector<int> locRes;
    std::vector<int> globRes(m);

    if (rank == 0) {
        receivedMatPart = std::vector<int>(
            matrix.begin(), matrix.begin() + (tasks + other) * n);

        locRes = getMaxValsSequential(tasks + other, n, receivedMatPart);

        for (size_t i = 0; i < locRes.size(); i++) globRes[i] = locRes[i];

        for (int i = 1; i < processes; i++) {
            MPI_Status status;
            MPI_Recv(globRes.data() + other + tasks * i, tasks, MPI_INT, i, i,
                     MPI_COMM_WORLD, &status);
        }

    } else if (rank < processes) {
        MPI_Status status;
        MPI_Recv(receivedMatPart.data(), tasks * n, MPI_INT, 0, 0,
                 MPI_COMM_WORLD, &status);

        locRes = getMaxValsSequential(tasks, n, receivedMatPart);

        MPI_Send(locRes.data(), tasks, MPI_INT, 0, rank, MPI_COMM_WORLD);
    }

    return globRes;
}
