// Copyright 2022 Pikin Ilya
#include <gtest/gtest.h>
#include <vector>
#include <iostream>
#include <gtest-mpi-listener.hpp>
#include "./lex_order.h"

TEST(Parallel_Comparison, Small_test) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::string s1;
    std::string s2;

    Result sequentialResult;

    if (rank == 0) {
        //  std::cout << "Processes number: " << size << std::endl;

        s1 = getRandomString(20);
        s2 = s1;
        s2 = changeSomeChars(s2, 3);

        //  std::cout << "A: " << s1 << std::endl << "B: " << s2 << std::endl;
        sequentialResult = getSequentialCompare(s1, s2);
        //  std::cout << "Sequential result: A is "
        //      << getStrFromRes(sequentialResult)
        //      << " B" << std::endl;
    }

    Result parallelResult = getParallelCompare(s1, s2);

    if (rank == 0) {
        //  std::cout << "Parallel result: A is "
        //      << getStrFromRes(parallelResult)
        //      << " B" << std::endl;
        ASSERT_EQ(sequentialResult, parallelResult);
    }
}

TEST(Parallel_Comparison, Compare_strings_of_the_same_len) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::string s1;
    std::string s2;

    Result sequentialResult;

    if (rank == 0) {
        //  std::cout << "Processes number: " << size << std::endl;

        s1 = getRandomString(111111);
        s2 = s1;
        s2 = changeSomeChars(s2, 10);

        sequentialResult =
            getSequentialCompare(s1, s2);
        //  std::cout << "Sequential result: A is "
        //      << getStrFromRes(sequentialResult)
        //      << " B" << std::endl;
    }

    Result parallelResult = getParallelCompare(s1, s2);

    if (rank == 0) {
        //  std::cout << "Parallel result: A is "
        //      << getStrFromRes(parallelResult)
        //      << " B" << std::endl;
        ASSERT_EQ(sequentialResult, parallelResult);
    }
}

TEST(Parallel_Comparison, A_is_greater_than_B) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::string s1;
    std::string s2;

    Result sequentialResult;

    if (rank == 0) {
        //  std::cout << "Processes number: " << size << std::endl;

        s1 = std::string(111111, 'a');
        s2 = s1;
        s1[100000] = 'b';

        sequentialResult = getSequentialCompare(s1, s2);
        //  std::cout << "Sequential result: A is "
        //      << getStrFromRes(sequentialResult)
        //      << " B" << std::endl;
    }

    Result parallelResult = getParallelCompare(s1, s2);

    if (rank == 0) {
        //  std::cout << "Parallel result: A is "
        //      << getStrFromRes(parallelResult)
        //      << " B" << std::endl;
        ASSERT_EQ(Result::greater_than, parallelResult);
        ASSERT_EQ(sequentialResult, parallelResult);
    }
}

TEST(Parallel_Comparison, A_is_less_than_B) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::string s1;
    std::string s2;

    Result sequentialResult;

    if (rank == 0) {
        //  std::cout << "Processes number: " << size << std::endl;

        s1 = std::string(111111, 'a');
        s2 = s1;
        s2[100000] = 'b';

        sequentialResult = getSequentialCompare(s1, s2);
        //  std::cout << "Sequential result: A is "
        //      << getStrFromRes(sequentialResult)
        //      << " B" << std::endl;
    }

    Result parallelResult = getParallelCompare(s1, s2);

    if (rank == 0) {
        //  std::cout << "Parallel result: A is "
        //      << getStrFromRes(parallelResult)
        //      << " B" << std::endl;
        ASSERT_EQ(Result::less_than, parallelResult);
        ASSERT_EQ(sequentialResult, parallelResult);
    }
}

TEST(Parallel_Comparison, A_is_equal_B) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::string s1;
    std::string s2;

    Result sequentialResult;

    if (rank == 0) {
        //  std::cout << "Processes number: " << size << std::endl;

        s1 = getRandomString(111111);
        s2 = s1;

        sequentialResult = getSequentialCompare(s1, s2);
        //  std::cout << "Sequential result: A is "
        //      << getStrFromRes(sequentialResult)
        //      << " B" << std::endl;
    }

    Result parallelResult = getParallelCompare(s1, s2);

    if (rank == 0) {
        //  std::cout << "Parallel result: A is "
        //      << getStrFromRes(parallelResult)
        //      << " B" << std::endl;
        ASSERT_EQ(Result::equal, parallelResult);
        ASSERT_EQ(sequentialResult, parallelResult);
    }
}

TEST(Parallel_Comparison, Compare_strings_with_diff_len) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::string s1;
    std::string s2;

    Result sequentialResult;
    Result parallelResult;

    if (rank == 0) {
        //  std::cout << "Processes number: " << size << std::endl;

        s2 = getRandomString(120000);
        // s1 - prefix of s2 => s1<s2
        s1 = std::string(s2, 0ll, 100000ll);

        sequentialResult = getSequentialCompare(s1, s2);
        //  std::cout << "Sequential result: A is "
        //      << getStrFromRes(sequentialResult)
        //      << " B" << std::endl;
    }

    parallelResult = getParallelCompare(s1, s2);

    if (rank == 0) {
        //  std::cout << "Parallel result: A is "
        //      << getStrFromRes(parallelResult)
        //      << " B" << std::endl;

        ASSERT_EQ(sequentialResult, parallelResult);
    }
}

/*  TEST(Parallel_Comparison, Time_test) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::string s1;
    std::string s2;

    Result sequentialResult;
    Result parallelResult;

    if (rank == 0) {
        std::cout << "Processes number: " << size << std::endl;

        s1 = std::string(100 * 1000 * 1000, 'a');
        s2 = std::string(100 * 1000 * 1000, 'a');
        s1[99999998] = 'b';  // 99.999.998

        double par_start, seq_start, par_end, seq_end, seq_time, par_time;

        seq_start = MPI_Wtime();
        sequentialResult = getSequentialCompare(s1, s2);
        seq_end = MPI_Wtime();
        seq_time = seq_end - seq_start;

        par_start = MPI_Wtime();
        parallelResult = getParallelCompare(s1, s2);
        par_end = MPI_Wtime();
        par_time = par_end - par_start;

        std::cout << "Sequential result: A is "
            << getStrFromRes(sequentialResult)
            << " B" << std::endl;
        std::cout << "Sequential time: " << seq_time << std::endl;
        std::cout << "Parallel result: A is "
            << getStrFromRes(parallelResult)
            << " B" << std::endl;
        std::cout << "Parallel time: " << par_time << std::endl << std::endl;

        ASSERT_EQ(sequentialResult, parallelResult);
    } else { getParallelCompare(s1, s2); }
}  */

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
