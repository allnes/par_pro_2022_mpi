// Copyright 2022 Selivankin Sergey
#include <gtest/gtest.h>
#include <vector>
#include "./strassen.h"
#include <gtest-mpi-listener.hpp>

TEST(Parallel_Operations_MPI, Test_1) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int n = 8;

    std::vector<double> matA(n*n);
    std::vector<double> matB(n*n);
    std::vector<double> parallel_result;

    if (rank == 0) {
        matA = getRandomMatrix(n, n);
        matB = getRandomMatrix(n, n);
    }

    parallel_result = getStrassenParallel(matA, matB, n, n);

    if (rank == 0) {
        std::vector<double> sequence_result;
        std::vector<double> convertedMatA;
        std::vector<double> convertedMatB;

        int new_mat_size;
        convertedMatA = convertMatrixToRequiredSize(matA, n, n, &new_mat_size);
        convertedMatB = convertMatrixToRequiredSize(matB, n, n, &new_mat_size);

        sequence_result = getStrassenSequence(convertedMatA, convertedMatB);
        ASSERT_EQ(sequence_result, parallel_result);
    }
}

TEST(Parallel_Operations_MPI, Test_2) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int n = 16;

    std::vector<double> matA(n * n);
    std::vector<double> matB(n * n);
    std::vector<double> parallel_result;

    if (rank == 0) {
        matA = getRandomMatrix(n, n);
        matB = getRandomMatrix(n, n);
    }

    parallel_result = getStrassenParallel(matA, matB, n, n);

    if (rank == 0) {
        std::vector<double> sequence_result;
        std::vector<double> convertedMatA;
        std::vector<double> convertedMatB;

        int new_mat_size;
        convertedMatA = convertMatrixToRequiredSize(matA, n, n, &new_mat_size);
        convertedMatB = convertMatrixToRequiredSize(matB, n, n, &new_mat_size);

        sequence_result = getStrassenSequence(convertedMatA, convertedMatB);
        ASSERT_EQ(sequence_result, parallel_result);
    }
}

TEST(Parallel_Operations_MPI, Test_3) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int n = 8;
    int m = 5;

    std::vector<double> matA(m * n);
    std::vector<double> matB(m * n);
    std::vector<double> parallel_result;

    if (rank == 0) {
        matA = getRandomMatrix(m, n);
        matB = getRandomMatrix(m, n);
    }

    parallel_result = getStrassenParallel(matA, matB, m, n);

    if (rank == 0) {
        std::vector<double> sequence_result;
        std::vector<double> convertedMatA;
        std::vector<double> convertedMatB;

        int new_mat_size;
        convertedMatA = convertMatrixToRequiredSize(matA, m, n, &new_mat_size);
        convertedMatB = convertMatrixToRequiredSize(matB, m, n, &new_mat_size);

        sequence_result = getStrassenSequence(convertedMatA, convertedMatB);
        ASSERT_EQ(sequence_result, parallel_result);
    }
}

TEST(Parallel_Operations_MPI, Test_4) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int n = 10;
    int m = 9;

    std::vector<double> matA(m * n);
    std::vector<double> matB(m * n);
    std::vector<double> parallel_result;

    if (rank == 0) {
        matA = getRandomMatrix(m, n);
        matB = getRandomMatrix(m, n);
    }

    parallel_result = getStrassenParallel(matA, matB, m, n);

    if (rank == 0) {
        std::vector<double> sequence_result;
        std::vector<double> convertedMatA;
        std::vector<double> convertedMatB;

        int new_mat_size;
        convertedMatA = convertMatrixToRequiredSize(matA, m, n, &new_mat_size);
        convertedMatB = convertMatrixToRequiredSize(matB, m, n, &new_mat_size);

        sequence_result = getStrassenSequence(convertedMatA, convertedMatB);
        ASSERT_EQ(sequence_result, parallel_result);
    }
}

TEST(Parallel_Operations_MPI, Test_5) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int n = 4;
    int m = 7;

    std::vector<double> matA(m * n);
    std::vector<double> matB(m * n);
    std::vector<double> parallel_result;

    if (rank == 0) {
        matA = getRandomMatrix(m, n);
        matB = getRandomMatrix(m, n);
    }

    parallel_result = getStrassenParallel(matA, matB, m, n);

    if (rank == 0) {
        std::vector<double> sequence_result;
        std::vector<double> convertedMatA;
        std::vector<double> convertedMatB;

        int new_mat_size;
        convertedMatA = convertMatrixToRequiredSize(matA, m, n, &new_mat_size);
        convertedMatB = convertMatrixToRequiredSize(matB, m, n, &new_mat_size);

        sequence_result = getStrassenSequence(convertedMatA, convertedMatB);
        ASSERT_EQ(sequence_result, parallel_result);
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
