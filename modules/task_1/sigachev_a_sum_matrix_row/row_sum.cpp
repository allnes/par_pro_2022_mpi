// Copyright 2022 Sigachev Anton
#include <mpi.h>
#include <random>
#include <algorithm>
#include "../../../modules/task_1/sigachev_a_sum_matrix_row/row_sum.h"

int* getRandomMatrix(int rows, int cols) {
    if (rows <= 0 || cols <= 0) {
        throw - 1;
    }
    std::random_device dev;
    std::mt19937 gen(dev());
    int* matrix = new int[rows * cols];
    for (int i = 0; i < rows * cols; i++) { matrix[i] = gen() % 100; }
    return matrix;
}

int* getSequentialOperations(int* matrix, int rows, int cols) {
    if (rows <= 0 || cols <= 0) {
        throw - 1;
    }
    int* sums = new int[rows];
    for (int i = 0; i < rows; i++) {
        sums[i] = 0;
    }
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            sums[i] += matrix[i * cols + j];
        }
    }
    return sums;
}

int *getParallelOperations(int* matrix, int rows, int cols) {
    if (rows <= 0 || cols <= 0) {
        throw - 1;
    }
    int size, rank;
    int* buffer = nullptr;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rows < size) {
        throw - 1;
    }
    const int delta = rows % size;
    const int buffer_size = rows / size;

    buffer = new int[cols * buffer_size];
    MPI_Scatter(matrix, cols * buffer_size, MPI_INT,
        buffer, cols * buffer_size, MPI_INT, 0, MPI_COMM_WORLD);

    int *local_sum = nullptr, *global_sum = nullptr;

    local_sum = getSequentialOperations(buffer, buffer_size, cols);

    if (rank == 0) {
        global_sum = new int[rows];
        for (int i = 0; i < rows; i++) {
            global_sum[i] = 0;
        }
    }

    MPI_Gather(local_sum, buffer_size, MPI_INT,
        global_sum, buffer_size, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        for (int i = 0; i < delta; i++) {
            for (int j = 0; j < cols; j++) {
                global_sum[buffer_size * size + i] +=
                    matrix[(buffer_size * size + i) * cols + j];
            }
        }
    }

    return global_sum;
}
