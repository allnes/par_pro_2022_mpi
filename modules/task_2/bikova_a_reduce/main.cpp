// Copyright 2022 Alexandra Bikova
#include<gtest/gtest.h>
#include <iostream>
#include"../../../modules/task_2/bikova_a_reduce/Reduce.h"
#include<gtest-mpi-listener.hpp>
extern int ProcRank;
extern int ProcSize;
TEST(Reduce, Test_Max1) {
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcSize);
    std::vector<int>a, b;
    DoWork(MPI_MAX, &a, &b);
    if (ProcRank == 0) {
        ASSERT_EQ(a, b);
    }
}
TEST(Reduce, Test_Max2) {
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcSize);
    std::vector<int>a, b;
    DoWork(MPI_MAX, &a, &b);
    if (ProcRank == 0) {
        ASSERT_EQ(a, b);
    }
}
TEST(Reduce, Test_Min1) {
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcSize);
    std::vector<int>a, b;
    DoWork(MPI_MIN, &a, &b);
    if (ProcRank == 0) {
        ASSERT_EQ(a, b);
    }
}
TEST(Reduce, Test_Min2) {
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcSize);
    std::vector<int>a, b;
    DoWork(MPI_MIN, &a, &b);
    if (ProcRank == 0) {
        ASSERT_EQ(a, b);
    }
}
TEST(Reduce, Test_Sum) {
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcSize);
    std::vector<int>a, b;
    DoWork(MPI_SUM, &a, &b);
    if (ProcRank == 0) {
        ASSERT_EQ(a, b);
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

