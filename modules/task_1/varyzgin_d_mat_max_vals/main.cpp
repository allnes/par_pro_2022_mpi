// Copyright 2023 Varyzgin Dmitriy
#include <gtest/gtest.h>
#include <vector>
#include "./mat_max_vals.h"
#include <gtest-mpi-listener.hpp>

void testRun(int m, int n) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> globMatrix;

    if (rank == 0) globMatrix = genRandomMatrix(m, n);

    std::vector<int> rSeq;
    if (rank == 0) rSeq = getMaxValsSequential(m, n, globMatrix);

    std::vector<int> rPar = getMaxValsParallel(m, n, globMatrix);

    if (rank == 0) ASSERT_EQ(rPar, rSeq);
}

TEST(MatMaxValsMPI, MatMaxValsMPI_1) { testRun(2, 2); }

TEST(MatMaxValsMPI, MatMaxValsMPI_2) { testRun(3, 3); }

TEST(MatMaxValsMPI, MatMaxValsMPI_3) { testRun(3, 4); }

TEST(MatMaxValsMPI, MatMaxValsMPI_4) { testRun(4, 7); }

TEST(MatMaxValsMPI, MatMaxValsMPI_5) { testRun(19, 19); }

TEST(MatMaxValsMPI, MatMaxValsMPI_6) { testRun(50, 50); }

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
