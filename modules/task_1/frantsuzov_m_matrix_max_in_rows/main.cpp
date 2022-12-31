// Copyright 2022 Frantsuzov Maksim
#include <mpi.h>
#include <gtest/gtest.h>
#include <gtest-mpi-listener.hpp>
#include "./matrix_max_in_rows.h"
// #include "matrix_max_in_rows.cpp"
// using namespace std;

TEST(Parallel_Operations_MPI, TEST1) {
  int rowSize = 10, columnSize = 10;
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> global_vec;
  int* arrayWithMPI = findMaxElementsInRowsMPI(rowSize, columnSize);
  if (rank == 0) {
    int* array = findMaxElementsInRows(rowSize, columnSize);
    for (int i = 0; i < rowSize; i++) {
      ASSERT_EQ(array[i], arrayWithMPI[i]);
    }
  }
}

TEST(Parallel_Operations_MPI, TEST2) {
  int rowSize = 1, columnSize = 10;
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> global_vec;

  int* arrayWithMPI = findMaxElementsInRowsMPI(rowSize, columnSize);
  if (rank == 0) {
    int* array = findMaxElementsInRows(rowSize, columnSize);
    for (int i = 0; i < rowSize; i++) {
      ASSERT_EQ(array[i], arrayWithMPI[i]);
    }
  }
}

TEST(Parallel_Operations_MPI, TEST3) {
  int rowSize = 10, columnSize = 1;
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> global_vec;

  int* arrayWithMPI = findMaxElementsInRowsMPI(rowSize, columnSize);
  if (rank == 0) {
    int* array = findMaxElementsInRows(rowSize, columnSize);
    for (int i = 0; i < rowSize; i++) {
      ASSERT_EQ(array[i], arrayWithMPI[i]);
    }
  }
}

TEST(Parallel_Operations_MPI, TEST4) {
  int rowSize = 5, columnSize = 6;
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> global_vec;

  int* arrayWithMPI = findMaxElementsInRowsMPI(rowSize, columnSize);
  if (rank == 0) {
    int* array = findMaxElementsInRows(rowSize, columnSize);
    for (int i = 0; i < rowSize; i++) {
      ASSERT_EQ(array[i], arrayWithMPI[i]);
    }
  }
}

TEST(Parallel_Operations_MPI, TEST5) {
  int rowSize = 0, columnSize = 0;
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> global_vec;

  int* arrayWithMPI = findMaxElementsInRowsMPI(rowSize, columnSize);
  if (rank == 0) {
    int* array = findMaxElementsInRows(rowSize, columnSize);
    for (int i = 0; i < rowSize; i++) {
      ASSERT_EQ(array[i], arrayWithMPI[i]);
    }
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
