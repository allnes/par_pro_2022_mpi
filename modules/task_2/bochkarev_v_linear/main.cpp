// Copyright 2022 Bochkarev Vladimir
#include <gtest/gtest.h>
#include <vector>
#include "./linear.h"
#include <gtest-mpi-listener.hpp>

TEST(linear_MPI, test1) {
    int rank, size;
    int mes;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size > 1) {
        if (rank == 0) {
            mes = 100;
        }

        send(&mes, 1, MPI_INT, 0, 1, 0, MPI_COMM_WORLD);

        if (rank == 1) {
            ASSERT_EQ(mes, 100);
        }
    }
}

TEST(linear_MPI, test2) {
    int rank, size;
    int mes;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size > 1) {
        if (rank == 1) {
            mes = 100;
        }

        send(&mes, 1, MPI_INT, 1, 0, 0, MPI_COMM_WORLD);

        if (rank == 0) {
            ASSERT_EQ(mes, 100);
        }
    }
}

TEST(linear_MPI, test3) {
    int rank, size;
    int mes;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size > 1) {
        if (rank == 0) {
            mes = 100;
        }

        send(&mes, 1, MPI_INT, 0, size - 1, 0, MPI_COMM_WORLD);

        if (rank == size - 1) {
            ASSERT_EQ(mes, 100);
        }
    }
}

TEST(linear_MPI, test4) {
    int rank, size;
    int mes;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size > 1) {
        if (rank == size - 1) {
            mes = 100;
        }

        send(&mes, 1, MPI_INT, size - 1, 0, 0, MPI_COMM_WORLD);

        if (rank == 0) {
            ASSERT_EQ(mes, 100);
        }
    }
}

TEST(linear_MPI, test5) {
    int rank, size;
    int mes;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size > 1) {
        if (rank == size - 2) {
            mes = 100;
        }

        send(&mes, 1, MPI_INT, size - 2, size - 1, 0, MPI_COMM_WORLD);

        if (rank == size - 1) {
            ASSERT_EQ(mes, 100);
        }
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
