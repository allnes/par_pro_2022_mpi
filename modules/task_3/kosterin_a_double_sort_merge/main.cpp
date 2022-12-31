// Copyright 2022 Kosterin Alexey
#include <gtest/gtest.h>
#include "../../../modules/task_3/kosterin_a_double_sort_merge/double_sort_merge.h"
#include <gtest-mpi-listener.hpp>

TEST(Sort_Merge_MPI, sort_vector_positive) {
  int rank = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int size = 5;
  double *vector = new double[size]{2.0, 8.66667, 1.69, 99.147, 57.3579};

  ParallelSort(&vector, size);
  if (0 == rank) {
    bool result = false;
    result = CheckSort(vector, size);
    ASSERT_EQ(true, result);
  }
}

TEST(Sort_Merge_MPI, sort_large_vector) {
  int rank = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int size = 1000;
  double *vector = new double[size];
  for (int i = 0; i < size; i++)
    vector[i] = (size - i) * 0.001 + (i + size / 2);

  ParallelSort(&vector, size);
  if (0 == rank) {
    bool result = false;
    result = CheckSort(vector, size);
    ASSERT_EQ(true, result);
  }
}

TEST(Sort_Merge_MPI, sort_is_correct) {
  int rank = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int size = 5;
  double *vector = new double[size]{2.0, 8.66667, 1.69, 99.147, 57.3579};

  ParallelSort(&vector, size);
  if (0 == rank) {
    bool result = false;
    result = CheckSort(vector, size);
    ASSERT_EQ(true, result);
  }
}

TEST(Sort_Merge_MPI, sort_vector) {
  int rank = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int size = 5;
  double *vector = new double[size]{99.147, 8.66667, 1.69, 2.0, 57.3579};

  ParallelSort(&vector, size);

  if (0 == rank) {
    bool result = false;
    result = CheckSort(vector, size);
    ASSERT_EQ(true, result);
  }
  delete[] vector;
}

TEST(Sort_Merge_MPI, sort_large_vector_two) {
  int rank = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int size = 1000;
  double *vector = new double[size];
  for (int i = 0; i < size; i++)
    vector[i] = (size - i) * 0.001 + (i + size / 2);

  ParallelSort(&vector, size);

  if (0 == rank) {
    bool result = false;
    result = CheckSort(vector, size);
    ASSERT_EQ(true, result);
  }
  delete[] vector;
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  MPI_Init(&argc, &argv);

  ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
  ::testing::TestEventListeners &listeners =
      ::testing::UnitTest::GetInstance()->listeners();

  listeners.Release(listeners.default_result_printer());
  listeners.Release(listeners.default_xml_generator());

  listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
  return RUN_ALL_TESTS();
}
