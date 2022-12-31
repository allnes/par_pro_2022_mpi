// Copyright 2022 Panov Alexey
#include <gtest/gtest.h>
#include <string>
#include "./symbols_diff.h"
#include <gtest-mpi-listener.hpp>


TEST(Parallel_Operations_MPI, Equals_Strings) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::string first = "abcdef";
    std::string second = first;

    int diff = getDifferentSymbolsCountParallel(first, second);

    if (rank == 0) {
        ASSERT_EQ(0, diff);
    }
}

TEST(Parallel_Operations_MPI, Empty_And_NonEmpty_String) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::string first = "abcdef";
    std::string second = "";

    int diff = getDifferentSymbolsCountParallel(first, second);

    if (rank == 0) {
        ASSERT_EQ(first.size(), diff);
    }
}

TEST(Parallel_Operations_MPI, Strings_With_Equal_Size) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::string first = "aaaaaaaa";
    std::string second = "aabababa";

    int diff = getDifferentSymbolsCountParallel(first, second);

    if (rank == 0) {
        ASSERT_EQ(3, diff);
    }
}

TEST(Parallel_Operations_MPI, Strings_With_Diff_Size_1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::string first = "aaaaaaaaaabb";
    std::string second = "aaaabaaa";

    int diff = getDifferentSymbolsCountParallel(first, second);

    if (rank == 0) {
        ASSERT_EQ(5, diff);
    }
}

TEST(Parallel_Operations_MPI, Strings_With_Diff_Size_2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::string first = "bcc";
    std::string second = "abcdefghijk";

    int diff = getDifferentSymbolsCountParallel(first, second);

    if (rank == 0) {
        ASSERT_EQ(second.size() - 1, diff);
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
