// Copyright 2022 Muhin Vadim
#include <gtest/gtest.h>
#include <vector>
#include "./check_lex_order_strings.h"
#include <gtest-mpi-listener.hpp>

TEST(Generation_String, can_generate_string) {
    ASSERT_NO_THROW(getRandomString(10));
}

TEST(Sequential_Operations_MPI, getSequentialOperations_can_work) {
    std::vector<char> str1 = getRandomString(10);
    std::vector<char> str2 = getRandomString(10);
    ASSERT_NO_THROW(getSequentialOperations(str1, str2));
}

TEST(Sequential_Operations_MPI, can_define_equil_strings) {
    std::vector<char> str1 = { 'a', 'a', 'a' };
    std::vector<char> str2 = { 'a', 'a', 'a' };
    int res = 0;
    ASSERT_EQ(res, getSequentialOperations(str1, str2));
}

TEST(Sequential_Operations_MPI, can_define_smaller_string) {
    std::vector<char> str1 = { 'a', 'a', 'a' };
    std::vector<char> str2 = { 'a', 'a', 'b' };
    int res = -1;
    ASSERT_EQ(res, getSequentialOperations(str1, str2));
}

TEST(Sequential_Operations_MPI, can_define_larger_string) {
    std::vector<char> str1 = { 'a', 'a', 'b' };
    std::vector<char> str2 = { 'a', 'a', 'a' };
    int res = 1;
    ASSERT_EQ(res, getSequentialOperations(str1, str2));
}

TEST(Parallel_Operations_MPI, getParallelOperations_can_work) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<char>::size_type size = 10;
    std::vector<char> str1(size), str2(size);

    if (rank == 0) {
        str1 = getRandomString(size);
        str2 = getRandomString(size);
    }

    ASSERT_NO_THROW(getParallelOperations(str1, str2, size));
}

TEST(Parallel_Operations_MPI, getParallelOperations_works_correctly) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<char>::size_type size = 100;
    std::vector<char> str1, str2;

    if (rank == 0) {
        str1 = getRandomString(size);
        str2 = getRandomString(size);
    }

    int res = getParallelOperations(str1, str2, size);

    if (rank == 0) {
        int expected_res = getSequentialOperations(str1, str2);

        ASSERT_EQ(res, expected_res);
    }
}

TEST(Parallel_Operations_MPI, getParallelOperations_works_correctly_with_odd_size) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<char>::size_type size = 101;
    std::vector<char> str1, str2;

    if (rank == 0) {
        str1 = getRandomString(size);
        str2 = getRandomString(size);
    }

    int res = getParallelOperations(str1, str2, size);

    if (rank == 0) {
        int expected_res = getSequentialOperations(str1, str2);

        ASSERT_EQ(res, expected_res);
    }
}

TEST(Parallel_Operations_MPI, getParallelOperations_can_define_equil_strings) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<char>::size_type size = 15;
    std::vector<char> str1 = { 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a' };
    std::vector<char> str2 = { 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a' };

    int res = getParallelOperations(str1, str2, size);

    if (rank == 0) {
        int expected_res = 0;

        ASSERT_EQ(res, expected_res);
    }
}

TEST(Parallel_Operations_MPI, getParallelOperations_can_define_smaller_strings) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<char>::size_type size = 15;
    std::vector<char> str1 = { 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a' };
    std::vector<char> str2 = { 'a', 'b', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a' };

    int res = getParallelOperations(str1, str2, size);

    if (rank == 0) {
        int expected_res = -1;

        ASSERT_EQ(res, expected_res);
    }
}

TEST(Parallel_Operations_MPI, getParallelOperations_can_define_larger_strings) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<char>::size_type size = 15;
    std::vector<char> str1 = { 'a', 'b', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a' };
    std::vector<char> str2 = { 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a' };

    int res = getParallelOperations(str1, str2, size);

    if (rank == 0) {
        int expected_res = 1;

        ASSERT_EQ(res, expected_res);
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
