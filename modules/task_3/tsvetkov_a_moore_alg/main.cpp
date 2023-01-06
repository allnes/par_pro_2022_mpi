// Copyright 2022 Tsvetkov
#include <gtest/gtest.h>
#include <vector>
#include <iostream>
#include <gtest-mpi-listener.hpp>
#include "./moore_alg.h"

TEST(Test_Moore, TEST_1_Sequence_Moore) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> graph = {0, 5, 2, 8,
                              5, 0, 3, 1,
                              2, 3, 0, 4,
                              8, 1, 4, 0};
    if (rank == 0) {
        std::vector<int> result = sequenceMoore(graph, 1, 4);
        std::vector<int> expectResult = {5, 0, 3, 1};
        EXPECT_EQ(expectResult, result);
    }
}

TEST(Test_Moore, TEST_2_Parallel_Moore) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> graph = {0, 5, 2, 8,
                              5, 0, 3, 1,
                              2, 3, 0, 4,
                              8, 1, 4, 0};
    std::vector<int> result = parallelMoore(graph, 1, 4);
    if (rank == 0) {
        std::vector<int> expectResult = {5, 0, 3, 1};
        EXPECT_EQ(expectResult, result);
    }
}

TEST(Test_Moore, TEST_3_Sequence_Moore) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> graph = {0, 5, 2, 8, 5,
                              5, 0, 3, 1, 7,
                              2, 3, 0, 4, 0,
                              8, 1, 4, 0, 8,
                              5, 7, 0, 8, 0};
    if (rank == 0) {
        std::vector<int> result = sequenceMoore(graph, 1, 5);
        std::vector<int> expectResult = {5, 0, 3, 1, 3};
        EXPECT_EQ(expectResult, result);
    }
}

TEST(Test_Moore, TEST_4_Parallel_Moore) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> graph = {0, 5, 2, 8, 5,
                              5, 0, 3, 1, 7,
                              2, 3, 0, 4, 0,
                              8, 1, 4, 0, 8,
                              5, 7, 0, 8, 0};
    std::vector<int> result = parallelMoore(graph, 1, 5);
    if (rank == 0) {
        std::vector<int> expectResult = {5, 0, 3, 1, 3};
        EXPECT_EQ(expectResult, result);
    }
}

TEST(Test_Moore, TEST_5_Sequence_Moore) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> graph = {0, 58, 62, 8, 5,
                              58, 0, 31, 1, 6,
                              62, 31, 0, 4, 0,
                              8, 1, 4, 0, 15,
                              5, 6, 0, 15, 0};
    if (rank == 0) {
        std::vector<int> result = sequenceMoore(graph, 1, 5);
        std::vector<int> expectResult = {9, 0, 5, 1, 5};
        EXPECT_EQ(expectResult, result);
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
