// Copyright 2022 Makarov Mikhail
#include <gtest/gtest.h>
#include <vector>
#include "./sparse_matrix_col.h"
#include <gtest-mpi-listener.hpp>

TEST(Produce_Matrix, can_create_random_matrix) {
  ASSERT_NO_THROW(GenerateMatrix(10, 10, 20.0));
}

TEST(Produce_Matrix, can_compare_matrix1) {
  int countRow1 = 80;
  int countCol1 = 20;
  int countRow2 = 20;
  int countCol2 = 90;

  double perc = 40;
  ComprColStorage FirstMatr = GenerateMatrix(countRow1, countCol1, perc);
  ComprColStorage SecondMatr = GenerateMatrix(countRow2, countCol2, perc);
  ASSERT_NE(FirstMatr, SecondMatr);
}

TEST(Produce_Matrix, can_compare_matrix2) {
  ComprColStorage FirstMatr = GenerateMatrix(10, 10, 20.0);
  ASSERT_EQ(FirstMatr, FirstMatr);
}

TEST(Produce_Matrix, test_produce80x20_20x90) {
  int countRow1 = 80;
  int countCol1 = 20;
  int countRow2 = 20;
  int countCol2 = 90;
  double perc = 50;

  int ProcRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  ComprColStorage FirstMatr = GenerateMatrix(countRow1, countCol1, perc);
  ComprColStorage SecondMatr = GenerateMatrix(countRow2, countCol2, perc);

  double start1 = MPI_Wtime();
  ComprColStorage resMatr = GetSequentialSolution(&FirstMatr, &SecondMatr);
  double end1 = MPI_Wtime();
  double start2 = MPI_Wtime();
  ComprColStorage resMatr2 = GetParalSolution(&FirstMatr, &SecondMatr);
  double end2 = MPI_Wtime();
}

TEST(Produce_Matrix, test_produce400x160_160x300) {
  int countRow1 = 400;
  int countCol1 = 160;
  int countRow2 = 260;
  int countCol2 = 300;
  double perc = 17.0;

  int ProcRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  ComprColStorage FirstMatr = GenerateMatrix(countRow1, countCol1, perc);
  ComprColStorage SecondMatr = GenerateMatrix(countRow2, countCol2, perc);

  double start1 = MPI_Wtime();
  ComprColStorage resMatr = GetSequentialSolution(&FirstMatr, &SecondMatr);
  double end1 = MPI_Wtime();
  double start2 = MPI_Wtime();
  ComprColStorage resMatr2 = GetParalSolution(&FirstMatr, &SecondMatr);
  double end2 = MPI_Wtime();
}

/* TEST(Produce_Matrix, test_produce300x150_150x200) {
  int countRow1 = 300;
  int countCol1 = 150;
  int countRow2 = 150;
  int countCol2 = 200;
  double perc = 10.5;

  int ProcRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  ComprColStorage FirstMatr = GenerateMatrix(countRow1, countCol1, perc);
  ComprColStorage SecondMatr = GenerateMatrix(countRow2, countCol2, perc);

  double start1 = MPI_Wtime();
  ComprColStorage resMatr = GetSequentialSolution(&FirstMatr, &SecondMatr);
  double end1 = MPI_Wtime();
  double start2 = MPI_Wtime();
  ComprColStorage resMatr2 = GetParalSolution(&FirstMatr, &SecondMatr);
  double end2 = MPI_Wtime();
}*/

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
