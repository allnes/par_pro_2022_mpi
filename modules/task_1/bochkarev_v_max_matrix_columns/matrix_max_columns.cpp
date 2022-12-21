// Copyright 2022 Bochkarev Vladimir
#include <mpi.h>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include "../../../modules/task_1/bochkarev_v_max_matrix_columns/matrix_max_columns.h"

std::vector<int> getRandomMatrix(int row, int col) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::vector<int> matrix(row*col);
    for (int i = 0; i < row*col; i++)
        matrix[i] = gen() % 100;

    std::vector<int> vec(row*col);
    int k = 0;
    for (int j = 0; j < col; j++) {
        for (int i = 0; i < row; i++) {
            vec[k] = matrix[i*col + j];
            k++;
        }
    }
    return vec;
}

int getSequentialOperations(std::vector<int> vector) {
    const int size = vector.size();
    int max = vector[0];

    for (int i = 0; i < size; i++)
        max = std::max(max, vector[i]);
    return max;
}

std::vector<int> getParallelOperations(std::vector<int> matrix, int count_row, int count_col) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int delta = count_col/size;
    std::vector<int> res(count_col);

    for (int i = 0; i < delta; i++) {
        if (rank == 0) {
            for (int proc = 1; proc < size; proc++) {
                MPI_Send(matrix.data() + proc*count_row + i*size*count_row,
                count_row, MPI_INT, proc, 0, MPI_COMM_WORLD);
            }
        }

        std::vector<int> vector(count_row);
        if (rank == 0) {
            vector = std::vector<int>(matrix.begin() + i*size*count_row, matrix.begin() + count_row + i*size*count_row);
        } else {
            MPI_Status status;
            MPI_Recv(vector.data(), count_row, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        }

        int l_res = getSequentialOperations(vector);

        if (rank == 0) {
            res[i*size] = l_res;
            for (int proc = 1; proc < size; proc++) {
                MPI_Status status1;
                MPI_Recv(res.data() + i*size + proc, 1, MPI_INT, proc, 0, MPI_COMM_WORLD, &status1);
            }
        } else {
            MPI_Send(&l_res, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }

    if ((count_col % size != 0) && (rank < count_col % size)) {
        if (rank == 0) {
            for (int proc = 1; proc < count_col % size; proc++) {
                MPI_Send(matrix.data() + delta*size*count_row + proc*count_row,
                count_row, MPI_INT, proc, 0, MPI_COMM_WORLD);
            }
        }

        std::vector<int> vector(count_row);
        if (rank == 0) {
            vector = std::vector<int>(matrix.begin() + delta*size*count_row,
            matrix.begin() + delta*size*count_row + count_row);
        } else {
            MPI_Status status;
            MPI_Recv(vector.data(), count_row, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        }

        int l_res = getSequentialOperations(vector);

        if (rank == 0) {
            res[delta*size] = l_res;
            for (int proc = 1; proc < count_col % size; proc++) {
                MPI_Status status1;
                MPI_Recv(res.data() + delta*size + proc, 1, MPI_INT, proc, 0, MPI_COMM_WORLD, &status1);
            }
        } else {
            MPI_Send(&l_res, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }

    return res;
}
