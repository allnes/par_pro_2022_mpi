// Copyright 2022 Shokurov Daniil

#include <gtest/gtest.h>
#include <gtest-mpi-listener.hpp>
#include "./quicksort_simple_merge.h"

TEST(quicksort_simple_merge, test_1) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int si = 10000;
  int* a = new int[si];
  for (int i = 0; i < si; ++i) a[i] = i;

  parallel_quick_sort(a, si);

  if (rank == 0) {
    bool flag = true;
    for (int i = 0; i < si; ++i) {
      if (a[i] != i) flag = false;
    }
    EXPECT_EQ(flag, true);
  }
  delete[] a;
}

TEST(quicksort_simple_merge, test_2) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::random_device dev;
  std::mt19937 gen(dev());
  int si = 100;
  int* a = new int[si];
  for (int i = 0; i < si; ++i) a[i] = gen();

  parallel_quick_sort(a, si);

  if (rank == 0) {
    EXPECT_EQ(check(a, si), true);
  }
  delete[] a;
}
TEST(quicksort_simple_merge, test_3) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::random_device dev;
  std::mt19937 gen(dev());
  int si = 1000;
  int* a = new int[si];
  for (int i = 0; i < si; ++i) a[i] = gen();

  parallel_quick_sort(a, si);

  if (rank == 0) {
    EXPECT_EQ(check(a, si), true);
  }
  delete[] a;
}

TEST(quicksort_simple_merge, test_4) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::random_device dev;
  std::mt19937 gen(dev());
  int si = 1500;
  int* a = new int[si];
  for (int i = 0; i < si; ++i) a[i] = gen();

  parallel_quick_sort(a, si);

  if (rank == 0) {
    EXPECT_EQ(check(a, si), true);
  }
  delete[] a;
}

TEST(quicksort_simple_merge, test_5) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::random_device dev;
  std::mt19937 gen(dev());
  int si = 3000;
  int* a = new int[si];
  for (int i = 0; i < si; ++i) a[i] = gen();

  parallel_quick_sort(a, si);

  if (rank == 0) {
    EXPECT_EQ(check(a, si), true);
  }
  delete[] a;
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
