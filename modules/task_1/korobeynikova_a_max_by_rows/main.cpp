// Copyright 2022 Korobeynikova Alisa

#include <gtest/gtest.h>

#include <vector>
#include <gtest-mpi-listener.hpp>

#include "./max_by_rows.h"

std::vector<int> seqSolution(const Matrix<int>& m) {
  int n = m.GetN();
  std::vector<int> seq_max_values(n);
  for (int i = 0; i < n; ++i) {
    seq_max_values.at(i) = findMax(std::vector<int>(m[i], m[i + 1]));
  }
  return seq_max_values;
}

TEST(Parallel_Operations_MPI, Test_Max) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  const int n_size_matrix = 1, m_size_matrix = 5;
  Matrix<int> global_matr(n_size_matrix, m_size_matrix);

  std::vector<int> ps = getParallelOperation(global_matr);

  if (rank == 0) {
    std::vector<int> ss = seqSolution(global_matr);
    ASSERT_EQ(ps, ss);
  }
}

TEST(Parallel_Operations_MPI, Test_Max_2) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  const int n_size_matrix = 3, m_size_matrix = 5;
  Matrix<int> global_matr(n_size_matrix, m_size_matrix);

  std::vector<int> ps = getParallelOperation(global_matr);

  if (rank == 0) {
    std::vector<int> ss = seqSolution(global_matr);
    ASSERT_EQ(ps, ss);
  }
}

TEST(Parallel_Operations_MPI, Test_Max_3) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  const int n_size_matrix = 10, m_size_matrix = 1;
  Matrix<int> global_matr(n_size_matrix, m_size_matrix);

  std::vector<int> ps = getParallelOperation(global_matr);

  if (rank == 0) {
    std::vector<int> ss = seqSolution(global_matr);
    ASSERT_EQ(ps, ss);
  }
}

TEST(Parallel_Operations_MPI, Test_Max_4) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  const int n_size_matrix = 47, m_size_matrix = 3;
  Matrix<int> global_matr(n_size_matrix, m_size_matrix);

  std::vector<int> ps = getParallelOperation(global_matr);

  if (rank == 0) {
    std::vector<int> ss = seqSolution(global_matr);
    ASSERT_EQ(ps, ss);
  }
}

TEST(Parallel_Operations_MPI, Test_Max_5) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  const int n_size_matrix = 100, m_size_matrix = 3;
  Matrix<int> global_matr(n_size_matrix, m_size_matrix);

  std::vector<int> ps = getParallelOperation(global_matr);

  if (rank == 0) {
    std::vector<int> ss = seqSolution(global_matr);
    ASSERT_EQ(ps, ss);
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
