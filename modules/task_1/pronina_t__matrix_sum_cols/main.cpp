// Copyright 2022 Pronina Tatiana
#include <gtest/gtest.h>

#include <gtest-mpi-listener.hpp>

#include "./matrix_sum_cols.h"

TEST(MPI_SUMM, MATRIX_NEGATIVE_ROWS_ERROR) {
  int Rank, rows = -1, cols = 5;
  MPI_Comm_rank(MPI_COMM_WORLD, &Rank);

  if (Rank == 0) {
    ASSERT_ANY_THROW(CreateRandMatrix(rows, cols));
  }
}

TEST(MPI_SUMM, MATRIX_ZERO_COLS_ERROR) {
  int Rank, rows = 5, cols = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &Rank);

  if (Rank == 0) {
    ASSERT_ANY_THROW(CreateRandMatrix(rows, cols));
  }
}

TEST(MPI_SUMM, MATRIX_ROWS_LESS_COLS) {
  int Rank, rows = 10, cols = 20;
  int* Matrix = nullptr;
  MPI_Comm_rank(MPI_COMM_WORLD, &Rank);

  if (Rank == 0) {
    Matrix = CreateRandMatrix(rows, cols);
  }

  int* sum = MPIMethod(Matrix, rows, cols);

  if (Rank == 0) {
    int* res = LinearMetod(Matrix, rows, cols);

    for (int i = 0; i < cols; i++) {
      ASSERT_EQ(res[i], sum[i]);
    }
  }
}

TEST(MPI_SUMM, SQUARE_MATRIX) {
  int Rank, rows = 5, cols = 5;
  int* Matrix = nullptr;
  MPI_Comm_rank(MPI_COMM_WORLD, &Rank);

  if (Rank == 0) {
    Matrix = CreateRandMatrix(rows, cols);
  }

  int* sum = MPIMethod(Matrix, rows, cols);

  if (Rank == 0) {
    int* res = LinearMetod(Matrix, rows, cols);

    for (int i = 0; i < cols; i++) {
      ASSERT_EQ(res[i], sum[i]);
    }
  }
}

TEST(MPI_SUMM, MATRIX_ROWS_MORE_COLS) {
  int Rank, rows = 20, cols = 10;
  MPI_Comm_rank(MPI_COMM_WORLD, &Rank);
  int* Matrix = nullptr;

  if (Rank == 0) {
    Matrix = CreateRandMatrix(rows, cols);
  }

  int* sum = MPIMethod(Matrix, rows, cols);

  if (Rank == 0) {
    int* res = LinearMetod(Matrix, rows, cols);
    for (int i = 0; i < cols; i++) {
      ASSERT_EQ(res[i], sum[i]);
    }
  }
}

TEST(MPI_SUMM, MATRIX_BIG_SQUARE) {
  int Rank, rows = 250, cols = 250;
  MPI_Comm_rank(MPI_COMM_WORLD, &Rank);
  int* Matrix = nullptr;

  if (Rank == 0) {
    Matrix = CreateRandMatrix(rows, cols);
  }

  int* sum = MPIMethod(Matrix, rows, cols);

  if (Rank == 0) {
    int* res = LinearMetod(Matrix, rows, cols);

    for (int i = 0; i < cols; i++) {
      ASSERT_EQ(res[i], sum[i]);
    }
  }
}

int main(int argc, char** argv) {
  MPI_Init(&argc, &argv);

  ::testing::InitGoogleTest(&argc, argv);
  ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
  ::testing::TestEventListeners& listeners =
      ::testing::UnitTest::GetInstance()->listeners();

  listeners.Release(listeners.default_result_printer());
  listeners.Release(listeners.default_xml_generator());
  listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);

  return RUN_ALL_TESTS();
}
