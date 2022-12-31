// Copyright 2022 Uvarkin Ilya
#include <gtest/gtest.h>
#include "./hypercube.h"
#include <gtest-mpi-listener.hpp>

// cmake --build . --target uvarkin_i_hypercube_mpi --config RELEASE
// mpirun -n 6 uvarkin_i_hypercube_mpi

TEST(hypercube_mpi, test1) {
    int rank, size;
    int test_data = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int next = getNext(0, 3);
    ASSERT_EQ(next, 1);
}

TEST(hypercube_mpi, test2) {
    int rank, size;
    int test_data = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size >= 2) {
        if (rank == 0) {
            test_data = 42;
        }

        HypercubeSend(&test_data, 1, MPI_INT, 0, 1, 0, MPI_COMM_WORLD);

        if (rank == 1) {
            ASSERT_EQ(test_data, 42);
        }
    }
}

TEST(hypercube_mpi, test3) {
    int rank, size;
    int test_data = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size >= 4) {
        if (rank == 0) {
            test_data = 42;
        }

        HypercubeSend(&test_data, 1, MPI_INT, 0, 3, 0, MPI_COMM_WORLD);

        if (rank == 3) {
            ASSERT_EQ(test_data, 42);
        }
    }
}

TEST(hypercube_mpi, test4) {
    int rank, size;
    int test_data = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size >= 4) {
        if (rank == 3) {
            test_data = 42;
        }

        HypercubeSend(&test_data, 1, MPI_INT, 3, 0, 0, MPI_COMM_WORLD);

        if (rank == 0) {
            ASSERT_EQ(test_data, 42);
        }
    }
}

TEST(hypercube_mpi, test5) {
    int rank, size;
    int test_data = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size >= 4) {
        if (rank == 0) {
            test_data = 42;
        }

        HypercubeSend(&test_data, 1, MPI_INT, 0, 2, 0, MPI_COMM_WORLD);

        if (rank == 2) {
            ASSERT_EQ(test_data, 42);
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

