  // Copyright 2022 Ermolaev Danila
#include <mpi.h>
#include <vector>
#include <string>
#include <numeric>
#include <random>
#include <algorithm>
#include <iostream>
#include "../../../modules/task_1/ermolaev_d_val_rows_matrix_sum/val_rows_matrix_sum.h"

int* getRandomMatrix(int x, int y) {
    std::mt19937 mt(time(nullptr));
    int* matrix = new int[x * y];

    for (int i = 0; i < x * y; i++) {
        matrix[i] = mt() % 100;
    }

    return matrix;
}

void getSequentialOperation(int* matrix, int* result, int x, int y) {
    for (int j = 0; j < y; j++) {
        result[j] = 0;
    }

    for (int j = 0; j < y; j++) {
        for (int i = 0; i < x; i++) {
            result[j] += matrix[x * j + i];
        }
    }
}

void getParallelOperation(int* matrix, int* result, int x, int y) {
    int* local_matrix;
    int* local_result;
    int id, col_proc;
    MPI_Status status;

    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &col_proc);

    int data_in_process;
    if (y / col_proc == 0)
        data_in_process = 1;
    else
        data_in_process = y / col_proc;

    int iteration;
    if (col_proc <= y)
        iteration = col_proc;
    else
        iteration = y;

    if (id == 0) {
        for (int i = 1; i < iteration; ++i) {
            MPI_Send(matrix + x * i * data_in_process, x * data_in_process, MPI_INT,
                i, 0, MPI_COMM_WORLD);
        }
        for (int i = 1; i < iteration; ++i) {
            MPI_Recv(result + i * data_in_process, data_in_process, MPI_INT, i, 0,
                MPI_COMM_WORLD, &status);
        }

        for (int i = 0; i < data_in_process; ++i) {
            result[i] =
                (std::accumulate(matrix + x * i, matrix + x * (i + 1), result[i]));
        }
        for (int i = data_in_process * col_proc; i < y; ++i) {
            result[i] =
                (std::accumulate(matrix + x * i, matrix + x * (i + 1), result[i]));
        }
    } else if (id < y) {
        local_matrix = new int[x * data_in_process];
        for (int i = 0; i < x * data_in_process; ++i) {
            local_matrix[i] = 0;
        }

        local_result = new int[data_in_process];
        for (int i = 0; i < data_in_process; ++i) {
            local_result[i] = 0;
        }
        MPI_Recv(local_matrix, x * data_in_process, MPI_INT, 0, 0, MPI_COMM_WORLD,
            &status);
        for (int i = 0; i < data_in_process; ++i) {
            local_result[i] = (std::accumulate(
                local_matrix + x * i, local_matrix + x * (i + 1), local_result[i]));
        }
        MPI_Send(local_result, data_in_process, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
}
