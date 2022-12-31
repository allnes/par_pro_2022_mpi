// Copyright 2022 Nikitin Alexander
#include <gtest/gtest.h>
#include <vector>
#include "./vector_min_value.h"
#include <gtest-mpi-listener.hpp>

TEST(Parallel_Operations_MPI, TEST_FIND_MIN_VAL_IN_VECTOR) {
  int commRank;
  std::vector<int> vector;

  MPI_Comm_rank(MPI_COMM_WORLD, &commRank);

  if (commRank == 0) {
    vector = Random(100);
  }

  int minFir = Min(vector);

  if (commRank == 0) {
    int minSec = Min(vector);
    ASSERT_EQ(minFir, minSec);
  }
}

TEST(Parallel_Operations_MPI, TEST_2) {
  int commRank;
  std::vector<int> vector;

  MPI_Comm_rank(MPI_COMM_WORLD, &commRank);

  if (commRank == 0) {
    vector = Random(100);
  }

  int minFir = Min(vector);

  if (commRank == 0) {
    int minSec = Min(vector);
    ASSERT_EQ(minFir, minSec);
  }
}

TEST(Parallel_Operations_MPI, TEST_3) {
  int commRank;
  std::vector<int> vector;
  int minFir, minSec;

  MPI_Comm_rank(MPI_COMM_WORLD, &commRank);
  if (commRank == 0) {
    vector = Random(4);
    minFir = Min(vector);
  }
  minSec = MinParallel(vector, 4);
  if (commRank == 0) {
    ASSERT_EQ(minFir, minSec);
  }
}

TEST(Parallel_Operations_MPI,
     TEST_PUT_VECTOR_HAS_FEWER_ELEMENTS_THAN_CPU_CORES_GET_MIN_VAL) {
  int commRank;
  std::vector<int> vector;
  int minFir, minSec;

  MPI_Comm_rank(MPI_COMM_WORLD, &commRank);
  if (commRank == 0) {
    vector = Random(4);
    minFir = Min(vector);
  }
  minSec = MinParallel(vector, 4);
  if (commRank == 0) {
    ASSERT_EQ(minFir, minSec);
  }
}

TEST(Parallel_Operations_MPI, TEST_4) {
  int commRank;
  std::vector<int> vector;
  int minFir, minSec;

  MPI_Comm_rank(MPI_COMM_WORLD, &commRank);
  if (commRank == 0) {
    vector = Random(100);
    minFir = Min(vector);
  }
  minSec = MinParallel(vector, 100);
  if (commRank == 0) {
    ASSERT_EQ(minFir, minSec);
  }
}

TEST(Parallel_Operations_MPI, TEST_PARALLEL_FIND_MIN_VAL_IN_VECTOR_GET_VAL) {
  int commRank;
  std::vector<int> vector;
  int minFir, minSec;

  MPI_Comm_rank(MPI_COMM_WORLD, &commRank);
  if (commRank == 0) {
    vector = Random(100);
    minFir = Min(vector);
  }
  minSec = MinParallel(vector, 100);
  if (commRank == 0) {
    ASSERT_EQ(minFir, minSec);
  }
}

TEST(Parallel_Operations_MPI,
     TEST_SIZE_VECTOR_LARGE_VALUE_TO_FUNCTION_PARALLEL_FIND_GET_MIN_VAL) {
  int commRank;
  std::vector<int> vector;
  int minFir, minSec;

  MPI_Comm_rank(MPI_COMM_WORLD, &commRank);
  if (commRank == 0) {
    vector = Random(20000000);
    minFir = Min(vector);
  }
  minSec = MinParallel(vector, 20000000);
  if (commRank == 0) {
    ASSERT_EQ(minFir, minSec);
  }
}

TEST(Parallel_Operations_MPI,
     TEST_PUT_NEGATIVE_SIZE_VALUE_TO_FUNCTION_FILL_VECTOR) {
  int commRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &commRank);

  ASSERT_ANY_THROW(Random(-15));
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
