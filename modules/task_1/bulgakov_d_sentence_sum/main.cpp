#include <gtest/gtest.h>
#include <string>
#include "./sentence_sum.h"
#include <gtest-mpi-listener.hpp>

TEST(Parallel_Operations_MPI, Test_Sum_10) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::string global_str;
    const int str_len = 10;

    if (rank == 0){
        global_str = getRandomText(str_len);
    }

    int global_sum = getParallelOperations(global_str, global_str.length());

    if (rank == 0){
        int ref_sum = getSequentialOperations(global_str);
        ASSERT_EQ(ref_sum, global_sum);
    }
}

TEST(Parallel_Operations_MPI, Test_Sum_100) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::string global_str;
    const int str_len = 100;

    if (rank == 0){
        global_str = getRandomText(str_len);
    }

    int global_sum = getParallelOperations(global_str, global_str.length());

    if (rank == 0){
        int ref_sum = getSequentialOperations(global_str);
        ASSERT_EQ(ref_sum, global_sum);
    }
}
TEST(Parallel_Operations_MPI, Test_Sum_1000) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::string global_str;
    const int str_len = 1000;

    if (rank == 0){
        global_str = getRandomText(str_len);
    }

    int global_sum = getParallelOperations(global_str, global_str.length());

    if (rank == 0){
        int ref_sum = getSequentialOperations(global_str);
        ASSERT_EQ(ref_sum, global_sum);
    }
}
TEST(Parallel_Operations_MPI, Test_Sum_10000) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::string global_str;
    const int str_len = 10000;

    if (rank == 0){
        global_str = getRandomText(str_len);
    }

    int global_sum = getParallelOperations(global_str, global_str.length());

    if (rank == 0){
        int ref_sum = getSequentialOperations(global_str);
        ASSERT_EQ(ref_sum, global_sum);
    }
}

TEST(Parallel_Operations_MPI, Test_Sum_100000) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::string global_str;
    const int str_len = 100000;

    if (rank == 0){
        global_str = getRandomText(str_len);
    }

    int global_sum = getParallelOperations(global_str, global_str.length());

    if (rank == 0){
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