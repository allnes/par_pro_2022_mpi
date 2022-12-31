// Copyright 2022 Zorin Oleg
#include <gtest/gtest.h>
#include "./non_char.h"
#include <gtest-mpi-listener.hpp>

TEST(Parallel_Operations_MPI, Test_Sequential_Count) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        const char *global_str = "banana";
        const char *global_compare_str = "apple";
        int count = countNonMatchingCharSequential(global_str, 6, global_compare_str, 5);
        ASSERT_EQ(3, count);
    }
}

TEST(Parallel_Operations_MPI, Test_Parallel_Count) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const char *global_str = "banana";
    const char *global_compare_str = "apple";


    int count = countNonMatchingCharParallel(global_str, 6, global_compare_str, 5);

    if (rank == 0) {
        ASSERT_EQ(3, count);
    }
}

TEST(Parallel_Operations_MPI, Test_Random_String_Same_Size) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const char *global_str = "abcdefghij";
    const char *global_compare_str = "JKAHJLASll";


    int global_count = countNonMatchingCharParallel(
            global_str, 10, global_compare_str, 10);

    if (rank == 0) {
        int reference_count = countNonMatchingCharSequential(
                global_str, 10, global_compare_str, 10);
        ASSERT_EQ(reference_count, global_count);
    }
}

TEST(Parallel_Operations_MPI, Test_Random_String_Diff_Size) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    const char *global_str = "1a2b3c4d5e6f7g8";
    const char *global_compare_str = "1234567890";

    int global_count = countNonMatchingCharParallel(
            global_str, 15, global_compare_str, 10);

    if (rank == 0) {
        int reference_count = countNonMatchingCharSequential(global_str, 15, global_compare_str, 10);
        ASSERT_EQ(reference_count, global_count);
    }
}

TEST(Parallel_Operations_MPI, Test_Random_String_Diff_Size_2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const char *global_str = "1234567890";
    const char *global_compare_str = "1a2b3c4d5e6f7g8";


    int global_count = countNonMatchingCharParallel(
            global_str, 10, global_compare_str, 15);

    if (rank == 0) {
        int reference_count = countNonMatchingCharSequential(
                global_str, 10, global_compare_str, 15);
        ASSERT_EQ(reference_count, global_count);
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
