// Copyright 2023 Varyzgin Dmitriy

#include <gtest/gtest.h>
#include <vector>
#include "./mult_mat_hor_vert.h"
#include <gtest-mpi-listener.hpp>

TEST(Multiply, MultMatrix) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> A(15);
    std::vector<int> B(6);
    std::vector<int> Vec(3);
    std::vector<int> res = {80, 40, 77, 37, 67, 35, 51, 31, 44, 28};
    std::vector<int> Res(10, 0);

    A = {1, 7, 3, 2, 1, 9, 3, 2, 6,
         4, 4, 1, 5, 1, 3};
    B = {3, 3, 8, 4, 7, 3};
    // A = {1, 7, 3, 1, 6, 2, 1, 9, 7, 3, 3, 2, 6,
    //      2, 5, 4, 4, 1, 4, 8, 5, 1, 3, 6, 4};
    // B = {3, 3, 5, 8, 7, 8, 4, 6, 2, 3, 7, 3, 8,
    //      4, 1, 6, 1, 7, 5, 2, 1, 5, 1, 8, 4};

    Res = multMatParallel(A, B, 3);

    if (rank == 0) {
        EXPECT_EQ(Res, res);
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
