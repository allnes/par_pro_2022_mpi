// Copyright 2018 Nesterov Alexander
#include <gtest/gtest.h>
#include <vector>
#include "./counting_alphabetic_char.h"
#include <gtest-mpi-listener.hpp>

TEST(Parallel_Operations_MPI, Test_Empty_string) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::string global_vec;
    const int count_size_vector = 0;
    if (rank == 0) {
        global_vec = getRandomString(count_size_vector);
    }

    int global_sum = CountingAlphabeticCharParallel(global_vec);

    if (rank == 0) {
        int reference_sum = CountingAlphabeticCharSequential(global_vec);
        ASSERT_EQ(reference_sum, global_sum);
    }
}

TEST(Parallel_Operations_MPI, Test_Diff) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::string global_vec;
    const int count_size_vector = 120;
    int global_diff;
    if (rank == 0) {
        global_vec = getRandomString(count_size_vector);
    }
    global_diff = CountingAlphabeticCharParallel(global_vec);

    if (rank == 0) {
        int reference_diff = CountingAlphabeticCharSequential(global_vec);
        ASSERT_EQ(reference_diff, global_diff);
    }
}

TEST(Parallel_Operations_MPI, Test_Diff_2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::string global_vec;
    const int count_size_vector = 120;

    if (rank == 0) {
        global_vec = getRandomString(count_size_vector);
    }

    int global_diff = CountingAlphabeticCharParallel(global_vec);

    if (rank == 0) {
        int reference_diff = CountingAlphabeticCharSequential(global_vec);
        ASSERT_EQ(reference_diff, global_diff);
    }
}

TEST(Parallel_Operations_MPI, Test_Max) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::string global_vec;
    const int count_size_vector = 120;

    if (rank == 0) {
        global_vec = getRandomString(count_size_vector);
    }

    int global_max;
    global_max = CountingAlphabeticCharParallel(global_vec);

    if (rank == 0) {
        int reference_max = CountingAlphabeticCharSequential(global_vec);
        ASSERT_EQ(reference_max, global_max);
    }
}

TEST(Parallel_Operations_MPI, Test_Max_2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::string global_vec;
    const int count_size_vector = 120;

    if (rank == 0) {
        global_vec = getRandomString(count_size_vector);
    }

    int global_max;
    global_max = CountingAlphabeticCharParallel(global_vec);

    if (rank == 0) {
        int reference_max = CountingAlphabeticCharSequential(global_vec);
        ASSERT_EQ(reference_max, global_max);
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
