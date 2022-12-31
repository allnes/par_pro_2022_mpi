// Copyright 2022 Kolesov Maxim
#include <gtest/gtest.h>
#include <vector>
#include "./matrix_column_min.h"
#include <gtest-mpi-listener.hpp>

TEST(matrix_column_min, transpose) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    std::vector<int> matrix = {1, 2, 3, 4, 5, 6, 7, 8};
    std::vector<int> check = {1, 5, 2, 6, 3, 7, 4, 8};
    std::vector<int> t = transposeMatrix(matrix, 2, 4);

    ASSERT_EQ(t, check);
  }
}

TEST(matrix_column_min, test1) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<int> matrix;
  int n = 2;
  int m = 4;
  if (rank == 0) {
    matrix = generateMatrix(n, m);
  }

  std::vector<int> paralelRes = getColumnMinParalel(matrix, n, m);

  if (rank == 0) {
    std::vector<int> check(m);
    std::vector<int> t = transposeMatrix(matrix, n, m);


    for (int i = 0; i < m; i++) {
      check[i] = getMinInSequence(std::vector<int>(t.begin() + i*n, t.begin() + i*n + n));
    }

    ASSERT_EQ(check, paralelRes);
  }
}

TEST(matrix_column_min, test2) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<int> matrix;
  int n = 6;
  int m = 6;
  if (rank == 0) {
    matrix = generateMatrix(n, m);
  }

  std::vector<int> paralelRes = getColumnMinParalel(matrix, n, m);

  if (rank == 0) {
    std::vector<int> check(m);
    std::vector<int> t = transposeMatrix(matrix, n, m);


    for (int i = 0; i < m; i++) {
      check[i] = getMinInSequence(std::vector<int>(t.begin() + i*n, t.begin() + i*n + n));
    }

    ASSERT_EQ(check, paralelRes);
  }
}

TEST(matrix_column_min, test3) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<int> matrix;
  int n = 6;
  int m = 12;
  if (rank == 0) {
    matrix = generateMatrix(n, m);
  }

  std::vector<int> paralelRes = getColumnMinParalel(matrix, n, m);

  if (rank == 0) {
    std::vector<int> check(m);
    std::vector<int> t = transposeMatrix(matrix, n, m);


    for (int i = 0; i < m; i++) {
      check[i] = getMinInSequence(std::vector<int>(t.begin() + i*n, t.begin() + i*n + n));
    }

    ASSERT_EQ(check, paralelRes);
  }
}

TEST(matrix_column_min, test4) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<int> matrix;
  int n = 2;
  int m = 2;
  if (rank == 0) {
    matrix = generateMatrix(n, m);
  }

  std::vector<int> paralelRes = getColumnMinParalel(matrix, n, m);

  if (rank == 0) {
    std::vector<int> check(m);
    std::vector<int> t = transposeMatrix(matrix, n, m);


    for (int i = 0; i < m; i++) {
      check[i] = getMinInSequence(std::vector<int>(t.begin() + i*n, t.begin() + i*n + n));
    }

    ASSERT_EQ(check, paralelRes);
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
