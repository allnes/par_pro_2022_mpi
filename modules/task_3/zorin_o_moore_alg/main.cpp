// Copyright 2022 Zorin Oleg
#include <gtest/gtest.h>
#include <vector>
#include "./moore_alg.h"
#include <gtest-mpi-listener.hpp>

TEST(moore_algo, simple_test) {
    int size = 2;
    int prank;
    MPI_Comm_rank(MPI_COMM_WORLD, &prank);
    std::vector<std::vector<int>> matrix(size, std::vector<int>(size, INF));
    matrix[0][1] = 1;
    int ans = moore_algorithm(matrix, 0, size - 1, size);
    if (prank == 0) {
        ASSERT_EQ(ans, 1);
    }
}

TEST(moore_algo, simple_neg_weight) {
    int size = 2;
    int prank;
    MPI_Comm_rank(MPI_COMM_WORLD, &prank);
    std::vector<std::vector<int>> matrix(size, std::vector<int>(size, INF));
    for (int i = 0; i < size - 1; i++) {
        matrix[i][i + 1] = -1;
    }
    int ans = moore_algorithm(matrix, 0, size - 1, size);
    if (prank == 0) {
        ASSERT_EQ(ans, -1 * size + 1);
    }
}

TEST(moore_algo, small_grahp) {
    int size = 3;
    int prank;
    MPI_Comm_rank(MPI_COMM_WORLD, &prank);
    std::vector<std::vector<int>> matrix(size, std::vector<int>(size, 1));
    for (int i = 0; i < size; i++) matrix[i][i] = INF;
    int ans = moore_algorithm(matrix, 0, size - 1, size);
    if (prank == 0) {
        ASSERT_EQ(ans, 1);
    }
}

TEST(moore_algo, big_test) {
    int size = 1345;
    int prank;
    MPI_Comm_rank(MPI_COMM_WORLD, &prank);
    std::vector<std::vector<int>> matrix(size, std::vector<int>(size, INF));
    for (int i = 0; i < size - 1; i++) matrix[i][i + 1] = 1;
    int ans = moore_algorithm(matrix, 0, size - 1, size);
    if (prank == 0) {
        ASSERT_EQ(ans, size - 1);
    }
}

TEST(moore_algo, big_test_neg_weigh) {
    int size = 1000;
    int prank;
    MPI_Comm_rank(MPI_COMM_WORLD, &prank);
    std::vector<std::vector<int>> matrix(size, std::vector<int>(size, INF));
    for (int i = 0; i < size - 1; i++) matrix[i][i + 1] = -1;
    int ans = moore_algorithm(matrix, 0, size - 1, size);
    if (prank == 0) {
        ASSERT_EQ(ans, -1 * size + 1);
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
