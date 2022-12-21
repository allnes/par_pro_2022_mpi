// Copyright 2022 Korobeynikova Alisa

#include <gtest/gtest.h>

#include <gtest-mpi-listener.hpp>

#include "./quick_merge_sort.h"

std::vector<int> seqSolution(const std::vector<int>& m) {
  std::vector<int> seq_sort_res(m);
  std::sort(seq_sort_res.begin(), seq_sort_res.end());
  return seq_sort_res;
}

void setRandomValues(std::vector<int> *vec) {
  std::random_device dev;
  std::mt19937 gen(dev());
  for (int i = 0; i < vec->size(); ++i) {
    vec->at(i) = gen() % 100;
  }
}

TEST(Parallel_Operations_MPI, Test_Sort) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  const int size = 27;
  std::vector<int> global_vec(size);
  setRandomValues(&global_vec);

  std::vector<int> ps = parallelSort(global_vec);

  if (rank == 0) {
    std::vector<int> ss = seqSolution(global_vec);
    ASSERT_EQ(ps, ss);
  }
}

TEST(Parallel_Operations_MPI, Test_Sort_2) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  const int size = 30;
  std::vector<int> global_vec(size);
  setRandomValues(&global_vec);

  std::vector<int> ps = parallelSort(global_vec);

  if (rank == 0) {
    std::vector<int> ss = seqSolution(global_vec);
    ASSERT_EQ(ps, ss);
  }
}

TEST(Parallel_Operations_MPI, Test_Sort_3) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  const int size = 15;
  std::vector<int> global_vec(size);
  setRandomValues(&global_vec);

  std::vector<int> ps = parallelSort(global_vec);

  if (rank == 0) {
    std::vector<int> ss = seqSolution(global_vec);
    ASSERT_EQ(ps, ss);
  }
}

TEST(Parallel_Operations_MPI, Test_Sort_4) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  const int size = 7;
  std::vector<int> global_vec(size);
  setRandomValues(&global_vec);

  std::vector<int> ps = parallelSort(global_vec);

  if (rank == 0) {
    std::vector<int> ss = seqSolution(global_vec);
    ASSERT_EQ(ps, ss);
  }
}

TEST(Parallel_Operations_MPI, Test_Sort_5) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  const int size = 50;
  std::vector<int> global_vec(size);
  setRandomValues(&global_vec);

  std::vector<int> ps = parallelSort(global_vec);

  if (rank == 0) {
    std::vector<int> ss = seqSolution(global_vec);
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
