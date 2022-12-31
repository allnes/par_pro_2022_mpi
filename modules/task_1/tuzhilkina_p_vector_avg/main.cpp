// Copyright 2022 Tuzhilkina Polina
#include <gtest/gtest.h>
#include <vector>
#include "./vector_avg.h"
#include <gtest-mpi-listener.hpp>

void runVecAvgTest(int glob_vec_size) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> globVec;

    if (rank == 0) {
        globVec = getRandomVec(glob_vec_size);
        // printVecElements(globVec);
    }

    double resultParallel = getAvgPar(globVec, glob_vec_size);

    if (rank == 0) {
        double resultSequential = getAvgSeq(globVec);
        ASSERT_EQ(resultSequential, resultParallel);
    }
}

TEST(Vector_Average_MPI, Test_Vector_Average_1) {
    runVecAvgTest(5);
}

TEST(Vector_Average_MPI, Test_Vector_Average_2) {
    runVecAvgTest(10);
}

TEST(Vector_Average_MPI, Test_Vector_Average_3) {
    runVecAvgTest(100);
}

TEST(Vector_Average_MPI, Test_Vector_Average_4) {
    runVecAvgTest(999);
}

TEST(Vector_Average_MPI, Test_Vector_Average_5) {
    runVecAvgTest(1000);
}

TEST(Vector_Average_MPI, Test_Vector_Average_6) {
    runVecAvgTest(1000000);
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
