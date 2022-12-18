// Copyright 2022 Chernova Anna
#include <gtest/gtest.h>
#include "./sort_shell_batcher.h"
#include <gtest-mpi-listener.hpp>

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

TEST(SORT_SHELL_BATCHER, IS_ONE_THREAD_VALID) {
    int rank, num;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num);
    if (log2(num) == static_cast<int>(log2(num))) {
        if (rank == 0) {
            auto v = randArr(128);
            shellSort(&v);
            for (int i = 0; i < static_cast<int>(v.size()) - 1; i++) {
                EXPECT_TRUE(v[i] <= v[i + 1]);
            }
        }
    }
}

TEST(SORT_SHELL_BATCHER, IS_ODD_EVEN_MERGE_VALID) {
    int rank, num;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num);
    if (log2(num) == static_cast<int>(log2(num))) {
        if (rank == 0) {
            auto v1 = randArr(128);
            auto v2 = randArr(128);
            shellSort(&v1);
            shellSort(&v2);
            auto result = v1;
            result.insert(result.end(), v2.begin(), v2.end());
            oddEvenMerge(&result, result.size());
            for (int i = 0; i < static_cast<int>(result.size()) - 1; i++) {
                EXPECT_TRUE(result[i] <= result[i + 1]);
            }
        }
    }
}

TEST(SORT_SHELL_BATCHER, IS_GLOBAL_MERGE_VALID) {
    int rank, num;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num);
    if (log2(num) == static_cast<int>(log2(num))) {
        if (rank == 0) {
            auto v1 = randArr(32);
            auto v2 = randArr(32);
            auto v3 = randArr(32);
            auto v4 = randArr(32);
            shellSort(&v1);
            shellSort(&v2);
            shellSort(&v3);
            shellSort(&v4);
            vector<vector<int>> result;
            result.push_back(v1);
            result.push_back(v2);
            result.push_back(v3);
            result.push_back(v4);
            auto temp = merge(result);
            for (int i = 0; i < static_cast<int>(temp.size()) - 1; i++) {
                EXPECT_TRUE(temp[i] <= temp[i + 1]);
            }
        }
    }
}

TEST(SORT_SHELL_BATCHER, IS_MULTI_THREAD_VALID) {
    int rank, num;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num);
    if (log2(num) == static_cast<int>(log2(num))) {
        vector<int> temp;
        if (rank == 0) {
            temp = randArr(128);
        }
        temp = shellSortMPI(temp, 128);
        if (rank == 0) {
            for (int i = 0; i < static_cast<int>(temp.size()) - 1; i++) {
                EXPECT_TRUE(temp[i] <= temp[i + 1]);
            }
        }
    }
}

TEST(SORT_SHELL_BATCHER, BIG_DATA) {
    int rank, num;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num);
    if (log2(num) == static_cast<int>(log2(num))) {
        vector<int> temp;
        if (rank == 0) {
            temp = randArr(pow(2, 16));
        }
        temp = shellSortMPI(temp, pow(2, 16));
        if (rank == 0) {
            for (int i = 0; i < static_cast<int>(temp.size()) - 1; i++) {
                EXPECT_TRUE(temp[i] <= temp[i + 1]);
            }
        }
    }
}

TEST(SORT_SHELL_BATCHER, SMALL_DATA) {
    int rank, num;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num);
    if (log2(num) == static_cast<int>(log2(num))) {
        vector<int> temp;
        if (rank == 0) {
            temp = randArr(4);
        }
        temp = shellSortMPI(temp, 4);
        if (rank == 0) {
            for (int i = 0; i < static_cast<int>(temp.size()) - 1; i++) {
                EXPECT_TRUE(temp[i] <= temp[i + 1]);
            }
        }
    }
}
