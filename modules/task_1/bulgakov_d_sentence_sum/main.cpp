// Copyright 2022 Bulgakov Daniil

#include <gtest/gtest.h>
#include <string>
#include <iostream>
#include "./sentence_sum.h"
#include <gtest-mpi-listener.hpp>

TEST(Parallel_Operations_MPI, Test_Sum_10) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::string global_str;
    const int word_len =  10;
    int str_len = -1;


    if (rank == 0) {
        global_str = getRandomText(word_len);
        str_len = global_str.length();
        #ifdef DEBUG
        std::cout << "LEN : " << str_len << ">" << std::endl;
        std::cout << "TEXT : " << global_str << ">" << std::endl;
        #endif
    }

    MPI_Bcast(&str_len, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int global_sum = getParallelOperations(global_str, str_len);


    if (rank == 0) {
        int ref_sum = getSequentialOperations(global_str);
        ASSERT_EQ(ref_sum, global_sum);
    }
}

TEST(Parallel_Operations_MPI, Test_Sum_100) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::string global_str;
    const int word_len =  100;
    int str_len = -1;


    if (rank == 0) {
        global_str = getRandomText(word_len);
        str_len = global_str.length();
    }

    MPI_Bcast(&str_len, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int global_sum = getParallelOperations(global_str, str_len);

    if (rank == 0) {
        int ref_sum = getSequentialOperations(global_str);
        ASSERT_EQ(ref_sum, global_sum);
    }
}
TEST(Parallel_Operations_MPI, Test_Sum_1000) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::string global_str;
    const int word_len =  1000;
    int str_len = -1;


    if (rank == 0) {
        global_str = getRandomText(word_len);
        str_len = global_str.length();
    }

    MPI_Bcast(&str_len, 1, MPI_INT, 0, MPI_COMM_WORLD);
    int global_sum = getParallelOperations(global_str, str_len);

    if (rank == 0) {
        int ref_sum = getSequentialOperations(global_str);
        ASSERT_EQ(ref_sum, global_sum);
    }
}
TEST(Parallel_Operations_MPI, Test_Sum_10000) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::string global_str;
    const int word_len =  10000;
    int str_len = -1;


    if (rank == 0) {
        global_str = getRandomText(word_len);
        str_len = global_str.length();
    }

    MPI_Bcast(&str_len, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int global_sum = getParallelOperations(global_str, str_len);

    if (rank == 0) {
        int ref_sum = getSequentialOperations(global_str);
        ASSERT_EQ(ref_sum, global_sum);
    }
}

TEST(Parallel_Operations_MPI, Test_Sum_100000) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::string global_str;
    const int word_len =  100000;
    int str_len = -1;

    if (rank == 0) {
        global_str = getRandomText(word_len);
        str_len = global_str.length();
    }

    MPI_Bcast(&str_len, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int global_sum = getParallelOperations(global_str, str_len);

    if (rank == 0) {
        int ref_sum = getSequentialOperations(global_str);
        ASSERT_EQ(ref_sum, global_sum);
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
