// Copyright 2022 Tychinin Alexey


#include <gtest/gtest.h>
#include <cmath>
#include "../../../modules/task_1/tychinin_a_sentences_num/sentences_num.h"
#include <gtest-mpi-listener.hpp>

TEST(Sentences_Num, Random_String) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int ch_num = 1234;
    std::string str = task1::generateSentence(ch_num);

    int par_count = task1::countSentences(str, true, ch_num);

    if (rank == 0) {
        int seq_count = task1::countSentences(str);
        EXPECT_EQ(seq_count, par_count);
    }
}

TEST(Sentences_Num, Custom_String) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::string str = "If. You. Are? Writing! Code. Look at the. Mirror!!!!";

    int par_count = task1::countSentences(str, true, str.size());

    if (rank == 0) {
        int seq_count = task1::countSentences(str);
        EXPECT_EQ(seq_count, par_count);
    }
}

TEST(Sentences_Num, Empty_String) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::string global_str;

    int par_count = task1::countSentences("", true, 0);

    if (rank == 0) {
        int seq_count = task1::countSentences(global_str);
        EXPECT_EQ(seq_count, par_count);
    }
}

TEST(Sentences_Num, Single_Word_String) {
    int proc_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    const std::string str = "chemical";

    int par_count = task1::countSentences(str, true, str.size());

    if (proc_rank == 0) {
        int seq_count = task1::countSentences(str);
        EXPECT_EQ(seq_count, par_count);
    }
}

TEST(Sentences_Num, Delim_String) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::string str = "!?!..!.!";

    int par_count = task1::countSentences(str, true, str.size());

    if (rank == 0) {
        int seq_count = task1::countSentences(str);
        EXPECT_EQ(seq_count, par_count);
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
