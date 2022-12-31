// Copyright 2022 Nikitin Alexander
#include <gtest/gtest.h>
#include <vector>
#include "./nikitin_a_linear_block_gauss.h"
#include <gtest-mpi-listener.hpp>

using std::vector;
TEST(GAUSS_TEST, TEST1) { ASSERT_NO_THROW(randomMatrix(144, 144)); }
TEST(GAUSS_TEST, TEST2) { ASSERT_NO_THROW(randomMatrix(144, 165)); }
TEST(GAUSS_TEST, TEST3) {
  vector<double> matrix = randomMatrix(100, 100);

  ASSERT_NO_THROW(operationSequential(matrix, 100, 100));
}
TEST(GAUSS_TEST, TEST4) {
  vector<double> matrix = randomMatrix(100, 144);

  ASSERT_NO_THROW(operationSequential(matrix, 100, 144));
}
TEST(GAUSS_TEST, TEST5) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int row_count = 203, column_count = 203;
  vector<double> matrix(row_count * column_count);

  if (rank == 0) {
    matrix = randomMatrix(row_count, column_count);
  }
  ASSERT_NO_THROW(operationParallel(matrix, 203));
}

TEST(GAUSS_TEST, TEST6) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int matrix_size = 183;
  vector<double> matrix(matrix_size * matrix_size);

  if (rank == 0) {
    matrix = randomMatrix(matrix_size, matrix_size);
  }

  gaussCore();
  vector<double> result = operationParallel(matrix, matrix_size);

  if (rank == 0) {
    vector<double> expected_res =
        operationSequential(matrix, matrix_size, matrix_size);

    ASSERT_EQ(result, expected_res);
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
