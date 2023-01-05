// Copyright 2022 Nikolaev Alexander
#include <gtest/gtest.h>
#include <vector>
#include "./shell_batcher.h"
#include <gtest-mpi-listener.hpp>

TEST(Shell_Bather_MPI, Test_Sort1) {
    int rank;
    const int ArraySize = 8;
    std::vector<int> array;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        array = getRandomArray(ArraySize);
    }
    std::vector<int> parallel_sort = GetShellSortParallel(array, ArraySize);
    if (rank == 0) {
        std::vector<int> sequential_sort = GetShellSortSequential(array);
        ASSERT_EQ(sequential_sort, parallel_sort);
    }
}

TEST(Shell_Bather_MPI, Test_Sort2) {
    int rank;
    const int ArraySize = 32;
    std::vector<int> array;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        array = getRandomArray(ArraySize);
    }
    std::vector<int> parallel_sort = GetShellSortParallel(array, ArraySize);
    if (rank == 0) {
        std::vector<int> sequential_sort = GetShellSortSequential(array);
        ASSERT_EQ(sequential_sort, parallel_sort);
    }
}

TEST(Shell_Bather_MPI, Test_Sort3) {
    int rank;
    const int ArraySize = 64;
    std::vector<int> array;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        array = getRandomArray(ArraySize);
    }
    std::vector<int> parallel_sort = GetShellSortParallel(array, ArraySize);
    if (rank == 0) {
        std::vector<int> sequential_sort = GetShellSortSequential(array);
        ASSERT_EQ(sequential_sort, parallel_sort);
    }
}

TEST(Shell_Bather_MPI, Test_Sort4) {
    int rank;
    const int ArraySize = 256;
    std::vector<int> array;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        array = getRandomArray(ArraySize);
    }
    std::vector<int> parallel_sort = GetShellSortParallel(array, ArraySize);
    if (rank == 0) {
        std::vector<int> sequential_sort = GetShellSortSequential(array);
        ASSERT_EQ(sequential_sort, parallel_sort);
    }
}

TEST(Shell_Bather_MPI, Test_Sort5) {
    int rank;
    const int ArraySize = 512;
    std::vector<int> array;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        array = getRandomArray(ArraySize);
    }
    std::vector<int> parallel_sort = GetShellSortParallel(array, ArraySize);
    if (rank == 0) {
        std::vector<int> sequential_sort = GetShellSortSequential(array);
        ASSERT_EQ(sequential_sort, parallel_sort);
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
