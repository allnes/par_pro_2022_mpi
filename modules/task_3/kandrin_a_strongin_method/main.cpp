// Copyright 2022 Kandrin Alexey
#include <gtest/gtest.h>
#define _USE_MATH_DEFINES
#include <math.h>

#include <gtest-mpi-listener.hpp>

#include "./strongin_method.h"

namespace {
double function1(double x) {
  return sqrt(1 + 3 * cos(x) * cos(x)) + cos(10 * x);
}

double function2(double x) { return cos(x); }

double function3(double x) { return sin(x); }

double function4(double x) { return sin(x) * cos(x) * cos(x); }

double function5(double x) { return sqrt(x) * sin(x); }
}  // namespace

static constexpr double epsilon = 0.01;

TEST(Parallel_Operations_MPI, Test_1) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  auto min = GetMinParallel(function1, 1, 50, epsilon);

  if (rank == 0) {
    ASSERT_NEAR(min, 0, epsilon);
  }
}

TEST(Parallel_Operations_MPI, Test_2) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  auto min = GetMinParallel(function2, 0, 2 * M_PI, epsilon);

  if (rank == 0) {
    ASSERT_NEAR(min, -1, epsilon);
  }
}

TEST(Parallel_Operations_MPI, Test_3) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  auto min = GetMinParallel(function3, 0, 2 * M_PI, epsilon);

  if (rank == 0) {
    ASSERT_NEAR(min, -1, epsilon);
  }
}

TEST(Parallel_Operations_MPI, Test_4) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  auto min = GetMinParallel(function4, -M_PI, M_PI, epsilon);

  if (rank == 0) {
    ASSERT_NEAR(min, -0.3849, epsilon);
  }
}

TEST(Parallel_Operations_MPI, Test_5) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  auto min = GetMinParallel(function5, 1, 13, epsilon);

  if (rank == 0) {
    ASSERT_NEAR(min, -3.319, epsilon);
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
