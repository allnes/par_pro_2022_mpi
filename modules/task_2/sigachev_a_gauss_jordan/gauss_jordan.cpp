// Copyright 2022 Sigachev Anton
#include <mpi.h>
#include <ctime>
#include <numeric>
#include <iostream>
#include <cstring>
#include "../../../modules/task_2/sigachev_a_gauss_jordan/gauss_jordan.h"

int getNumRows(int total, int size, int rank) {
    int size_mtx = total;
    int q = size_mtx / size;
    if (size_mtx % size)
        q++;
    int r = size * q - size_mtx;
    int chunk = q;
    if (rank >= size - r)
        chunk = q - 1;
    return chunk;
}

double* parallelGaussJordan(int size_mtx, int nums_rank, int* rows, double* a) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    double* x = new double[sizeof(double) * size_mtx];
    double* tmp = new double[sizeof(double) * (size_mtx + 1)];
    int row = 0;
    for (int i = 0; i < size_mtx - 1; i++) {
        if (i == rows[row]) {
            int k = row * (size_mtx + 1);
            MPI_Bcast(&a[k], size_mtx + 1, MPI_DOUBLE, rank, MPI_COMM_WORLD);
            for (int j = 0; j <= size_mtx; j++)
                tmp[j] = a[k + j];
            row++;
        } else {
            MPI_Bcast(tmp, size_mtx + 1, MPI_DOUBLE, i % size, MPI_COMM_WORLD);
        }
        for (int j = row; j < nums_rank; j++) {
            double scaling = a[j * (size_mtx + 1) + i] / tmp[i];
            for (int k = i; k < size_mtx + 1; k++)
                a[j * (size_mtx + 1) + k] -= scaling * tmp[k];
        }
    }
    row = 0;
    for (int i = 0; i < size_mtx; i++) {
        x[i] = 0;
        if (i == rows[row]) {
            x[i] = a[row * (size_mtx + 1) + size_mtx];
            row++;
        }
    }
    row = nums_rank - 1;
    for (int i = size_mtx - 1; i > 0; i--) {
        if (row >= 0) {
            if (i == rows[row]) {
                x[i] /= a[row * (size_mtx + 1) + i];
                MPI_Bcast(&x[i], 1, MPI_DOUBLE, rank, MPI_COMM_WORLD);
                row--;
            } else {
                MPI_Bcast(&x[i], 1, MPI_DOUBLE, i % size, MPI_COMM_WORLD);
            }
        } else {
            MPI_Bcast(&x[i], 1, MPI_DOUBLE, i % size, MPI_COMM_WORLD);
        }
        for (int j = i; j <= row; j++)
            x[rows[j]] -= a[j * (size_mtx + 1) + i] * x[i];
    }
    if (rank == 0)
        x[0] /= a[row * (size_mtx + 1)];

    MPI_Bcast(x, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    return x;
}
