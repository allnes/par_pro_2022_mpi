// Copyright 2022 Elistratov Vladimir
#include <gtest/gtest.h>
#include "./letter_count.h"
#include <gtest-mpi-listener.hpp>


TEST(Parallel_Count_Letters_MPI, Size_0) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int elements_count = 0;
  std::string result;
  if (rank == 0) result = CreateRandomString(elements_count);
  int sum = calculateCountParallel(result, elements_count);
  if (rank == 0) {
    int control_sum = calculateCountSequental(result);
    ASSERT_EQ(control_sum, sum);
  }
}

TEST(Parallel_Count_Letters_MPI, Size_1) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int elements_count = 1;
  std::string result;
  if (rank == 0) result = CreateRandomString(elements_count);
  int sum = calculateCountParallel(result, elements_count);
  if (rank == 0) {
    int control_sum = calculateCountSequental(result);
    ASSERT_EQ(control_sum, sum);
  }
}

TEST(Parallel_Count_Letters_MPI, Size_25) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int elements_count = 25;
  std::string result;
  if (rank == 0) result = CreateRandomString(elements_count);
  int sum = calculateCountParallel(result, elements_count);
  if (rank == 0) {
    int control_sum = calculateCountSequental(result);
    ASSERT_EQ(control_sum, sum);
  }
}

TEST(Parallel_Count_Letters_MPI, Size_100) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int elements_count = 100;
  std::string result;
  if (rank == 0) result = CreateRandomString(elements_count);
  int sum = calculateCountParallel(result, elements_count);
  if (rank == 0) {
    int control_sum = calculateCountSequental(result);
    ASSERT_EQ(control_sum, sum);
  }
}

TEST(Parallel_Count_Letters_MPI, Size_1000) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int elements_count = 1000;
  std::string result;
  if (rank == 0) result = CreateRandomString(elements_count);
  int sum = calculateCountParallel(result, elements_count);
  if (rank == 0) {
    int control_sum = calculateCountSequental(result);
    ASSERT_EQ(control_sum, sum);
  }
}


int main(int argc, char* argv[]) {
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
