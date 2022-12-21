// Copyright 2022 Zorin Oleg
#include <gtest/gtest.h>
#include <vector>
#include "./jacobi.h"
#include <gtest-mpi-listener.hpp>

TEST(Jacobi_MPI, operator_minus_vectors) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    Vector X = {0.0, 1.0, 3.0, -5.0, -1.1};
    Vector Y = {-3.0, 2.0, -1.1, 2.2, -3.3};
    Vector ans = {3.0, -1.0, 4.1, -7.2, 2.2};
    Vector res = X - Y;
    if (rank == 0) {
        for (std::size_t i = 0; i < res.size(); i++) {
            ASSERT_DOUBLE_EQ(ans[i], res[i]);
        }
    }
}

TEST(Jacobi_MPI, norm_of_vector) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    Vector X = {0.0, 1.0, 3.0, -5.0, -1.1};
    Vector Y = {-3.0, 2.0, -1.1, 2.2, -3.3};
    Vector ans = {3.0, -1.0, 4.1, -7.2, 2.2};
    Vector res = X - Y;
    if (rank == 0) {
        double norms[4] = {5.0, 3.3, 7.2, 7.2};
        ASSERT_DOUBLE_EQ(norms[0], normVector(X));
        ASSERT_DOUBLE_EQ(norms[1], normVector(Y));
        ASSERT_DOUBLE_EQ(norms[2], normVector(ans));
        ASSERT_DOUBLE_EQ(norms[3], normVector(res));
    }
}

TEST(Jacobi_MPI, expand_vector) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    Vector X = {0.0, 1.0, 3.0, -5.0, -1.1};
    Vector Y = expandVector(X, 10);
    if (rank == 0) {
        for (std::size_t i = 0; i < X.size(); i++) {
            ASSERT_DOUBLE_EQ(X[i], Y[i]);
        }
        for (std::size_t i = X.size(); i < X.size() + 10; i++) {
            ASSERT_DOUBLE_EQ(0.0, Y[i]);
        }
    }
}

TEST(Jacobi_MPI, expand_matrix) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    Matrix X = {{1.1, 1.2},
                {2.1, 2.2}};
    Matrix Y = expandMatrix(X, 2);
    if (rank == 0) {
        for (std::size_t i = 0; i < X.size(); i++) {
            for (std::size_t j = 0; j < X.size(); j++) {
                ASSERT_DOUBLE_EQ(X[i][j], Y[i][j]);
            }
            for (std::size_t j = X.size(); j < X.size() + 2; j++) {
                ASSERT_DOUBLE_EQ(0.0, Y[i][j]);
            }
        }
        for (std::size_t i = X.size(); i < X.size() + 2; i++) {
            for (std::size_t j = 0; j < Y.size(); j++) {
                if (i == j)
                    ASSERT_DOUBLE_EQ(1.0, Y[i][j]);
                else
                    ASSERT_DOUBLE_EQ(0.0, Y[i][j]);
            }
        }
    }
}

TEST(Jacobi_MPI, jacobi_sequential) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    Matrix A = {{20.0, 1.0,  -2.0},
                {3.0,  20.0, -1.0},
                {2.0,  -3.0, 20.0}};
    Vector B = {17.0, -18.0, 25.0};
    Vector ans = {1.0, -1.0, 1.0};
    if (rank == 0) {
        Vector X = JacobiSequential(A, B);
        for (std::size_t i = 0; i < X.size(); i++) {
            ASSERT_DOUBLE_EQ(ans[i], X[i]);
        }
    }
}

TEST(Jacobi_MPI, jacobi_parallel) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    Matrix A = {{20.0, 1.0,  -2.0},
                {3.0,  20.0, -1.0},
                {2.0,  -3.0, 20.0}};
    Vector B = {17.0, -18.0, 25.0};
    Vector ans = {1.0, -1.0, 1.0};

    Vector X = JacobiParallel(A, B);

    if (rank == 0) {
        for (std::size_t i = 0; i < X.size(); i++) {
            ASSERT_DOUBLE_EQ(ans[i], X[i]);
        }
    }
}

TEST(Jacobi_MPI, jacobi_parallel1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    Matrix A = {{4,  -1, 0,  0},
                {-1, 4,  -1, 0},
                {0,  -1, 4,  -1},
                {0,  0,  -1, 4}};
    Vector B = {2, 4, 6, 13};
    Vector ans = {1, 2, 3, 4};

    Vector X = JacobiParallel(A, B);

    if (rank == 0) {
        for (std::size_t i = 0; i < X.size(); i++) {
            ASSERT_DOUBLE_EQ(ans[i], X[i]);
        }
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
