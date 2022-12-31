// Copyright 2022 Antonova Anastsya
#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include <gtest-mpi-listener.hpp>
#include "../../../modules/task_2/antonova_n_smoothing_image/smoothing_image.h"

TEST(SMOOTHING_IMAGE, test1_on_wrong_image) {
  EXPECT_ANY_THROW(std::vector <int> matr = getImg(0, -1));
}

TEST(SMOOTHING_IMAGE_MPI, test2_simple_test) {
  std::vector <int> matr;
  int smoothing = 20;
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    matr = getImg(5, 7);
    ASSERT_ANY_THROW(ParallelSmoothing(matr, 3, 2, smoothing));
  }
}

TEST(SMOOTHING_IMAGE_MPI, test3_3x3) {
  int row = 3, col = 3, smoothing = 30;
  std::vector <int> matr(row * col), seq_fin(row * col);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  matr = { 100, 65, 79, 48, 54, 160, 73, 46, 91 };
  seq_fin = { 51, 97, 79, 119, 111, 0, 86, 121, 63 };
  std::vector <int> fin(row * col);
  fin = ParallelSmoothing(matr, row, col, smoothing);
  if (rank == 0) {
    ASSERT_EQ(seq_fin, fin);
  }
}

TEST(SMOOTHING_IMAGE_MPI, test4_3x4) {
  int row = 3, col = 4, smoothing = 60;
  std::vector <int> matr(row * col), seq_fin(row * col);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  matr = { 131, 86, 94, 200, 185, 140, 170, 200, 176, 147, 99, 151 };
  seq_fin = { 175, 247, 234, 64, 88, 160, 112, 64, 103, 149, 226, 143 };
  std::vector <int> fin(row * col);
  fin = ParallelSmoothing(matr, row, col, smoothing);
  if (rank == 0) {
    ASSERT_EQ(seq_fin, fin);
  }
}

TEST(SMOOTHING_IMAGE_MPI, test5_9x15) {
  int row = 9, col = 15, smoothing = 20;
  std::vector <int> matr(row * col);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    matr = getImg(row, col);
  }
  std::vector <int> fin(row * col);
  fin = ParallelSmoothing(matr, row, col, smoothing);
  std::vector <int> seq_fin(row * col);
  if (rank == 0) {
    seq_fin = SequintialSmoothing(matr, row, col, smoothing);
    ASSERT_EQ(seq_fin, fin);
  }
}

TEST(SMOOTHING_IMAGE_MPI, test6_1366x768) {
  int row = 1366, col = 768, smoothing = 80;
  std::vector <int> matr(row * col);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    matr = getImg(row, col);
  }
  std::vector <int> fin(row * col);
  fin = ParallelSmoothing(matr, row, col, smoothing);
  std::vector <int> seq_fin(row * col);
  if (rank == 0) {
    seq_fin = SequintialSmoothing(matr, row, col, smoothing);
    ASSERT_EQ(seq_fin, fin);
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
