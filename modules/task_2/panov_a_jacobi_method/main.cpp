// Copyright 2022 Panov Alexey
#include <gtest/gtest.h>
#include "./jacobi_method.h"
#include <gtest-mpi-listener.hpp>


TEST(Parallel_Operations_MPI, Matrix_1x1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    Matrix A = {
        { 3 },
    };
    Vector b = { 6 };
    Vector expectedX = { 2 };

    Vector x = calculateJacobiParallel(A, b);

    if (rank == 0) {
        for (int i = 0; i < x.size(); i++) {
            ASSERT_NEAR(expectedX[i], x[i], EPSILON * 10.0);
        }
    }
}

TEST(Parallel_Operations_MPI, Matrix_2x2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    Matrix A = {
        { 3, 2 },
        { 2, 3 },
    };
    Vector b = { 1, -1 };
    Vector expectedX = { 1, -1 };

    Vector x = calculateJacobiParallel(A, b);

    if (rank == 0) {
        for (int i = 0; i < x.size(); i++) {
            ASSERT_NEAR(expectedX[i], x[i], EPSILON * 10.0);
        }
    }
}

TEST(Parallel_Operations_MPI, Matrix_3x3) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    Matrix A = {
        { 5, 2, 2 },
        { 2, 6, 3 },
        { 3, 3, 7 }
    };
    Vector b = { 9, 11, 13 };
    Vector expectedX = { 1, 1, 1 };

    Vector x = calculateJacobiParallel(A, b);

    if (rank == 0) {
        for (int i = 0; i < x.size(); i++) {
            ASSERT_NEAR(expectedX[i], x[i], EPSILON * 10.0);
        }
    }
}

TEST(Parallel_Operations_MPI, Matrix_Diagonal_3x3) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    Matrix A = {
        { 1, 0, 0 },
        { 0, 1, 0 },
        { 0, 0, 1 }
    };
    Vector b = { 9, 11, 13 };
    Vector expectedX = b;

    Vector x = calculateJacobiParallel(A, b);

    if (rank == 0) {
        for (int i = 0; i < x.size(); i++) {
            ASSERT_NEAR(expectedX[i], x[i], EPSILON * 10.0);
        }
    }
}

TEST(Parallel_Operations_MPI, Matrix_4x4) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    Matrix A = {
        { 6, 2, 2, 1 },
        { 2, 7, 3, 1 },
        { 3, 2, 7, 1 },
        { 3, 3, 7, 14 }
    };
    Vector b = { 11, 13, 13, 27 };
    Vector expectedX = { 1, 1, 1, 1 };

    Vector x = calculateJacobiParallel(A, b);

    if (rank == 0) {
        for (int i = 0; i < x.size(); i++) {
            ASSERT_NEAR(expectedX[i], x[i], EPSILON * 10.0);
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
