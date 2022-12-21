// Copyright 2022 Sigachev Anton
#include <gtest/gtest.h>
#include "./row_sum.h"
#include <gtest-mpi-listener.hpp>

TEST(Parallel_Operations_MPI, Matrix_fewer_rows_than_processes) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int rows = 1, cols = 5;

    if (size == 1) {
        ASSERT_NO_THROW(getParallelOperations(getRandomMatrix(rows, cols),
            rows, cols));
    } else {
        ASSERT_ANY_THROW(getParallelOperations(getRandomMatrix(rows, cols),
            rows, cols));
    }
}

TEST(Parallel_Operations_MPI, Matrix_5_rows_5_cols) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int* matrix = nullptr;
    int rows = 5, cols = 5;

    if (rank == 0) {
        matrix = getRandomMatrix(rows, cols);
    }

    int* global_sum = getParallelOperations(matrix, rows, cols);

    if (rank == 0) {
        int* reference_sum = getSequentialOperations(matrix, rows, cols);
        for (int i = 0; i < rows; i++) {
            ASSERT_EQ(reference_sum[i], global_sum[i]);
        }
    }
}

TEST(Parallel_Operations_MPI, Matrix_5_rows_10_cols) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int* matrix = nullptr;
    int rows = 5, cols = 10;

    if (rank == 0) {
        matrix = getRandomMatrix(rows, cols);
    }

    int* global_sum = getParallelOperations(matrix, rows, cols);

    if (rank == 0) {
        int* reference_sum = getSequentialOperations(matrix, rows, cols);
        for (int i = 0; i < rows; i++) {
            ASSERT_EQ(reference_sum[i], global_sum[i]);
        }
    }
}

TEST(Parallel_Operations_MPI, Matrix_10_rows_5_cols) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int* matrix = nullptr;
    int rows = 10, cols = 5;

    if (rank == 0) {
        matrix = getRandomMatrix(rows, cols);
    }

    int* global_sum = getParallelOperations(matrix, rows, cols);

    if (rank == 0) {
        int* reference_sum = getSequentialOperations(matrix, rows, cols);
        for (int i = 0; i < rows; i++) {
            ASSERT_EQ(reference_sum[i], global_sum[i]);
        }
    }
}

TEST(Parallel_Operations_MPI, Matrix_100_rows_100_cols) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int* matrix = nullptr;
    int rows = 100, cols = 100;

    if (rank == 0) {
        matrix = getRandomMatrix(rows, cols);
    }

    int* global_sum = getParallelOperations(matrix, rows, cols);

    if (rank == 0) {
        int* reference_sum = getSequentialOperations(matrix, rows, cols);
        for (int i = 0; i < rows; i++) {
            ASSERT_EQ(reference_sum[i], global_sum[i]);
        }
    }
}

TEST(Parallel_Operations_MPI, Matrix_200_rows_300_cols) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int* matrix = nullptr;
    int rows = 200, cols = 300;

    if (rank == 0) {
        matrix = getRandomMatrix(rows, cols);
    }

    int* global_sum = getParallelOperations(matrix, rows, cols);

    if (rank == 0) {
        int* reference_sum = getSequentialOperations(matrix, rows, cols);
        for (int i = 0; i < rows; i++) {
            ASSERT_EQ(reference_sum[i], global_sum[i]);
        }
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
