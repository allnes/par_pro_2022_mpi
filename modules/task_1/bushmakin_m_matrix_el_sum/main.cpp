// Copyright 2022 Bushmakin Maksim
#include <gtest/gtest.h>
#include <vector>
#include "./matrix_el_sum.h"
#include <gtest-mpi-listener.hpp>

// 2 * 2
TEST(MatrixElementsSum, MatrixElementsSum_1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int m = 2, n = 2;

    std::vector<int> matrix;
    if (rank == 0) matrix = genRandomIntMatrix(m, n);

    int parallelResult = parallelMatSum(matrix, m, n);

    if (rank == 0) {
        int simpleResult = sequentialMatSum(matrix, m, n);
        ASSERT_EQ(parallelResult, simpleResult);
    }
}

// 3 * 2
TEST(Vector_Average_MPI, MatrixElementsSum_2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int m = 3, n = 2;

    std::vector<int> matrix;
    if (rank == 0) matrix = genRandomIntMatrix(m, n);

    int parallelResult = parallelMatSum(matrix, m, n);

    if (rank == 0) {
        int simpleResult = sequentialMatSum(matrix, m, n);
        ASSERT_EQ(parallelResult, simpleResult);
    }
}

// 4 * 5
TEST(Vector_Average_MPI, MatrixElementsSum_3) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int m = 4, n = 5;

    std::vector<int> matrix;
    if (rank == 0) matrix = genRandomIntMatrix(m, n);

    int parallelResult = parallelMatSum(matrix, m, n);

    if (rank == 0) {
        int simpleResult = sequentialMatSum(matrix, m, n);
        ASSERT_EQ(parallelResult, simpleResult);
    }
}

// 7 * 7
TEST(Vector_Average_MPI, MatrixElementsSum_4) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int m = 7, n = 7;

    std::vector<int> matrix;
    if (rank == 0) matrix = genRandomIntMatrix(m, n);

    int parallelResult = parallelMatSum(matrix, m, n);

    if (rank == 0) {
        int simpleResult = sequentialMatSum(matrix, m, n);
        ASSERT_EQ(parallelResult, simpleResult);
    }
}

// 49 * 49
TEST(Vector_Average_MPI, MatrixElementsSum_5) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int m = 49, n = 49;

    std::vector<int> matrix;
    if (rank == 0) matrix = genRandomIntMatrix(m, n);

    int parallelResult = parallelMatSum(matrix, m, n);

    if (rank == 0) {
        int simpleResult = sequentialMatSum(matrix, m, n);
        ASSERT_EQ(parallelResult, simpleResult);
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners &listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}
