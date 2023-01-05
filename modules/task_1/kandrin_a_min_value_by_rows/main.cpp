// Copyright 2022 Kandrin Alexey
#include <gtest/gtest.h>
#include <vector>
#include "./min_value_by_rows.h"
#include <gtest-mpi-listener.hpp>

namespace {
struct Random {
  std::random_device dev;
  std::mt19937 gen{dev()};

  int operator()() { return gen() % 100; }
} random_0_to_99;
}  // namespace

TEST(Parallel_Operations_MPI, Test_Sum_1) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  Matrix<int> matrix;
  const size_t rowCount = 1;
  const size_t colCount = 1;

  if (rank == 0) {
    matrix = GetRandomMatrix<int>(rowCount, colCount, random_0_to_99);
  }

  auto minValuesByRows = GetMinValuesByRowsParallel(matrix);

  if (rank == 0) {
    auto referenceMinValuesByRows = GetMinValuesByRowsSequential(matrix);
    ASSERT_EQ(referenceMinValuesByRows, minValuesByRows);
  }
}

TEST(Parallel_Operations_MPI, Test_Sum_2) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  Matrix<int> matrix;
  const size_t rowCount = 10;
  const size_t colCount = 10;

  if (rank == 0) {
    matrix = GetRandomMatrix<int>(rowCount, colCount, random_0_to_99);
  }

  auto minValuesByRows = GetMinValuesByRowsParallel(matrix);

  if (rank == 0) {
    auto referenceMinValuesByRows = GetMinValuesByRowsSequential(matrix);

    ASSERT_EQ(referenceMinValuesByRows, minValuesByRows);
  }
}

TEST(Parallel_Operations_MPI, Test_Sum_3) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  Matrix<int> matrix;
  const size_t rowCount = 10;
  const size_t colCount = 20;

  if (rank == 0) {
    matrix = GetRandomMatrix<int>(rowCount, colCount, random_0_to_99);
  }

  auto minValuesByRows = GetMinValuesByRowsParallel(matrix);

  if (rank == 0) {
    auto referenceMinValuesByRows = GetMinValuesByRowsSequential(matrix);

    ASSERT_EQ(referenceMinValuesByRows, minValuesByRows);
  }
}

TEST(Parallel_Operations_MPI, Test_Sum_4) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  Matrix<int> matrix;
  const size_t rowCount = 20;
  const size_t colCount = 10;

  if (rank == 0) {
    matrix = GetRandomMatrix<int>(rowCount, colCount, random_0_to_99);
  }

  auto minValuesByRows = GetMinValuesByRowsParallel(matrix);

  if (rank == 0) {
    auto referenceMinValuesByRows = GetMinValuesByRowsSequential(matrix);

    ASSERT_EQ(referenceMinValuesByRows, minValuesByRows);
  }
}

TEST(Parallel_Operations_MPI, Test_Sum_5) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  Matrix<int> matrix;
  const size_t rowCount = 13;
  const size_t colCount = 17;

  if (rank == 0) {
    matrix = GetRandomMatrix<int>(rowCount, colCount, random_0_to_99);
  }

  auto minValuesByRows = GetMinValuesByRowsParallel(matrix);

  if (rank == 0) {
    auto referenceMinValuesByRows = GetMinValuesByRowsSequential(matrix);

    ASSERT_EQ(referenceMinValuesByRows, minValuesByRows);
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
