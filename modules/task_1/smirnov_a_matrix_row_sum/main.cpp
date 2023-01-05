// Copyright 2018 Nesterov Alexander
#include <gtest/gtest.h>
#include <vector>
#include "./matrix_row_sum.h"
#include <gtest-mpi-listener.hpp>

TEST(Parallel_Operations_MPI, Test_Seq_Sum) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> global_matrix;
  std::vector<int> result;
  const int count_rows_matrix = 3;
  const int count_columns_matrix = 3;

  if (rank == 0) {
    global_matrix = { 3, 5 , 7, 10, 12, 15, 5, 5, 5 };
  }

  if (rank == 0) {
    result = calculateSequentialMatrixRowSum(global_matrix,
      count_rows_matrix, count_columns_matrix);
    std::vector<int> reference_result = { 15, 37, 15 };
    ASSERT_EQ(reference_result, result);
  }
}

TEST(Parallel_Operations_MPI, Test_Sum_When_One_Row) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_matrix;
    std::vector<int> result;
    const int count_rows_matrix = 1;
    const int count_columns_matrix = 10;

    if (rank == 0) {
        global_matrix = getRandomMatrix(count_rows_matrix, count_columns_matrix);
    }

    result = calculateParallelMatrixRowSum(global_matrix, count_rows_matrix, count_columns_matrix);

    if (rank == 0) {
        std::vector<int> reference_result = calculateSequentialMatrixRowSum(global_matrix,
          count_rows_matrix, count_columns_matrix);
        ASSERT_EQ(reference_result, result);
    }
}

TEST(Parallel_Operations_MPI, Test_Sum_When_Even_Rows) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> global_matrix;
  std::vector<int> result;
  const int count_rows_matrix = 12;
  const int count_columns_matrix = 10;

  if (rank == 0) {
    global_matrix = getRandomMatrix(count_rows_matrix, count_columns_matrix);
  }

  result = calculateParallelMatrixRowSum(global_matrix, count_rows_matrix, count_columns_matrix);

  if (rank == 0) {
    std::vector<int> reference_result = calculateSequentialMatrixRowSum(global_matrix,
      count_rows_matrix, count_columns_matrix);
    ASSERT_EQ(reference_result, result);
  }
}

TEST(Parallel_Operations_MPI, Test_Sum_When_Odd_Rows) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> global_matrix;
  std::vector<int> result;
  const int count_rows_matrix = 11;
  const int count_columns_matrix = 10;

  if (rank == 0) {
    global_matrix = getRandomMatrix(count_rows_matrix, count_columns_matrix);
  }

  result = calculateParallelMatrixRowSum(global_matrix, count_rows_matrix, count_columns_matrix);

  if (rank == 0) {
    std::vector<int> reference_result = calculateSequentialMatrixRowSum(global_matrix,
      count_rows_matrix, count_columns_matrix);
    ASSERT_EQ(reference_result, result);
  }
}

TEST(Parallel_Operations_MPI, Test_Sum_When_Many_Rows) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> global_matrix;
  std::vector<int> result;
  const int count_rows_matrix = 170;
  const int count_columns_matrix = 10;

  if (rank == 0) {
    global_matrix = getRandomMatrix(count_rows_matrix, count_columns_matrix);
  }

  result = calculateParallelMatrixRowSum(global_matrix, count_rows_matrix, count_columns_matrix);

  if (rank == 0) {
    std::vector<int> reference_result = calculateSequentialMatrixRowSum(global_matrix,
      count_rows_matrix, count_columns_matrix);
    ASSERT_EQ(reference_result, result);
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
