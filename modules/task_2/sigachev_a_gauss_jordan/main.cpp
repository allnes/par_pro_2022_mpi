// Copyright 2022 Sigachev Anton
#include <gtest/gtest.h>
#include <random>
#include "../../../modules/task_2/sigachev_a_gauss_jordan/gauss_jordan.h"
#include <gtest-mpi-listener.hpp>

TEST(parallel_mpi, parallel_calculation_solution_matrix_1) {
    int size_matrix = 3, rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int num_rows_rank = getNumRows(size_matrix, size, rank);
    int* rows = new int[sizeof(int) * num_rows_rank];
    int k = size_matrix + 1;
    double* mtx = new
        double[sizeof(double) * num_rows_rank * (k)];
    std::mt19937 gen;
    for (int i = 0; i < num_rows_rank; i++) {
        rows[i] = rank + size * i;
        gen.seed(rows[i] * (k));
        for (int j = 0; j <= size_matrix; j++)
            mtx[i * (k) + j] = gen() % 100 + 1;
    }

    double* res = parallelGaussJordan(size_matrix, num_rows_rank, rows, mtx);
    if (rank == 0) {
        for (int j = 0; j < size_matrix; j++)
            std::cout << res[j] << " ";
        std::cout << "\n";
    }
}

TEST(parallel_mpi, parallel_calculation_solution_matrix_2) {
    int size_matrix = 6, rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int num_rows_rank = getNumRows(size_matrix, size, rank);
    int* rows = new int[sizeof(int) * num_rows_rank];
    int k = size_matrix + 1;
    double* mtx = new
        double[sizeof(double) * num_rows_rank * (k)];
    std::mt19937 gen;
    for (int i = 0; i < num_rows_rank; i++) {
        rows[i] = rank + size * i;
        gen.seed(rows[i] * (k));
        for (int j = 0; j <= size_matrix; j++)
            mtx[i * (k) + j] = gen() % 100 + 1;
    }
    double* res = parallelGaussJordan(size_matrix, num_rows_rank, rows, mtx);
    if (rank == 0) {
        for (int j = 0; j < size_matrix; j++)
            std::cout << res[j] << " ";
        std::cout << "\n";
    }
}

TEST(parallel_mpi, parallel_calculation_solution_matrix_3) {
    int size_matrix = 9, rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int num_rows_rank = getNumRows(size_matrix, size, rank);
    int* rows = new int[sizeof(int) * num_rows_rank];
    int k = size_matrix + 1;
    double* mtx = new
        double[sizeof(double) * num_rows_rank * (k)];
    std::mt19937 gen;
    for (int i = 0; i < num_rows_rank; i++) {
        rows[i] = rank + size * i;
        gen.seed(rows[i] * (k));
        for (int j = 0; j <= size_matrix; j++)
            mtx[i * (k) + j] = gen() % 100 + 1;
    }
    double* res = parallelGaussJordan(size_matrix, num_rows_rank, rows, mtx);
    if (rank == 0) {
        for (int j = 0; j < size_matrix; j++)
            std::cout << res[j] << " ";
        std::cout << "\n";
    }
}

TEST(parallel_mpi, parallel_calculation_solution_matrix_4) {
    int size_matrix = 12, rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int num_rows_rank = getNumRows(size_matrix, size, rank);
    int* rows = new int[sizeof(int) * num_rows_rank];
    int k = size_matrix + 1;
    double* mtx = new
        double[sizeof(double) * num_rows_rank * (k)];
    std::mt19937 gen;
    for (int i = 0; i < num_rows_rank; i++) {
        rows[i] = rank + size * i;
        gen.seed(rows[i] * (k));
        for (int j = 0; j <= size_matrix; j++)
            mtx[i * (k) + j] = gen() % 100 + 1;
    }
    double* res = parallelGaussJordan(size_matrix, num_rows_rank, rows, mtx);
    if (rank == 0) {
        for (int j = 0; j < size_matrix; j++)
            std::cout << res[j] << " ";
        std::cout << "\n";
    }
}

TEST(parallel_mpi, parallel_calculation_solution_matrix_5) {
    int size_matrix = 15, rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int num_rows_rank = getNumRows(size_matrix, size, rank);
    int* rows = new int[sizeof(int) * num_rows_rank];
    int k = size_matrix + 1;
    double* mtx = new
        double[sizeof(double) * num_rows_rank * (k)];
    std::mt19937 gen;
    for (int i = 0; i < num_rows_rank; i++) {
        rows[i] = rank + size * i;
        gen.seed(rows[i] * (k));
        for (int j = 0; j <= size_matrix; j++)
            mtx[i * (k) + j] = gen() % 100 + 1;
    }
    double* res = parallelGaussJordan(size_matrix, num_rows_rank, rows, mtx);
    if (rank == 0) {
        for (int j = 0; j < size_matrix; j++)
            std::cout << res[j] << " ";
        std::cout << "\n";
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}
