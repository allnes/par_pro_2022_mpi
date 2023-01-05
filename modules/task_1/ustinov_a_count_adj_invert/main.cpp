// Copyright 2022 Ustinov A.
#include <mpi.h>
#include <gtest/gtest.h>
#include <vector>
#include <random>
#include "./count_adj_invert.h"
#include <gtest-mpi-listener.hpp>

using std::vector;
using std::mt19937;
using std::random_device;
using std::uniform_real_distribution;

void test_random_vector(size_t size) {
    int rank;
    int ans_parallel;
    int ans_sequential;
    vector<int> vec(size);
    mt19937 generator {random_device {}()};

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        for (size_t i = 0; i < size; ++i)
            vec[i] = generator();
    }
    ans_parallel = count_adjacent_invertions_parallel(vec);
    if (rank == 0) {
        ans_sequential = count_adjacent_invertions_sequential(vec);
        ASSERT_EQ(ans_parallel, ans_sequential);
    }
}

TEST(Count_Adjacent_Invertions_MPI, Test_1) {
    for (int i = 0; i < 10; ++i)
        test_random_vector(10);
}

TEST(Count_Adjacent_Invertions_MPI, Test_2) {
    for (int i = 0; i < 10; ++i)
        test_random_vector(100);
}

TEST(Count_Adjacent_Invertions_MPI, Test_3) {
    for (int i = 0; i < 10; ++i)
        test_random_vector(1000);
}

TEST(Count_Adjacent_Invertions_MPI, Test_4) {
    for (int i = 0; i < 10; ++i)
        test_random_vector(10000);
}

TEST(Count_Adjacent_Invertions_MPI, Test_5) {
    for (int i = 0; i < 10; ++i)
        test_random_vector(100000);
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
