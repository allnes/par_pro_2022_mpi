// Copyright 2022 Makarov Danila
#include <gtest/gtest.h>

#include <gtest-mpi-listener.hpp>

#include "./integration_montecarlo.h"

TEST(Integration_montecarlo, test1) {
  int ProcRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

  int a, b, h, N;
  h = 4;
  a = 0;
  b = 8;
  N = 200000;

  double MPI_result = MPIintegration(N, a, b, h, function1);
  if (ProcRank == 0) {
    double not_MPI_result = notMPIintegration(N, a, b, h, function1);
    ASSERT_NEAR(MPI_result, not_MPI_result, 1);
    // std::cout << " expected ~16" << std::endl;
  }
}

TEST(Integration_montecarlo, test2) {
  int ProcRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  int a, b, h, N;
  h = 4;
  a = 1;
  b = 4;
  N = 200000;

  double MPI_result = MPIintegration(N, a, b, h, function2);
  if (ProcRank == 0) {
    double not_MPI_result = notMPIintegration(N, a, b, h, function2);
    ASSERT_NEAR(MPI_result, not_MPI_result, 1);
    // std::cout << " expected ~4.2" << std::endl;
  }
}

TEST(Integration_montecarlo, test3) {
  int ProcRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  int a, b, h, N;
  h = 4;
  a = 0;
  b = 10;
  N = 200000;

  double MPI_result = MPIintegration(N, a, b, h, function3);
  if (ProcRank == 0) {
    double not_MPI_result = notMPIintegration(N, a, b, h, function3);
    ASSERT_NEAR(MPI_result, not_MPI_result, 1);
    // std::cout << " expected ~21.08" << std::endl;
  }
}

TEST(Integration_montecarlo, test4) {
  int ProcRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  int a, b, h, N;
  h = 4;
  a = -2;
  b = 11;
  N = 200000;

  double MPI_result = MPIintegration(N, a, b, h, function4);
  if (ProcRank == 0) {
    double not_MPI_result = notMPIintegration(N, a, b, h, function4);
    ASSERT_NEAR(MPI_result, not_MPI_result, 1);
    // std::cout << " expected ~12.579" << std::endl;
  }
}

TEST(Integration_montecarlo, test5) {
  int ProcRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  int a, b, h, N;
  h = 20;
  a = 26;
  b = 30;
  N = 200000;

  double MPI_result = MPIintegration(N, a, b, h, function1);
  if (ProcRank == 0) {
    double not_MPI_result = notMPIintegration(N, a, b, h, function1);
    ASSERT_NEAR(MPI_result, not_MPI_result, 1);
    // std::cout << " expected ~56" << std::endl;
  }
}

TEST(Integration_montecarlo_throw, test6) {
  int ProcRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  int a, b, h, N;
  h = 4;
  a = 20;
  b = 10;
  N = 40000;

  ASSERT_ANY_THROW(MPIintegration(N, a, b, h, function3));
}

TEST(Integration_montecarlo_throw, test7) {
  int ProcRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  int a, b, h, N;
  h = 4;
  a = 20;
  b = 10;
  N = 0;

  ASSERT_ANY_THROW(MPIintegration(N, a, b, h, function3));
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
