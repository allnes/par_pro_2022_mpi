// Copyright 2022 Elistratov Vladimir

#include <gtest/gtest.h>
#include <vector>
#include "./betcher_sort.h"
#include <gtest-mpi-listener.hpp>

TEST(Radix_Sort_Merge_Batcher, TEST_10) {
  int ProcRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  std::vector<int> vec;
  const int n = 10;
  vec = GenRandVector(n);
  std::vector<int> vec1 = BetcherMerge(vec, n);
  if (ProcRank == 0) {
    std::vector<int> vec2 = SequentialRadixSort(vec);
    ASSERT_EQ(vec1, vec2);
  }
}
TEST(Radix_Sort_Merge_Batcher, TEST_40) {
  int ProcRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  std::vector<int> vec;
  int n = 40;
  vec = GenRandVector(n);
  std::vector<int> vec1 = BetcherMerge(vec, n);
  if (ProcRank == 0) {
    std::vector<int> vec2 = SequentialRadixSort(vec);
    ASSERT_EQ(vec1, vec2);
  }
}
TEST(Radix_Sort_Merge_Batcher, TEST_50) {
  int ProcRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  std::vector<int> vec;
  int n = 50;
  vec = GenRandVector(n);
  std::vector<int> vec1 = BetcherMerge(vec, n);
  if (ProcRank == 0) {
    std::vector<int> vec2 = SequentialRadixSort(vec);
    ASSERT_EQ(vec1, vec2);
  }
}
TEST(Radix_Sort_Merge_Batcher, TEST_100) {
  int ProcRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  std::vector<int> vec;
  int n = 100;
  vec = GenRandVector(n);
  std::vector<int> vec1 = BetcherMerge(vec, n);
  if (ProcRank == 0) {
    std::vector<int> vec2 = SequentialRadixSort(vec);
    ASSERT_EQ(vec1, vec2);
  }
}
TEST(Radix_Sort_Merge_Batcher, TEST_1000) {
  int ProcRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  std::vector<int> vec;
  int n = 1000;
  vec = GenRandVector(n);
  std::vector<int> vec1 = BetcherMerge(vec, n);
  if (ProcRank == 0) {
    std::vector<int> vec2 = SequentialRadixSort(vec);
    ASSERT_EQ(vec1, vec2);
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
