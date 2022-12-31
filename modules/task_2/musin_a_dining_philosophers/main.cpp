// Copyright 2021 Musin Alexandr
#include <gtest/gtest.h>

#include <gtest-mpi-listener.hpp>

#include "./dining_philosophers.h"

TEST(Parallel_Operations_MPI, Test_Plus_0) {
    int incriment_parameter = 0;
    int rank;
    int size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (rank == 0) {
        int res = manager(size);
        ASSERT_EQ(res, incriment_parameter * (size - 1));
    } else {
        philosoph(rank, incriment_parameter);
    }
}
TEST(Parallel_Operations_MPI, Test_Minus_1) {
    int incriment_parameter = -1;
    int rank;
    int size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (rank == 0) {
        int res = manager(size);
        ASSERT_EQ(res, incriment_parameter * (size - 1));
    } else {
        philosoph(rank, incriment_parameter);
    }
}

TEST(Parallel_Operations_MPI, Test_Plus_2) {
    int incriment_parameter = 2;
    int rank;
    int size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (rank == 0) {
        int res = manager(size);
        ASSERT_EQ(res, incriment_parameter * (size - 1));
    } else {
        philosoph(rank, incriment_parameter);
    }
}

TEST(Parallel_Operations_MPI, Tes_Plus_3) {
    int incriment_parameter = 3;
    int rank;
    int size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (rank == 0) {
        int res = manager(size);
        ASSERT_EQ(res, incriment_parameter * (size - 1));
    } else {
        philosoph(rank, incriment_parameter);
    }
}

TEST(Parallel_Operations_MPI, Test_Plus_And_Minus) {
    int rank;
    int size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int examination = size % 2 == 0 ? -1 : 0;

    if (rank == 0) {
        int res = manager(size);
        ASSERT_EQ(res, examination);
    } else {
        if (rank % 2 == 0) {
            philosoph(rank, 1);
        }
        if (rank % 2 != 0) {
            philosoph(rank, -1);
        }
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
