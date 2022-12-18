// Copyright 2022 Strogantsev Anton
#include <gtest/gtest.h>
#include <vector>
#include "./matrix_column_sum.h"
#include <gtest-mpi-listener.hpp>


TEST(CalculateMatrixSumParallel, CorrectCalculation) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int matrixXSize = 5;
    const int matrixYSize = 5;

    std::vector<int> matrix;
    if (rank == 0) {
        matrix = std::vector<int>{
            1, 2, 3, 4, 5,
            1, 2, 3, 4, 5,
            1, 2, 3, 4, 5,
            1, 2, 3, 4, 5,
            1, 2, 3, 4, 5,
        };
    }

    auto sum = calculateMatrixSumParallel(matrix, matrixXSize, matrixYSize);

    if (rank == 0) {
        auto expectedSum = std::vector<int>{ 5, 10, 15, 20, 25 };
        auto sequentialSum = calculateMatrixSumSequentially(matrix, matrixXSize, matrixYSize);
        ASSERT_EQ(expectedSum, sum);
        ASSERT_EQ(expectedSum, sequentialSum);
    }
}

TEST(CalculateMatrixSumParallel, CorrectCalculationWithWideRandomMatrix) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int matrixXSize = 13;
    const int matrixYSize = 5;

    std::vector<int> matrix;
    if (rank == 0) {
        matrix = generateRandomMatrix(matrixXSize, matrixYSize);
    }

    auto sum = calculateMatrixSumParallel(matrix, matrixXSize, matrixYSize);

    if (rank == 0) {
        auto expectedSum = calculateMatrixSumSequentially(matrix, matrixXSize, matrixYSize);
        ASSERT_EQ(expectedSum, sum);
    }
}

TEST(CalculateMatrixSumParallel, CorrectCalculationWithLongRandomMatrix) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int matrixXSize = 4;
    const int matrixYSize = 15;

    std::vector<int> matrix;
    if (rank == 0) {
        matrix = generateRandomMatrix(matrixXSize, matrixYSize);
    }

    auto sum = calculateMatrixSumParallel(matrix, matrixXSize, matrixYSize);

    if (rank == 0) {
        auto expectedSum = calculateMatrixSumSequentially(matrix, matrixXSize, matrixYSize);
        ASSERT_EQ(expectedSum, sum);
    }
}

TEST(CalculateMatrixSumParallel, CorrectCalculationWithSquareRandomMatrix) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int matrixXSize = 13;
    const int matrixYSize = 13;

    std::vector<int> matrix;
    if (rank == 0) {
        matrix = generateRandomMatrix(matrixXSize, matrixYSize);
    }

    auto sum = calculateMatrixSumParallel(matrix, matrixXSize, matrixYSize);

    if (rank == 0) {
        auto expectedSum = calculateMatrixSumSequentially(matrix, matrixXSize, matrixYSize);
        ASSERT_EQ(expectedSum, sum);
    }
}

TEST(CalculateMatrixSumParallel, CorrectCalculationWith1x1RandomMatrix) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> matrix;
    if (rank == 0) {
        matrix = generateRandomMatrix(1, 1);
    }

    auto sum = calculateMatrixSumParallel(matrix, 1, 1);

    if (rank == 0) {
        auto expectedSum = calculateMatrixSumSequentially(matrix, 1, 1);
        ASSERT_EQ(expectedSum, sum);
    }
}

TEST(CalculateMatrixSumParallel, CorrectCalculationWithZeroMatrix) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int matrixXSize = 0;
    const int matrixYSize = 0;

    std::vector<int> matrix;

    auto sum = calculateMatrixSumParallel(matrix, matrixXSize, matrixYSize);

    if (rank == 0) {
        ASSERT_EQ(std::vector<int>(), sum);
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
