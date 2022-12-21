// Copyright 2022 Ermolaev Danila
#include <mpi.h>
#include <vector>
#include <string>
#include <numeric>
#include <random>
#include <algorithm>
#include <iostream>
#include "../../../modules/task_2/ermolaev_d_gauss_Jordan_method/gauss_Jordan_method.h"

double* getRandomMatrix(double* matrix, double x) {
    std::random_device dev;
    std::mt19937 gen(dev());
    for (int i = 0; i < x; i++) {
        matrix[i] = 0;
    }

    for (int i = 0; i < x; i++) {
        matrix[i] = gen() % 10 + 1;
    }

    return matrix;
}

double* getRandomMatrixWithBvector(int* vec_x, double* matrix, int x) {
    std::random_device dev;
    std::mt19937 gen(dev());

    double* MatrixB = new double[x * (x + 1)];
    double* sum = new double[x];

    for (int i = 0; i < x; i++) {
        for (int j = 0; j < x + 1; j++) {
            MatrixB[i * x + j] = 0;
        }
        sum[i] = 0;
    }

    for (int i = 0; i < x; i++) {
        for (int j = 0; j < x; j++) {
            MatrixB[i * (x + 1) + j] = matrix[i * x + j];
            sum[i] += matrix[i * x + j] * vec_x[j];
        }
        MatrixB[(i + 1) * (x + 1) - 1] = sum[i];
    }

    return MatrixB;
}

void getDivisionInString(double* line, double* divider, int num, int x,
    int local_part_size) {
    for (int i = 0; i < local_part_size; i++) {
        double coeff = line[i * (x + 1) + num] / divider[num];
        for (int j = 0; j < x + 1; j++) {
            line[i * (x + 1) + j] -= coeff * divider[j];
            if (std::abs(line[i * (x + 1) + j]) <= 1e-9) line[i * (x + 1) + j] = 0;
        }
    }
}

double* getParallelGausJordan(double* matrix, int x) {
    int root = 0, rank = 0, col_proc = 0;

    MPI_Comm_size(MPI_COMM_WORLD, &col_proc);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int part_size = 0;
    int num_parts = 0;
    int* matrix_sendcounts = nullptr;
    int* matrix_displs = nullptr;

    if (rank == root) {
        part_size = x / col_proc;
        num_parts = x % col_proc;
        matrix_sendcounts = new int[col_proc];
        matrix_displs = new int[col_proc];

        matrix_displs[0] = 0;

        int q;
        if (0 != num_parts)
            q = part_size + 1;
        else
            q = part_size;
        matrix_sendcounts[0] = q * (x + 1);

        for (int i = 1; i < col_proc; i++) {
            if (i < num_parts)
                q = part_size + 1;
            else
                q = part_size;

            matrix_sendcounts[i] = q * (x + 1);

            matrix_displs[i] = matrix_displs[i - 1] + matrix_sendcounts[i - 1];
        }
    }
    MPI_Bcast(&part_size, 1, MPI_INT, root, MPI_COMM_WORLD);
    MPI_Bcast(&num_parts, 1, MPI_INT, root, MPI_COMM_WORLD);

    int local_part_size;

    if (rank < num_parts) {
        local_part_size = part_size + 1;
    } else {
        local_part_size = part_size;
    }

    double* buffer = new double[local_part_size * (x + 1)];

    double* divider = new double[x + 1];

    for (int i = 0; i < x; i++) {
        if (rank == root) {
            int k = i;
            for (int j = 0; j < x; j++) {
                if ((std::abs(matrix[k + (x + 1) * k]) <
                    std::abs(matrix[k + (x + 1) * j])) &&
                    (j > k))
                    swapString(matrix, k, j, x);
            }

            std::copy(matrix + (i * (x + 1)), matrix + (i + 1) * (x + 1), divider);
            MPI_Bcast(divider, x + 1, MPI_DOUBLE, root, MPI_COMM_WORLD);

        } else {
            MPI_Bcast(divider, x + 1, MPI_DOUBLE, root, MPI_COMM_WORLD);
        }

        MPI_Scatterv(matrix, matrix_sendcounts, matrix_displs, MPI_DOUBLE, buffer,
            local_part_size * (x + 1), MPI_DOUBLE, root, MPI_COMM_WORLD);

        getDivisionInString(buffer, divider, i, x, local_part_size);

        MPI_Gatherv(buffer, local_part_size * (x + 1), MPI_DOUBLE, matrix,
            matrix_sendcounts, matrix_displs, MPI_DOUBLE, root,
            MPI_COMM_WORLD);

        for (int i = 0; i < local_part_size; i++) {
            for (int j = 0; j < x; j++) {
                buffer[i * (x + 1) + j] = 0;
            }
            buffer[(i + 1) * (x + 1) - 1] = 0;
        }

        if (rank == root) {
            std::copy(divider, divider + x + 1, matrix + (i * (x + 1)));
        }
    }

    double* res = new double[x];

    if (rank == root) {
        for (int i = 0; i < x; i++) {
            res[i] = matrix[(i + 1) * (x + 1) - 1] / matrix[i * (x + 1) + i];
        }
    }

    delete[] matrix_sendcounts;
    delete[] matrix_displs;
    delete[] buffer;
    delete[] divider;

    return res;
}

void* swapString(double* matrixB, int k, int l, int x) {
    for (int i = 0; i < x + 1; i++) {
        std::swap(matrixB[(x + 1) * k + i], matrixB[l * (x + 1) + i]);
    }
    return nullptr;
}

void PrintMatrix(double* matrix, int x) {
    for (int i = 0; i < x; i++) {
        for (int j = 0; j < x; j++) {
            std::cout << matrix[i * (x + 1) + j] << "\t\t";
        }
        std::cout << matrix[(i + 1) * (x + 1) - 1] << "\n";
    }
}
