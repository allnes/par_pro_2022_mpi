// Copyright 2022 Panov Alexey
#include <gtest/gtest.h>
#include "./int_merge_sort.h"
#include <gtest-mpi-listener.hpp>


TEST(Parallel_Operations_MPI, Array_0) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    Vector arr;
    Vector sorted;

    sortParallel(&arr);

    if (rank == 0) {
        ASSERT_EQ(sorted, arr);
    }
}

TEST(Parallel_Operations_MPI, Array_1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    Vector arr = { 420 };
    Vector sorted = { 420 };

    sortParallel(&arr);

    if (rank == 0) {
        ASSERT_EQ(sorted, arr);
    }
}

TEST(Parallel_Operations_MPI, Array_10) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    Vector arr = { 114, 1001, 100, 8, 7, 6, 5, 14, 455, 780 };
    Vector sorted = { 5, 6, 7, 8, 14, 100, 114, 455, 780, 1001 };

    sortParallel(&arr);

    if (rank == 0) {
        ASSERT_EQ(sorted, arr);
    }
}

TEST(Parallel_Operations_MPI, Array_10_With_Negative_Nums) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    Vector arr = { -10, 10, 30, 20, 700, 1, -42 };
    Vector sorted = { -42, -10, 1, 10, 20, 30, 700 };

    sortParallel(&arr);

    if (rank == 0) {
        ASSERT_EQ(sorted, arr);
    }
}

TEST(Parallel_Operations_MPI, Array_20_With_Negative_Nums) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    Vector arr = {
        1001, 150, 45, 30, 20, 700, 1, -42, 12345, 123456,
        -1001, -150, -45, -30, -20, -700, -1, 42, -12345, -123456,
    };
    Vector sorted = {
        -123456, -12345, -1001, -700, -150, -45, -42, -30, -20, -1,
        1, 20, 30, 42, 45, 150, 700, 1001, 12345, 123456
    };

    sortParallel(&arr);

    if (rank == 0) {
        ASSERT_EQ(sorted, arr);
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
