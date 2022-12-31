// Copyright 2021 Musin Alexandr
#include <gtest/gtest.h>

#include <random>
#include <string>
#include <gtest-mpi-listener.hpp>

#include "./chars_on_str.h"

TEST(Parallel_Operations_MPI, Test_find_A) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    char *global_str = nullptr;
    const int count_size_str = 10000;
    char sym = 'A';

    if (rank == 0) {
        global_str = getRandomString(count_size_str);
    }

    int global_sum = par_sym_on_str(global_str, count_size_str, sym);

    if (rank == 0) {
        int reference_sum = sym_on_str(global_str, count_size_str, sym);
        ASSERT_EQ(reference_sum, global_sum);
    }
}
TEST(Parallel_Operations_MPI, Test_find_size_100000) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    char *global_str = nullptr;
    const int count_size_str = 10000;
    char sym = 'A';

    if (rank == 0) {
        global_str = getRandomString(count_size_str);
    }

    int global_sum = par_sym_on_str(global_str, count_size_str, sym);

    if (rank == 0) {
        int reference_sum = sym_on_str(global_str, count_size_str, sym);
        ASSERT_EQ(reference_sum, global_sum);
    }
}
TEST(Parallel_Operations_MPI, Test_find_size_1000) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    char *global_str = nullptr;
    const int count_size_str = 1000;
    char sym = 'A';

    if (rank == 0) {
        global_str = getRandomString(count_size_str);
    }

    int global_sum = par_sym_on_str(global_str, count_size_str, sym);

    if (rank == 0) {
        int reference_sum = sym_on_str(global_str, count_size_str, sym);
        ASSERT_EQ(reference_sum, global_sum);
    }
}
TEST(Parallel_Operations_MPI, Test_find_size_2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    char *global_str = nullptr;
    const int count_size_str = 2;
    char sym = 'A';

    if (rank == 0) {
        global_str = getRandomString(count_size_str);
    }

    int global_sum = par_sym_on_str(global_str, count_size_str, sym);

    if (rank == 0) {
        int reference_sum = sym_on_str(global_str, count_size_str, sym);
        ASSERT_EQ(reference_sum, global_sum);
    }
}
TEST(Parallel_Operations_MPI, Test_find_size_10) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    char *global_str = nullptr;
    const int count_size_str = 10;
    char sym = 'A';

    if (rank == 0) {
        global_str = getRandomString(count_size_str);
    }

    int global_sum = par_sym_on_str(global_str, count_size_str, sym);

    if (rank == 0) {
        int reference_sum = sym_on_str(global_str, count_size_str, sym);
        ASSERT_EQ(reference_sum, global_sum);
    }
}
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners &listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}
