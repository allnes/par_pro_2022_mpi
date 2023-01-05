// Copyright 2022 Pronina Tatiana

#include <gtest/gtest.h>
#include <vector>
#include "./pronina_t_matrix_multiplication.h"
#include <gtest-mpi-listener.hpp>


TEST(CCS_Matrix_mult, Sequential_multiplication) {
  int ProcRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

  if (ProcRank == 0) {
    SparseMatrix A, B;
    A = CCS(std::vector<double>{0, 0, 1, 0, 2, 0, 0, 0, 3, 0, 0, 0}, 3, 4);
    B = CCS(std::vector<double>{3, 0, 0, 0, 4, 0}, 2, 3);

    std::vector<double> result = A * B;
    std::vector<double> exp_result{ 4, 0, 0, 0, 12, 0, 0, 0 };

    ASSERT_EQ(result, exp_result);
  }
}

TEST(CCS_Matrix_mult, Parallel_multiplication) {
  int ProcRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

  SparseMatrix A, B;
  if (ProcRank == 0) {
    A = CCS(std::vector<double>{0, 0, 1, 0, 2, 0, 0, 0, 3, 0, 0, 0}, 3, 4);
    B = CCS(std::vector<double>{3, 0, 0, 0, 4, 0}, 2, 3);
  }

  std::vector<double> result = Multiply(A, B);

  if (ProcRank == 0) {
    std::vector<double> exp_result{ 4, 0, 0, 0, 12, 0, 0, 0 };

    ASSERT_EQ(result, exp_result);
  }
}

TEST(CCS_Matrix_mult, Random_20x20) {
  int ProcRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

  SparseMatrix A, B;
  if (ProcRank == 0) {
    A = CCS(RandMatrix(20, 20), 20, 20);
    B = CCS(RandMatrix(20, 20), 20, 20);
  }

  std::vector<double> result = Multiply(A, B);

  if (ProcRank == 0) {
    std::vector<double> exp_result = A * B;

    ASSERT_EQ(result, exp_result);
  }
}

TEST(CCS_Matrix_mult, Random_50x50) {
  int ProcRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

  SparseMatrix A, B;
  if (ProcRank == 0) {
    A = CCS(RandMatrix(50, 50), 50, 50);
    B = CCS(RandMatrix(50, 50), 50, 50);
  }

  std::vector<double> result = Multiply(A, B);

  if (ProcRank == 0) {
    std::vector<double> exp_result = A * B;

    ASSERT_EQ(result, exp_result);
  }
}

TEST(CCS_Matrix_mult, Random_75x75) {
  int ProcRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

  SparseMatrix A, B;
  if (ProcRank == 0) {
    A = CCS(RandMatrix(75, 75), 75, 75);
    B = CCS(RandMatrix(75, 75), 75, 75);
  }
  std::vector<double> result = Multiply(A, B);

  if (ProcRank == 0) {
    std::vector<double> exp_result = A * B;
    ASSERT_EQ(result, exp_result);
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
