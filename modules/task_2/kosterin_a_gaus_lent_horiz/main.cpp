// Copyright 2022 Kosterin Alexey
#include <gtest/gtest.h>
#include <math.h>
#include <vector>
#include "../../../modules/task_2/kosterin_a_gaus_lent_horiz/gaus_lent_horiz.h"
#include <gtest-mpi-listener.hpp>

TEST(Horiz_Scheme_MPI, Test_3x3) {
  int size = 12;
  bool check;
  std::vector<double> a(size);
  a = newMatrix(size);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<double> res(3);
  res = getGauss(a, 3);
  if (rank == 0) {
    check = ChekTrueAnswer(a, 3, res);
    EXPECT_EQ(check, true);
  }
}

TEST(Horiz_Scheme_MPI, Test_2x2) {
  int size = 6;
  bool check;
  std::vector<double> a(size);
  a = newMatrix(size);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<double> res(2);
  res = getParGauss(a, 2);
  if (rank == 0) {
    check = ChekTrueAnswer(a, 2, res);
    EXPECT_EQ(check, true);
  }
}

TEST(Horiz_Scheme_MPI, Test_4x4) {
  int size = 20;
  bool check;
  std::vector<double> a(size);
  a = newMatrix(size);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<double> res = getParGauss(a, 3);
  if (rank == 0) {
    check = ChekTrueAnswer(a, 3, res);
    EXPECT_EQ(check, true);
  }
}

TEST(Horiz_Scheme_MPI, Test_5x5) {
  int size = 30;
  bool check;
  std::vector<double> a(size);
  a = newMatrix(size);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<double> res(4);
  res = getParGauss(a, 4);
  if (rank == 0) {
    check = ChekTrueAnswer(a, 4, res);
    EXPECT_EQ(check, true);
  }
}

TEST(Horiz_Scheme_MPI, Test_6x6) {
  int size = 42;
  bool check;
  std::vector<double> a(size);
  a = newMatrix(size);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<double> res(5);
  res = getParGauss(a, 5);
  if (rank == 0) {
    check = ChekTrueAnswer(a, 5, res);
    EXPECT_EQ(check, true);
  }
}

TEST(Horiz_Scheme_MPI, Test_10x10) {
  int size = 110;
  bool check;
  std::vector<double> a(size);
  a = newMatrix(size);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<double> res(6);
  res = getParGauss(a, 6);
  if (rank == 0) {
    check = ChekTrueAnswer(a, 6, res);
    EXPECT_EQ(check, true);
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
