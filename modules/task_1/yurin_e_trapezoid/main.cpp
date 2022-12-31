// Copyright 2022 Yurin Evgeny
#include <gtest/gtest.h>

#include <cmath>
#include <functional>
#include <gtest-mpi-listener.hpp>

#include "./trapezoid.h"

TEST(Parallel_MPI, firstTestSimpleFunction) {
  int ProcRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  int a = 0;
  int b = 100;
  int n = 10000;
  auto simpleFunction = [](double x) { return x; };

  double parallelResult = trapezoidalIntegralParallel(a, b, n, simpleFunction);

  if (ProcRank == 0) {
    double synchronizedResult = trapezoidalIntegral(a, b, n, simpleFunction);
    ASSERT_NEAR(synchronizedResult, parallelResult, 1e-7);
  }
}
TEST(Parallel_MPI, secondTestSquare) {
  int ProcRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

  int a = 0;
  int b = 100;
  int n = 10000;
  auto squareFunction = [](double x) { return x * x; };

  double parallelResult = trapezoidalIntegralParallel(a, b, n, squareFunction);

  if (ProcRank == 0) {
    double synchronizedResult = trapezoidalIntegral(a, b, n, squareFunction);
    ASSERT_NEAR(synchronizedResult, parallelResult, 1e-7);
  }
}
TEST(Parallel_MPI, thirdTestSin) {
  int ProcRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

  int a = 0;
  int b = 100;
  int n = 10000;
  auto sinFunction = [](double x) { return sin(x); };

  double parallelResult = trapezoidalIntegralParallel(a, b, n, sinFunction);

  if (ProcRank == 0) {
    double synchronizedResult = trapezoidalIntegral(a, b, n, sinFunction);
    ASSERT_NEAR(synchronizedResult, parallelResult, 1e-7);
  }
}
TEST(Parallel_MPI, fourthTestSomeFunction) {
  int ProcRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

  int a = 0;
  int b = 100;
  int n = 10000;
  auto someFunction = [](double x) {
    return x * x * x - x * 5 + x * x * 6 - 2501;
  };

  double parallelResult = trapezoidalIntegralParallel(a, b, n, someFunction);

  if (ProcRank == 0) {
    double synchronizedResult = trapezoidalIntegral(a, b, n, someFunction);
    ASSERT_NEAR(synchronizedResult, parallelResult, 1e-7);
  }
}
TEST(Parallel_MPI, fifthFunctionLog) {
  int ProcRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

  int a = 1;
  int b = 100;
  int n = 10000;
  auto logFunction = [](double x) { return log(x); };

  double parallelResult = trapezoidalIntegralParallel(a, b, n, logFunction);

  if (ProcRank == 0) {
    double synchronizedResult = trapezoidalIntegral(a, b, n, logFunction);
    ASSERT_NEAR(synchronizedResult, parallelResult, 1e-7);
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
