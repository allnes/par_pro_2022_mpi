// Copyright 2022 Gosteeva Ekaterina


#include <gtest/gtest.h>
#include <vector>
#include "./quick_sort_batcher.h"
#include <gtest-mpi-listener.hpp>

void SortTEST(int vec_size) {
    int MPI_rank, MPI_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &MPI_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &MPI_size);
    std::vector<int> vec;
    if (MPI_rank == 0) {
        vec = GetRandomVec(MPI_size * vec_size);
    }
    std::vector<int> sort_vec = QuickSortParrallel(vec);
    if (MPI_rank == 0) {
        for (size_t i = 0; i < sort_vec.size() - 1; i++) {
            EXPECT_TRUE(sort_vec.at(i) <= sort_vec.at(i + 1));
        }
    }
}

TEST(Quick_Sort_Batcher, Test1) {
    SortTEST(8);
}

TEST(Quick_Sort_Batcher, Test2) {
    SortTEST(32);
}

TEST(Quick_Sort_Batcher, Test3) {
    SortTEST(64);
}

TEST(Quick_Sort_Batcher, Test4) {
    SortTEST(512);
}

TEST(Quick_Sort_Batcher, Test5) {
    SortTEST(1024);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners &listeners = ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}
