// Copyright 2022 Ivlev A
#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include "./mult_ccs.h"
#include <gtest-mpi-listener.hpp>


TEST(Test_mult_ccs_MPI, Test_0) {
    int size, rank;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int m = 6;
    int n = 5;
    int val_n_a = 4;
    matrix_ccs a(m, n, val_n_a);
    int val_n_b = 5;
    matrix_ccs b(n, m, val_n_b);

    if (rank == 0) {
        a.create_rand();
        b.create_rand();
    }

    matrix_ccs d = a.mpi_mult(b);

    if (rank == 0) {
        matrix_ccs c = a.mult(b);
        EXPECT_EQ(c, d);
    }
}

TEST(Test_mult_ccs_MPI, Test_1) {
    int size, rank;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int m = 5;
    int n = 8;
    int val_n_a = 4;
    matrix_ccs a(m, n, val_n_a);
    int val_n_b = 4;
    matrix_ccs b(n, m, val_n_b);

    if (rank == 0) {
        a.create_rand();
        b.create_rand();
    }

    matrix_ccs d = a.mpi_mult(b);

    if (rank == 0) {
        matrix_ccs c = a.mult(b);
        EXPECT_EQ(c, d);
    }
}

TEST(Test_mult_ccs_MPI, Test_2) {
    int size, rank;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int m = 5;
    int n = 5;
    int val_n_a = 3;
    matrix_ccs a(m, n, val_n_a);
    int val_n_b = 4;
    matrix_ccs b(n, m, val_n_b);

    if (rank == 0) {
        a.create_rand();
        b.create_rand();
    }

    matrix_ccs d = a.mpi_mult(b);

    if (rank == 0) {
        matrix_ccs c = a.mult(b);
        EXPECT_EQ(c, d);
    }
}

TEST(Test_mult_ccs_MPI, Test_3) {
    int size, rank;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int m = 10;
    int n = 10;
    int val_n_a = 9;
    matrix_ccs a(m, n, val_n_a);
    int val_n_b = 9;
    matrix_ccs b(n, m, val_n_b);

    if (rank == 0) {
        a.create_rand();
        b.create_rand();
    }

    matrix_ccs d = a.mpi_mult(b);

    if (rank == 0) {
        matrix_ccs c = a.mult(b);
        EXPECT_EQ(c, d);
    }
}

TEST(Test_mult_ccs_MPI, Test_4) {
    int size, rank;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int m = 20;
    int n = 5;
    int val_n_a = 4;
    matrix_ccs a(m, n, val_n_a);
    int val_n_b = 17;
    matrix_ccs b(n, m, val_n_b);

    if (rank == 0) {
        a.create_rand();
        b.create_rand();
    }

    matrix_ccs d = a.mpi_mult(b);

    if (rank == 0) {
        matrix_ccs c = a.mult(b);
        EXPECT_EQ(c, d);
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
