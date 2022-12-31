// Copyright 2022 Semenova Veronika
#include <gtest/gtest.h>

#include "./alter_sign.h"

#include <gtest-mpi-listener.hpp>

TEST(Parallel_Operations_MPI, correct_operation_of_ParallelSum12) {
  int rank = 0;
  const int n = 12;
  int * V = nullptr;

  MPI_Comm_rank(MPI_COMM_WORLD, & rank);
  if (rank == 0) {
    V = new int[n];
    std::fill_n(V, n, 1);
    V[2] = -1;
  }
  int resPar = ParallelSum(V, n);
  if (rank == 0) {
    delete[] V;
    ASSERT_EQ(2, resPar);
  }
}
TEST(Parallel_Operations_MPI, correct_operation_of_ParallelSum25) {
  int rank = 0;
  const int n = 25;
  int * V = nullptr;

  MPI_Comm_rank(MPI_COMM_WORLD, & rank);
  if (rank == 0) {
    V = new int[n];
    std::fill_n(V, n, 1);
    V[2] = -1;
  }
  int resPar = ParallelSum(V, n);
  if (rank == 0) {
    delete[] V;
    ASSERT_EQ(2, resPar);
  }
}
TEST(Parallel_Operations_MPI, correct_operation_of_ParallelSum6) {
  int rank = 0;
  const int n = 6;
  int * V = nullptr;

  MPI_Comm_rank(MPI_COMM_WORLD, & rank);
  if (rank == 0) {
    V = new int[n];
    std::fill_n(V, n, 1);
    V[0] = -1;
    V[n - 1] = -1;
  }
  int resPar = ParallelSum(V, n);
  if (rank == 0) {
    delete[] V;
    ASSERT_EQ(2, resPar);
  }
}
TEST(Parallel_Operations_MPI, correct_operation_of_ParallelSum_with_Random12) {
  int rank = 0;
  const int n = 12;
  int * V = nullptr;

  MPI_Comm_rank(MPI_COMM_WORLD, & rank);
  if (rank == 0) {
    V = new int[n];
    RandVec(V, n);
  }
  int sumPar = ParallelSum(V, n);
  if (rank == 0) {
    int sumSer = SerialSum(V, n);
    delete[] V;
    ASSERT_EQ(sumSer, sumPar);
  }
}
TEST(Parallel_Operations_MPI, correct_operation_of_ParallelSum_with_Random25) {
  int rank = 0;
  const int n = 25;
  int * V = nullptr;

  MPI_Comm_rank(MPI_COMM_WORLD, & rank);
  if (rank == 0) {
    V = new int[n];
    RandVec(V, n);
  }
  int sumPar = ParallelSum(V, n);
  if (rank == 0) {
    int sumSer = SerialSum(V, n);
    delete[] V;
    ASSERT_EQ(sumSer, sumPar);
  }
}
TEST(Parallel_Operations_MPI, correct_operation_of_ParallelSum_with_Random6) {
  int rank = 0;
  const int n = 6;
  int * V = nullptr;

  MPI_Comm_rank(MPI_COMM_WORLD, & rank);
  if (rank == 0) {
    V = new int[n];
    RandVec(V, n);
  }
  int sumPar = ParallelSum(V, n);
  if (rank == 0) {
    int sumSer = SerialSum(V, n);
    delete[] V;
    ASSERT_EQ(sumSer, sumPar);
  }
}
TEST(Parallel_Operations_MPI, correct_operation_of_ParallelSum_with_Random100000) {
  int rank = 0;
  const int n = 6;
  int * V = nullptr;

  MPI_Comm_rank(MPI_COMM_WORLD, & rank);
  if (rank == 0) {
    V = new int[n];
    RandVec(V, n);
  }
  int sumPar = ParallelSum(V, n);
  if (rank == 0) {
    int sumSer = SerialSum(V, n);
    delete[] V;
    ASSERT_EQ(sumSer, sumPar);
  }
}

int main(int argc, char ** argv) {
  ::testing::InitGoogleTest(& argc, argv);
  MPI_Init(& argc, & argv);

  ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
  ::testing::TestEventListeners& listeners =
      ::testing::UnitTest::GetInstance()->listeners();

  listeners.Release(listeners.default_result_printer());
  listeners.Release(listeners.default_xml_generator());

  listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
  return RUN_ALL_TESTS();
}
