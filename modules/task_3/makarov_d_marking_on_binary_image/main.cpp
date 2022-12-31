// Copyright 2022 Makarov Danila
#include <gtest/gtest.h>

#include <gtest-mpi-listener.hpp>

#include "./marking_on_binary_image.h"

TEST(Marking_on_binary_image, test_1_custom) {
  std::vector<int> vec = preset1();
  int colls = vec.back();
  vec.pop_back();
  int rows = vec.back();
  vec.pop_back();

  int ProcNum, ProcRank;
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

  if (ProcNum == 1) return;
  if (ProcNum > rows) return;
  MPI_Status status;

  std::vector<int> vecmpi = rowsDistrib(vec, rows, colls);
  if (ProcRank == 0) {
    std::vector<int> vecnonmpi = marking(vec, rows, colls);
    for (int i = 0; i < vecmpi.size(); i++) ASSERT_EQ(vecmpi[i], vecnonmpi[i]);
  }
  MPI_Barrier(MPI_COMM_WORLD);
}

TEST(Marking_on_binary_image, test_2_custom) {
  std::vector<int> vec = preset2();
  int colls = vec.back();
  vec.pop_back();
  int rows = vec.back();
  vec.pop_back();

  int ProcNum, ProcRank;
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

  if (ProcNum == 1) return;
  if (ProcNum > rows) return;
  MPI_Status status;

  std::vector<int> vecmpi = rowsDistrib(vec, rows, colls);
  if (ProcRank == 0) {
    std::vector<int> vecnonmpi = marking(vec, rows, colls);
    for (int i = 0; i < vecmpi.size(); i++) ASSERT_EQ(vecmpi[i], vecnonmpi[i]);
  }
  MPI_Barrier(MPI_COMM_WORLD);
}

TEST(Marking_on_binary_image, test_3_not_intersect) {
  std::vector<int> vec = preset3();
  int colls = vec.back();
  vec.pop_back();
  int rows = vec.back();
  vec.pop_back();

  int ProcNum, ProcRank;
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

  if (ProcNum == 1) return;
  if (ProcNum > rows) return;
  MPI_Status status;

  std::vector<int> vecmpi = rowsDistrib(vec, rows, colls);
  if (ProcRank == 0) {
    std::vector<int> vecnonmpi = marking(vec, rows, colls);
    for (int i = 0; i < vecmpi.size(); i++) ASSERT_EQ(vecmpi[i], vecnonmpi[i]);
  }
  MPI_Barrier(MPI_COMM_WORLD);
}

TEST(Marking_on_binary_image, test_4_1) {
  std::vector<int> vec = preset4();
  int colls = vec.back();
  vec.pop_back();
  int rows = vec.back();
  vec.pop_back();

  int ProcNum, ProcRank;
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

  if (ProcNum == 1) return;
  if (ProcNum > rows) return;
  MPI_Status status;

  std::vector<int> vecmpi = rowsDistrib(vec, rows, colls);
  if (ProcRank == 0) {
    std::vector<int> vecnonmpi = marking(vec, rows, colls);
    for (int i = 0; i < vecmpi.size(); i++) ASSERT_EQ(vecmpi[i], vecnonmpi[i]);
  }
  MPI_Barrier(MPI_COMM_WORLD);
}

TEST(Marking_on_binary_image, test_5_0) {
  std::vector<int> vec = preset5();
  int colls = vec.back();
  vec.pop_back();
  int rows = vec.back();
  vec.pop_back();

  int ProcNum, ProcRank;
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

  if (ProcNum == 1) return;
  if (ProcNum > rows) return;
  MPI_Status status;

  std::vector<int> vecmpi = rowsDistrib(vec, rows, colls);
  if (ProcRank == 0) {
    std::vector<int> vecnonmpi = marking(vec, rows, colls);
    for (int i = 0; i < vecmpi.size(); i++) ASSERT_EQ(vecmpi[i], vecnonmpi[i]);
  }
  MPI_Barrier(MPI_COMM_WORLD);
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
