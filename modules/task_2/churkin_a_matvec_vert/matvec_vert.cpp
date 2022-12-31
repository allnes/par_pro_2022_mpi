// Copyright 2022 Churkin Alexander
#include <mpi.h>
#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include "../../modules/task_2/churkin_a_matvec_vert/matvec_vert.h"

std::vector<int> getRandomVector(int n) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::vector<int> vec(n);
    for (int i = 0; i < n; i++) {
        vec[i] = gen() % 10;
    }
    return vec;
}

std::vector<int> getRandomMatrix(int m, int n) {
    return getRandomVector(m * n);
}

void printVector(const std::vector<int>& vec) {
    std::cout << '[';
    for (int i = 0; i < static_cast<int>(vec.size()) - 1; i++) {
        std::cout << vec[i] << ", ";
    }
    std::cout << vec[static_cast<int>(vec.size()) - 1];
    std::cout << ']';
}

void printMatrix(const std::vector<int>& matrix, int m, int n) {
    for (int i = 0; i < m; i++) {
        std::cout << '[';
        for (int j = 0; j < n - 1; j++) {
            std::cout << matrix[m * j + i] << ", ";
        }
        std::cout << matrix[m * (n - 1) + i];
        std::cout << "]\n";
    }
}

std::vector<int> getMultSequential(const std::vector<int>& matrix, const std::vector<int>& vec, int m, int n) {
    std::vector<int> result(m);

    // Iterating over every [column number j]
    for (int j = 0; j < n; j++) {
        // Multiply every element of [column number j] by [vector[j]]
        for (int i = 0; i < m; i++) {
            result[i] += (matrix[m * j + i] * vec[j]);
        }
    }

    return result;
}

std::vector<int> getMultParallel(const std::vector<int>& matrix, const std::vector<int>& vec, int m, int n) {
    int comm_size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Every proccess will count colsToCount columns
    // (for latest colsToCount = (delta + diff),
    // for others colsToCount = delta)
    const int delta = n / comm_size;
    const int diff = n - comm_size * delta;

    int colsToCount;
    if (rank != comm_size - 1) {
        colsToCount = delta;
    } else {
        colsToCount = delta + diff;
    }

    std::vector<int> matrix_part(m * colsToCount);
    std::vector<int> vector_part(1 * colsToCount);

    std::vector<int> res_part(m);

    int* sendcounts = new int[comm_size];
    int* displs = new int[comm_size];

    int* sendcounts_2 = new int[comm_size];
    int* displs_2 = new int[comm_size];

    for (int i = 0; i < comm_size - 1; i++) {
        sendcounts[i] = (m * delta);
        displs[i] = i * (m * delta);

        sendcounts_2[i] = delta;
        displs_2[i] = i * delta;
    }

    sendcounts[comm_size - 1] = (m * (delta + diff));
    displs[comm_size - 1] = (comm_size - 1) * (m * delta);

    sendcounts_2[comm_size - 1] = delta + diff;
    displs_2[comm_size - 1] = (comm_size - 1) * delta;


    /* Scatter matrix columns */
    MPI_Scatterv(matrix.data(), sendcounts, displs, MPI_INT,
        matrix_part.data(), m * colsToCount, MPI_INT, 0, MPI_COMM_WORLD);
    /* Scatter vec elements */
    MPI_Scatterv(vec.data(), sendcounts_2, displs_2, MPI_INT,
        vector_part.data(), colsToCount, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < m * colsToCount; i++) {
        res_part[i % m] += matrix_part[i] * vector_part[i / m];
    }

    std::vector<int> result(m);
    MPI_Reduce(&res_part[0], &result[0], m, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    return result;
}
