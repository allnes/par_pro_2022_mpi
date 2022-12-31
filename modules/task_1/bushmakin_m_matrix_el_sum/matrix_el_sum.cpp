// Copyright 2022 Bushmakin Maksim
#include "../../../modules/task_1/bushmakin_m_matrix_el_sum/matrix_el_sum.h"
#include <mpi.h>
#include <algorithm>
#include <iostream>
#include <random>
#include <string>
#include <vector>

std::vector<int> genRandomIntMatrix(int m, int n) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::vector<int> matrix(m * n);
    for (int i = 0; i < m * n; i++) {
        matrix[i] = gen() % 1000;
    }
    return matrix;
}

int sequentialMatSum(const std::vector<int>& matrix, int m, int n) {
    int sum = 0;
    for (int i = 0; i < m * n; i++) sum += matrix[i];
    return sum;
}

int parallelMatSum(const std::vector<int>& matrix, int m, int n) {
    int comm_size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int totalWorkersCount = std::min(m, comm_size);
    const int rowsPerProcess = m / totalWorkersCount;
    const int rowsForRootProcess = rowsPerProcess + (m % totalWorkersCount);

    std::vector<int> localRows(rowsPerProcess * n);
    int localSum = 0;

    int globalSum = 0;

    if (rank < totalWorkersCount) {
        if (rank == 0) {
            for (int rankk = 1; rankk < totalWorkersCount; rankk++) {
                MPI_Send(matrix.data() + (rankk - 1) * (rowsPerProcess * n),
                         rowsPerProcess * n, MPI_INT, rankk, 0, MPI_COMM_WORLD);
            }

            localRows = std::vector<int>(
                matrix.begin() + (totalWorkersCount - 1) * (rowsPerProcess * n),
                matrix.end());

            localSum = sequentialMatSum(localRows, rowsForRootProcess, n);
        } else {
            MPI_Status stat;
            MPI_Recv(localRows.data(), rowsPerProcess * n, MPI_INT, 0, 0,
                     MPI_COMM_WORLD, &stat);

            localSum = sequentialMatSum(localRows, rowsPerProcess, n);
        }
    }

    MPI_Reduce(&localSum, &globalSum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    return globalSum;
}
