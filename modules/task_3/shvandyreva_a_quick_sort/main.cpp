// Copyright 2022 Shvandyreva Alina
#include <gtest/gtest.h>
#include <vector>
#include "../../modules/task_3/shvandyreva_a_quick_sort/quick_sort.h"
#include <gtest-mpi-listener.hpp>


TEST(Sequential_Operations_MPI, Test_1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank != 0) return;

    std::vector<int> vec = {7, 8, 1, 2, 4, 5};
    std::vector<int> expected_res = { 1, 2, 4, 5, 7, 8 };
    std::vector<int> res = quickSortSequential(vec);

    EXPECT_EQ(expected_res, res);
}

TEST(Parallel_Operations_MPI, Test_Size_5) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> array(5);
    if (rank == 0)
        array = getRandomVector(5);
    std::vector<int> res = quickSortParallel(array);
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) {
        std::vector<int> exp_arr = quickSortSequential(array);
        ASSERT_EQ(exp_arr, res);
    }
}

TEST(Parallel_Operations_MPI, Test_Size_10) {
     int rank;
     MPI_Comm_rank(MPI_COMM_WORLD, &rank);
     std::vector<int> array(10);
     if (rank == 0)
         array = getRandomVector(10);
     std::vector<int> res = quickSortParallel(array);
     MPI_Barrier(MPI_COMM_WORLD);
     if (rank == 0) {
         std::vector<int> exp_arr = quickSortSequential(array);
         ASSERT_EQ(exp_arr, res);
     }
}

TEST(Parallel_Operations_MPI, Test_Size_13) {
     int rank;
     MPI_Comm_rank(MPI_COMM_WORLD, &rank);
     std::vector<int> array(13);
     if (rank == 0)
         array = getRandomVector(13);
     std::vector<int> res = quickSortParallel(array);
     MPI_Barrier(MPI_COMM_WORLD);
     if (rank == 0) {
         std::vector<int> exp_arr = quickSortSequential(array);
         ASSERT_EQ(exp_arr, res);
     }
}

TEST(Parallel_Operations_MPI, Test_Size_100) {
     int rank;
     MPI_Comm_rank(MPI_COMM_WORLD, &rank);
     std::vector<int> array(100);
     if (rank == 0)
         array = getRandomVector(100);
     std::vector<int> res = quickSortParallel(array);
     MPI_Barrier(MPI_COMM_WORLD);
     if (rank == 0) {
         std::vector<int> exp_arr = quickSortSequential(array);
         ASSERT_EQ(exp_arr, res);
     }
}


TEST(Parallel_Operations_MPI, Test_Size_111111) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> array(111111);
    if (rank == 0)
        array = getRandomVector(111111);
    std::vector<int> res = quickSortParallel(array);
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) {
        std::vector<int> exp_arr = quickSortSequential(array);
        ASSERT_EQ(exp_arr, res);
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
