// Copyright 2022 Yurin Evgeny
#include "../../../modules/task_3/yurin_e_rectangle/main.h"
#include <gtest/gtest.h>
#include <functional>
#include <gtest-mpi-listener.hpp>

TEST(RectangleIntegration, Test1) {
  int ProcRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  int n = 1000;
  double xmin = 0, xmax = 2;
  double ymin = 0, ymax = 2;
  auto f = [](double x, double y) { return x * y; };
  double result = integrateRectangleParallel(xmin, xmax, ymin, ymax, n, n, f);
  double expectedResult = integrateRectangle(xmin, xmax, ymin, ymax, n, n, f);
  if (ProcRank == 0) {
    ASSERT_NEAR(result, expectedResult, 0.01);
  }
}

TEST(RectangleIntegration, Test2) {
  int ProcRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  int n = 1000;
  double xmin = 1, xmax = 3;
  double ymin = 2, ymax = 5;
  auto f = [](double x, double y) { return 3 * x * x - y; };
  double result = integrateRectangleParallel(xmin, xmax, ymin, ymax, n, n, f);
  double expectedResult = integrateRectangle(xmin, xmax, ymin, ymax, n, n, f);
  if (ProcRank == 0) {
    ASSERT_NEAR(result, expectedResult, 0.01);
  }
}

TEST(RectangleIntegration, Test3) {
  int ProcRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  int n = 1000;
  double xmin = 2, xmax = 4;
  double ymin = 3, ymax = 5;
  auto f = [](double x, double y) { return x * y * y * y - 123; };
  double result = integrateRectangleParallel(xmin, xmax, ymin, ymax, n, n, f);
  double expectedResult = integrateRectangle(xmin, xmax, ymin, ymax, n, n, f);
  if (ProcRank == 0) {
    ASSERT_NEAR(result, expectedResult, 0.01);
  }
}

TEST(RectangleIntegration, Test4) {
  int ProcRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  int n = 1000;
  double xmin = 1, xmax = 6;
  double ymin = 0, ymax = 2;
  auto f = [](double x, double y) { return y - x; };
  double result = integrateRectangleParallel(xmin, xmax, ymin, ymax, n, n, f);
  double expectedResult = integrateRectangle(xmin, xmax, ymin, ymax, n, n, f);
  if (ProcRank == 0) {
    ASSERT_NEAR(result, expectedResult, 0.01);
  }
}

TEST(RectangleIntegration, Test5) {
  int ProcRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  int n = 1000;
  double xmin = 1, xmax = 4;
  double ymin = 2, ymax = 3;
  auto f = [](double x, double y) { return y + x + x * x; };
  double result = integrateRectangleParallel(xmin, xmax, ymin, ymax, n, n, f);
  double expectedResult = integrateRectangle(xmin, xmax, ymin, ymax, n, n, f);
  if (ProcRank == 0) {
    ASSERT_NEAR(result, expectedResult, 0.01);
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
