// Copyright 2022 Artemev Alexej

#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include "./rect_intg.h"


float lin(float x) { return x; }
float qubic(float x) { return x * x * x; }

TEST(Rect_Intg, Test_L_Sequential_Lin) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        float res = lrect_intg_seq(0.0, 5.0, 1000, lin);
        float exp = 12.5;
        EXPECT_NEAR(res, exp, 0.01251);
    }
}

TEST(Rect_Intg, Test_L_Sequential_Qubic) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        float res = lrect_intg_seq(0.0, 10.0, 1000, qubic);
        float exp = 333.33333;
        EXPECT_NEAR(res, exp, 1.0);
    }
}

TEST(Rect_Intg, Test_L_Parallel_Lin) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    float res = lrect_intg_par(0.0, 5.0, 1000, lin);
    if (rank == 0) {
        float exp = lrect_intg_seq(0.0, 5.0, 1000, lin);
        EXPECT_NEAR(res, exp, 0.025);
    }
}

TEST(Rect_Intg, Test_L_Parallel_Qubic) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    float res = lrect_intg_par(0.0, 10.0, 1000, qubic);
    if (rank == 0) {
        float exp = lrect_intg_seq(0.0, 10.0, 1000, qubic);
        EXPECT_NEAR(res, exp, 1.0);
    }
}

TEST(Rect_Intg, Test_Midpoint_Sequential_Lin) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        float res = mp_intg_seq(0.0, 5.0, 1000, lin);
        float exp = 12.5;
        EXPECT_NEAR(res, exp, 5.2e-6);  
    }
}

TEST(Rect_Intg, Test_Midpoint_Sequential_Qubic) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        float res = mp_intg_seq(0.0, 10.0, 1000, qubic);
        float exp = 333.33333;
        EXPECT_NEAR(res, exp, 8.4e-5);
    }
}

TEST(Rect_Intg, Test_Midpoint_Parallel_Lin) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    float res = mp_intg_par(0.0, 5.0, 1000, lin);
    if (rank == 0) {
        float exp = mp_intg_seq(0.0, 5.0, 1000, lin);
        EXPECT_NEAR(res, exp, 0.4e-5);
    }
}

TEST(Rect_Intg, Test_Midpoint_Parallel_Qubic) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    float res = mp_intg_par(0.0, 10.0, 1000, qubic);
    if (rank == 0) {
        float exp = mp_intg_seq(0.0, 10.0, 1000, qubic);
        EXPECT_NEAR(res, exp, 8.33e-5);
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
