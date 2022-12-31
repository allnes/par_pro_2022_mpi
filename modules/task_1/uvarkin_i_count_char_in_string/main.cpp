// Copyright 2022 Uvarkin Ilya
#include <gtest/gtest.h>
#include "./count_char_in_string.h"
#include <gtest-mpi-listener.hpp>

TEST(count_char_in_string, test1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::string str = "";
    int count = getCountCharInStringParallel(str, 'a');

    if (rank == 0) {
        ASSERT_EQ(0, count);
    }
}

TEST(count_char_in_string, test2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::string str = "aaaaaaaaaa";
    int count = getCountCharInStringParallel(str, 'a');

    if (rank == 0) {
        ASSERT_EQ(10, count);
    }
}

TEST(count_char_in_string, test3) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::string str = "some text without ch";
    int count = getCountCharInStringParallel(str, 'a');

    if (rank == 0) {
        ASSERT_EQ(0, count);
    }
}

TEST(count_char_in_string, test4) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::string str = "This_is_an_example.";
    int count = getCountCharInStringParallel(str, 'i');

    if (rank == 0) {
        ASSERT_EQ(2, count);
    }
}

TEST(count_char_in_string, test5) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::string str = "some text, some text";
    int count = getCountCharInStringParallel(str, 'e');

    if (rank == 0) {
        ASSERT_EQ(4, count);
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
