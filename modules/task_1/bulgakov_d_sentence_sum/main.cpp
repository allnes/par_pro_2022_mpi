// Copyright 2022 Bulgakov Daniil

#include <gtest/gtest.h>
#include <string>
#include <iostream>
#include <fstream>
#include "./sentence_sum.h"
#include <gtest-mpi-listener.hpp>

// #define debug


// Simple test
TEST(Parallel_Operations_MPI, Test_1) {
    std::string test_string =
            "But I must explain to you how all this mistaken idea of denouncing pleasure and praising pain was bo"
            "rn and I will give you a complete account of the system, and expound the actual teachings of the gre"
            "at explorer of the truth, the master-builder of human happiness. No one rejects, dislikes, or avoids"
            " pleasure itself, because it is pleasure, but because those who do not know how to pursue pleasure r"
            "ationally encounter consequences that are extremely painful. Nor again is there anyone who loves or "
            "pursues or desires to obtain pain of itself, because it is pain, but because occasionally circumstan"
            "ces occur in which toil and pain can procure him some great pleasure. To take a trivial example, whi"
            "ch of us ever undertakes laborious physical exercise, except to obtain some advantage from it? But w"
            "ho has any right to find fault with a man who chooses to enjoy a pleasure that has no annoying conse";

    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);


    int global_sum = parallelSentenceCount(test_string);

    if (rank == 0) {
        int local_cnt = computeSenteceCount(test_string);
        #ifdef debug
        std::cout << "Result seq =" << computeSenteceCount(test_string) << std::endl;
        std::cout << "Result parallel =" << global_sum << std::endl;
        #endif
        ASSERT_EQ(local_cnt, global_sum);
    }
}

// Test if string sentences is ""
TEST(Parallel_Operations_MPI, Test_2) {
    std::string test_string = "";
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);


    int global_sum = parallelSentenceCount(test_string);

    if (rank == 0) {
        int local_cnt = computeSenteceCount(test_string);
        #ifdef debug
        std::cout << "Result seq =" << computeSenteceCount(test_string) << std::endl;
        std::cout << "Result parallel =" << global_sum << std::endl;
        #endif
        ASSERT_EQ(local_cnt, global_sum);
    }
}

// Test if string is a single word
TEST(Parallel_Operations_MPI, Test_3) {
    std::string test_string = "A!";
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);


    int global_sum = parallelSentenceCount(test_string);

    if (rank == 0) {
        int local_cnt = computeSenteceCount(test_string);
        #ifdef debug
        std::cout << "Result seq =" << computeSenteceCount(test_string) << std::endl;
        std::cout << "Result parallel =" << global_sum << std::endl;
        #endif
        ASSERT_EQ(local_cnt, global_sum);
    }
}

// A sentence contains line ending characters typeof ?! or ... or !! and so on
TEST(Parallel_Operations_MPI, Test_4) {
    std::string test_string = "One was a car!? Two was a car?! Three was a car... Four was a car!?";
    test_string = " ";
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);


    int global_sum = parallelSentenceCount(test_string);

    if (rank == 0) {
        int local_cnt = computeSenteceCount(test_string);
        #ifdef debug
        std::cout << "Result seq =" << computeSenteceCount(test_string) << std::endl;
        std::cout << "Result parallel =" << global_sum << std::endl;
        #endif
        ASSERT_EQ(local_cnt, global_sum);
    }
}

// A sentence made of digits
TEST(Parallel_Operations_MPI, Test_5) {
    std::string test_string = "1921!22231.123?1222!!";
    test_string = " ";
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);


    int global_sum = parallelSentenceCount(test_string);

    if (rank == 0) {
        int local_cnt = computeSenteceCount(test_string);
        #ifdef debug
        std::cout << "Result seq =" << computeSenteceCount(test_string) << std::endl;
        std::cout << "Result parallel =" << global_sum << std::endl;
        #endif
        ASSERT_EQ(local_cnt, global_sum);
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
