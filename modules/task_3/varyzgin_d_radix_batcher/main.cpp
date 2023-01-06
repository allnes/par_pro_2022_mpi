// Copyright 2023 Varyzgin Dmitriy

#include <gtest/gtest.h>

#include <gtest-mpi-listener.hpp>
#include <iostream>
#include <random>
#include <string>

#include "./radix_batcher.h"

TEST(Radix_Bather, Radix_Sort) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        int length = 134;
        std::vector<double> vec = genDobVec(length * length);
        radix_sort(vec.data(), vec.size());
        for (int i = 0; i < vec.size() - 1; i++) {
            ASSERT_LE(vec[i], vec[i + 1]);
        }
    }
}

TEST(Radix_Bather, Batcher_Merge_Not_Power_2) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int length = 358;
    std::vector<double> vec = genDobVec(size * length);
    std::vector<double> svec = radixBatcherParallel(vec);
    if (rank == 0) {
        for (int i = 0; i < svec.size() - 1; i++) {
            ASSERT_LE(svec[i], svec[i + 1]);
        }
    }
}

TEST(Radix_Bather, Batcher__Power_2) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int length = 128;
    std::vector<double> vec = genDobVec(size * length);
    std::vector<double> svec = radixBatcherParallel(vec);
    if (rank == 0) {
        for (int i = 0; i < svec.size() - 1; i++) {
            ASSERT_LE(svec[i], svec[i + 1]);
        }
    }
}

TEST(Radix_Bather, If_Merge_1) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int length = 3;
    std::vector<double> vec = genDobVec(size * length);
    std::vector<double> svec = radixBatcherParallel(vec);
    if (rank == 0) {
        for (int i = 0; i < svec.size() - 1; i++) {
            ASSERT_LE(svec[i], svec[i + 1]);
        }
    }
}

TEST(Radix_Bather, Test_Long) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int length = 5000;
    std::vector<double> vec = genDobVec(size * length);
    std::vector<double> svec = radixBatcherParallel(vec);
    if (rank == 0) {
        for (int i = 0; i < svec.size() - 1; i++) {
            ASSERT_LE(svec[i], svec[i + 1]);
        }
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
