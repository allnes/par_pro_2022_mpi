// Copyright 2022 Bulgakov Daniil

#include <gtest/gtest.h>
#include <string>
#include <random>
#include <iostream>
#include "./radix_batcher.h"
#include "./batcher_merge.h"
#include "./radix_sort.h"
#include <gtest-mpi-listener.hpp>

// #define debug
#define test1
#define test2
#define test3
#define test4
#define test5


#ifdef test1
TEST(Parallel_Operations_MPI, Test_Radix_Sort) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        int vec_size = 100;
        std::vector<double> unsorted_vec = genvec(vec_size * vec_size);
        radix_sort(unsorted_vec.data(), unsorted_vec.size());
        for (int i = 0; i < unsorted_vec.size() - 1; i++) {
            ASSERT_LE(unsorted_vec[i], unsorted_vec[i+1]);
        }
    }
}
#endif

#ifdef test2
TEST(Parallel_Operations_MPI, Test_Batcher_Merge_Not_Power_2) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int vec_size = 333;
    std::vector<double> unsorted_vec = genvec(size * vec_size);
    std::vector<double> sorted_vec = parallel_radix_batcher_sort(unsorted_vec);
    if (rank == 0) {
        for (int i = 0; i < sorted_vec.size() - 1; i++) {
            ASSERT_LE(sorted_vec[i], sorted_vec[i+1]);
        }
    }
}
#endif


#ifdef test3
TEST(Parallel_Operations_MPI, Test_Batcher__Power_2) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int vec_size = 256;
    std::vector<double> unsorted_vec = genvec(size * vec_size);
    std::vector<double> sorted_vec = parallel_radix_batcher_sort(unsorted_vec);
    if (rank == 0) {
        for (int i = 0; i < sorted_vec.size() - 1; i++) {
            ASSERT_LE(sorted_vec[i], sorted_vec[i+1]);
        }
    }
}
#endif

#ifdef test4
TEST(Parallel_Operations_MPI, Test_If_Merge_1) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int vec_size = 3;
    std::vector<double> unsorted_vec = genvec(size * vec_size);
    std::vector<double> sorted_vec = parallel_radix_batcher_sort(unsorted_vec);
    if (rank == 0) {
        for (int i = 0; i < sorted_vec.size() - 1; i++) {
            ASSERT_LE(sorted_vec[i], sorted_vec[i+1]);
        }
    }
}
#endif

#ifdef test5
TEST(Parallel_Operations_MPI, Test_Long) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int vec_size = 1000;
    std::vector<double> unsorted_vec = genvec(size * vec_size);
    std::vector<double> sorted_vec = parallel_radix_batcher_sort(unsorted_vec);
    if (rank == 0) {
        for (int i = 0; i < sorted_vec.size() - 1; i++) {
            ASSERT_LE(sorted_vec[i], sorted_vec[i+1]);
        }
    }
}
#endif


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
