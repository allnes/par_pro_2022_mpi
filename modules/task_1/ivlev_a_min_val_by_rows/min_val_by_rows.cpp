  // Copyright 2022 Ivlev A
#include <mpi.h>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include "../../../modules/task_1/ivlev_a_min_val_by_rows/min_val_by_rows.h"


int* getRandomMatrix(int m, int n) {
    std::random_device dev;
    std::mt19937 gen(dev());

    int* matrix = new int[m*n];

    for (int i = 0; i < m*n; i++) {
        matrix[i] = gen() % 100;
    }

    return matrix;
}

int min_in_vec(int* vec, int size) {
    if (vec == nullptr) {
        return 0;
    }

    int min_elem = vec[0];

    for (int i = 1; i < size; i++) {
        min_elem = std::min(min_elem, vec[i]);
    }

    return min_elem;
}

int* getMatrixMinbyRow(int* global_matrix, int row_num, int column_num) {
    int* global_min = new int[row_num];

    for (int i = 0; i < row_num; i++) {
        global_min[i] = min_in_vec(global_matrix+i*column_num, column_num);
    }

    return global_min;
}

int* getParallelMin(int* global_matrix, int row_num, int column_num) {
    int size, rank;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int block_size = row_num/size;
    int block_overflow = row_num%size;
    int block_size_all, delta;
    int* vec_row = nullptr;
    int* global_min = nullptr;

    if (rank == 0) {
        for (int proc = 1; proc < size; proc++) {
            if (proc < block_overflow) {
                delta = proc * (block_size+1) * column_num;
                block_size_all = (block_size+1)*column_num;
            } else {
                delta = (block_overflow * (block_size+1)
                        + (proc - block_overflow) * (block_size)) * column_num;
                block_size_all = (block_size)*column_num;
            }

            MPI_Send(global_matrix + delta, block_size_all, MPI_INT, proc,
                    0, MPI_COMM_WORLD);
        }

        if (block_overflow != 0) {
            delta = (block_size+1)*column_num;
        } else {
            delta = (block_size)*column_num;
        }
        vec_row = new int[delta];
        for (int i = 0; i < delta; i++) {
           vec_row[i] = global_matrix[i];
        }

        global_min = new int[row_num];
        int rows = delta/column_num;
        int counts = 0;

        for (int i = 0; i < rows; i++) {
            global_min[i] = min_in_vec(vec_row + i*column_num, column_num);
            counts++;
        }
        delete [] vec_row;

        for (int i = 1; i < size; i++) {
            int count;
            MPI_Status status;
            MPI_Probe(i, i, MPI_COMM_WORLD, &status);
            MPI_Get_count(&status, MPI_INT, &count);
            int* min_row = new int[count];

            MPI_Recv(min_row, count, MPI_INT, i, i, MPI_COMM_WORLD, &status);
            for (int j = 0; j < count; j++) {
                global_min[counts] = min_row[j];
                counts++;
            }

            delete [] min_row;
        }

    } else {
        int count;
        MPI_Status status;
        MPI_Probe(0, 0, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_INT, &count);
        vec_row = new int[count];
        MPI_Recv(vec_row, count, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

        int rows = count/column_num;
        int* local_min = new int[rows];
        for (int i = 0; i < rows; i++) {
            local_min[i] = min_in_vec(vec_row + i*column_num, column_num);
        }
        MPI_Send(local_min, rows, MPI_INT, 0, rank, MPI_COMM_WORLD);

        delete [] local_min;
    }

    return global_min;
}
