// Copyright 2022 Nikolaev Alexander
#include <gtest/gtest.h>
#include <vector>
#include "./hor_scheme.h"
#include <gtest-mpi-listener.hpp>

TEST(Horiz_Scheme_MPI, Test_Mult1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_matrix, global_vec;
    const int n = 4, m = 2;

    if (rank == 0) {
        global_matrix = getRandomMatrix(n, m);
        global_vec = getRandomVector(m);
    }

    std::vector<int> parallel_mult = getMultVectorParallel(global_matrix, global_vec, n, m);
    if (rank == 0) {
        std::vector<int> sequential_mult = getMultVectorSequential(global_matrix, global_vec, n, m);
        ASSERT_EQ(sequential_mult, parallel_mult);
    }
}

TEST(Horiz_Scheme_MPI, Test_Mult2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_matrix, global_vec;
    const int n = 4, m = 4;

    if (rank == 0) {
        global_matrix = getRandomMatrix(n, m);
        global_vec = getRandomVector(m);
    }

    std::vector<int> parallel_mult = getMultVectorParallel(global_matrix, global_vec, n, m);
    if (rank == 0) {
        std::vector<int> sequential_mult = getMultVectorSequential(global_matrix, global_vec, n, m);
        ASSERT_EQ(sequential_mult, parallel_mult);
    }
}

TEST(Horiz_Scheme_MPI, Test_Mult3) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_matrix, global_vec;
    const int n = 8, m = 8;

    if (rank == 0) {
        global_matrix = getRandomMatrix(n, m);
        global_vec = getRandomVector(m);
    }

    std::vector<int> parallel_mult = getMultVectorParallel(global_matrix, global_vec, n, m);
    if (rank == 0) {
        std::vector<int> sequential_mult = getMultVectorSequential(global_matrix, global_vec, n, m);
        ASSERT_EQ(sequential_mult, parallel_mult);
    }
}

TEST(Horiz_Scheme_MPI, Test_Mult4) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_matrix, global_vec;
    const int n = 11, m = 11;

    if (rank == 0) {
        global_matrix = getRandomMatrix(n, m);
        global_vec = getRandomVector(m);
    }

    std::vector<int> parallel_mult = getMultVectorParallel(global_matrix, global_vec, n, m);
    if (rank == 0) {
        std::vector<int> sequential_mult = getMultVectorSequential(global_matrix, global_vec, n, m);
        ASSERT_EQ(sequential_mult, parallel_mult);
    }
}

TEST(Horiz_Scheme_MPI, Test_Mult5) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_matrix, global_vec;
    const int n = 20, m = 20;

    if (rank == 0) {
        global_matrix = getRandomMatrix(n, m);
        global_vec = getRandomVector(m);
    }

    std::vector<int> parallel_mult = getMultVectorParallel(global_matrix, global_vec, n, m);
    if (rank == 0) {
        std::vector<int> sequential_mult = getMultVectorSequential(global_matrix, global_vec, n, m);
        ASSERT_EQ(sequential_mult, parallel_mult);
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
