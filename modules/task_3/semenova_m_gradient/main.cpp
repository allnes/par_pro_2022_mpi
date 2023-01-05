// Copyright 2022 Semenova Veronika
#include <gtest/gtest.h>
#include <vector>
#include "./m_gradient.h"
#include <gtest-mpi-listener.hpp>

TEST(Parallel_Operations_MPI, Serial_method_gradient_is_correct_1) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, & rank);
  int n = 2;
  Vector V(n);
  Vector M(n * n);
  Vector res2(n);
  V[0] = 17;
  V[1] = 16;
  M[0] = 1;
  M[1] = 2;
  M[2] = 2;
  M[3] = 1;
  res2[0] = 5;
  res2[1] = 6;
  if (rank == 0) {
    Vector res1 = Serial_method_gradient(M, V, n);
    for (int i = 0; i < res1.size(); i++)
      EXPECT_LE(abs(res2[i] - res1[i]), 1);
  }
}
TEST(Parallel_Operations_MPI, Serial_method_gradient_is_correct_2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int n = 2;
    Vector V(n);
    Vector M(n * n);
    Vector res2(n);
    V[0] = 3;
    V[1] = 7;
    M[0] = 3;
    M[1] = -1;
    M[2] = -1;
    M[3] = 3;
    res2[0] = 2;
    res2[1] = 3;
    if (rank == 0) {
        Vector res1 = Serial_method_gradient(M, V, n);
        for (int i = 0; i < res1.size(); i++)
            EXPECT_LE(abs(res2[i] - res1[i]), 1);
    }
}
TEST(Parallel_Operations_MPI, Serial_method_gradient_is_correct_3) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, & rank);
  int n = 3;
  Vector V(n);
  Vector M(n * n);
  Vector res2(n);
  V[0] = 6;
  V[1] = 6;
  V[2] = 10;
  M[0] = 1;
  M[1] = 2;
  M[2] = 3;
  M[3] = 2;
  M[4] = 1;
  M[5] = 0;
  M[6] = 3;
  M[7] = 0;
  M[8] = 1;
  res2[0] = 3;
  res2[1] = 0;
  res2[2] = 1;
  if (rank == 0) {
    Vector res1 = Serial_method_gradient(M, V, n);
    for (int i = 0; i < res1.size(); i++)
      EXPECT_LE(abs(res2[i] - res1[i]), 1);
  }
}
TEST(Parallel_Operations_MPI, Paralle_method_gradient_is_correct_1) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, & rank);
  int n = 2;
  Vector V(n);
  Vector M(n * n);
  Vector res2(n);
  V[0] = 17;
  V[1] = 16;
  M[0] = 1;
  M[1] = 2;
  M[2] = 2;
  M[3] = 1;
  res2[0] = 5;
  res2[1] = 6;
  Vector res1 = Paralle_method_gradient(M, V, n);
  if (rank == 0) {
    for (int i = 0; i < res1.size(); i++)
        EXPECT_LE(abs(res2[i] - res1[i]), 1);
  }
}
TEST(Parallel_Operations_MPI, Paralle_method_gradient_is_correct_2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int n = 2;
    Vector V(n);
    Vector M(n * n);
    Vector res2(n);
    V[0] = 3;
    V[1] = 7;
    M[0] = 3;
    M[1] = -1;
    M[2] = -1;
    M[3] = 3;
    res2[0] = 2;
    res2[1] = 3;
    Vector res1 = Paralle_method_gradient(M, V, n);
    if (rank == 0) {
        for (int i = 0; i < res1.size(); i++)
            EXPECT_LE(abs(res2[i] - res1[i]), 1);
    }
}
TEST(Parallel_Operations_MPI, Paralle_method_gradient_is_correct_3) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, & rank);
  int n = 3;
  Vector V(n);
  Vector M(n * n);
  Vector res2(n);
  V[0] = 6;
  V[1] = 6;
  V[2] = 10;
  M[0] = 1;
  M[1] = 2;
  M[2] = 3;
  M[3] = 2;
  M[4] = 1;
  M[5] = 0;
  M[6] = 3;
  M[7] = 0;
  M[8] = 1;
  res2[0] = 3;
  res2[1] = 0;
  res2[2] = 1;
  Vector res1 = Paralle_method_gradient(M, V, n);
  if (rank == 0) {
    for (int i = 0; i < res1.size(); i++)
        EXPECT_LE(abs(res2[i] - res1[i]), 1);
  }
}
TEST(Parallel_Operations_MPI, Serial_and_paralle_method1) {
    int rank;
    int n = 2;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    Vector V(n);
    Vector M(n * n);
    V[0] = 3;
    V[1] = 7;
    M[0] = 3;
    M[1] = -1;
    M[2] = -1;
    M[3] = 3;
    Vector res2 = Paralle_method_gradient(M, V, n);
    if (rank == 0) {
        Vector res1 = Serial_method_gradient(M, V, n);
        for (int i = 0; i < res1.size(); i++) {
            double E = std::max(1.0, res2[i] / 0.00001);
            EXPECT_LE(abs(res2[i] - res1[i]), E);
        }
    }
}
TEST(Parallel_Operations_MPI, Serial_and_paralle_method2) {
    int rank;
    int n = 3;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    Vector V(n);
    Vector M(n * n);
    Vector res2(n);
    V[0] = 6;
    V[1] = 6;
    V[2] = 10;
    M[0] = 1;
    M[1] = 2;
    M[2] = 3;
    M[3] = 2;
    M[4] = 1;
    M[5] = 0;
    M[6] = 3;
    M[7] = 0;
    M[8] = 1;

    Vector res1 = Paralle_method_gradient(M, V, n);
    if (rank == 0) {
        Vector res2 = Serial_method_gradient(M, V, n);
        for (int i = 0; i < res1.size(); i++) {
            double E = std::max(1.0, res2[i] / 0.00001);
            EXPECT_LE(abs(res2[i] - res1[i]), E);
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
