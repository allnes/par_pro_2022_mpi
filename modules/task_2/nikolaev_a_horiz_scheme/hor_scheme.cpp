// Copyright 2018 Nesterov Alexander
#include <mpi.h>
#include <vector>
#include <string>
#include <iostream>
#include <random>
#include <algorithm>
#include "../../../modules/task_2/nikolaev_a_horiz_scheme/hor_scheme.h"

std::vector<int> getRandomMatrix(int n, int m) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::vector<int> matrix(n*m);

    for (int i = 0; i < n * m; i++) {
        matrix[i] = static_cast<int>(gen() % 100);
    }
    return matrix;
}

std::vector<int> getRandomVector(int vecSize) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::vector<int> vec(vecSize);
    for (int i = 0; i < vecSize; i++) {
        vec[i] = static_cast<int>(gen() % 100);
    }
    return vec;
}

std::vector<int> getMultVectorSequential(const std::vector<int>& pMatrix,
                                            const std::vector<int>& pVector, int n, int m) {
    std::vector<int> pResult(n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            pResult[i] += pMatrix[i * m + j] * pVector[j];
        }
    }
    return pResult;
}

void printMatrix(std::vector<int> vec) {
    const int size = vec.size();
    for (int i = 0; i < size; i++) {
        std::cout << vec[i] << " ";
    }
    std::cout << std::endl;
}

std::vector<int> getMultVectorParallel(const std::vector<int>& matrix, const std::vector<int>& vec, int n, int m) {
    std::vector<int> global_vec(n);
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int delta = n / size;
    int* constant = new int[size];
    int* displs = new int[size];
    int* recvcounts = new int[size];
    if (size >= n) {
        for (int i = 0; i < size; i++) {
            constant[i] = m;
            recvcounts[i] = 1;
            displs[i] = 1 * i;
        }
    } else {
        int tmp = 0;
        for (int i = 0; i < size; i++) {
            if (i != size - 1) {
                constant[i] = delta * m;
                tmp += constant[i];
                recvcounts[i] = constant[i] / m;
                displs[i] = i * recvcounts[i];
            } else {
                constant[i] = n * m - tmp;
                recvcounts[i] = constant[i] / m;
                displs[i] = i * recvcounts[i - 1];
            }
        }
    }
    if (rank == 0) {
        for (int proc = 1; proc < size; proc++) {
            MPI_Send(matrix.data() + proc * constant[proc - 1], constant[proc], MPI_INT, proc, 0, MPI_COMM_WORLD);
            MPI_Send(vec.data(), m, MPI_INT, proc, 1, MPI_COMM_WORLD);
            MPI_Send(&constant[proc], 1, MPI_INT, proc, 2, MPI_COMM_WORLD);
        }
    }
    int local_size_vec = 0;
    if (rank != 0) {
        MPI_Status status1;
        MPI_Recv(&local_size_vec, 1, MPI_INT, 0, 2, MPI_COMM_WORLD, &status1);
    }
    std::vector<int> local_row(local_size_vec);
    std::vector<int> local_vec(m);
    if (rank == 0) {
        local_row = std::vector<int>(matrix.begin(), matrix.begin() + constant[0]);
        local_vec = std::vector<int>(vec.begin(), vec.begin() + m);
    } else {
        MPI_Status status;
        MPI_Recv(local_row.data(), local_size_vec, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(local_vec.data(), m, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
    }
    std::vector<int> local_value = getMultVectorSequential(local_row, local_vec, local_row.size() / m, m);
    MPI_Gatherv(local_value.data(), local_value.size(), MPI_INT, global_vec.data(),
                recvcounts, displs, MPI_INT, 0, MPI_COMM_WORLD);
    return global_vec;
}
