// Copyright 2022 Zorin Oleg
#include <gtest/gtest.h>
#include <vector>
#include "./moore_alg.h"
#include <gtest-mpi-listener.hpp>

TEST(moore_alg, test_sequential) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int start = 0, size = 6;
  Matrix weight_matrix = {
      {INF, 4, INF, 6, INF, INF},   {INF, INF, 7, -8, 6, INF},
      {INF, INF, INF, INF, -7, 5},  {INF, INF, 8, INF, 9, INF},
      {INF, INF, INF, INF, INF, 3}, {INF, INF, INF, INF, INF, INF}};
  Vector ans = {0, 4, 4, -4, -3, 0};

  if (rank == 0) {
    Vector res = MooreAlgSequential(weight_matrix, start);
    ASSERT_EQ(ans, res);
  }
}

TEST(moore_alg, test_sequential_expanded) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int start = 0;
  Matrix weight_matrix = {
      {INF, 4, INF, 6, INF, INF},   {INF, INF, 7, -8, 6, INF},
      {INF, INF, INF, INF, -7, 5},  {INF, INF, 8, INF, 9, INF},
      {INF, INF, INF, INF, INF, 3}, {INF, INF, INF, INF, INF, INF}};
  Vector ans = {0, 4, 4, -4, -3, 0};

  if (rank == 0) {
    weight_matrix = expandMatrix(weight_matrix, 4);
    Vector res = MooreAlgSequential(weight_matrix, start);
    res.erase(res.end()- 4, res.end());
    ASSERT_EQ(ans, res);
  }
}

TEST(moore_alg, test_expand_matrix) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  Matrix matrix = {
      {INF, 4, INF, 6, INF, INF},   {INF, INF, 7, -8, 6, INF},
      {INF, INF, INF, INF, -7, 5},  {INF, INF, 8, INF, 9, INF},
      {INF, INF, INF, INF, INF, 3}, {INF, INF, INF, INF, INF, INF}};
  Matrix expanded_matrix = {
      {INF, 4, INF, 6, INF, INF, INF, INF, INF, INF},
      {INF, INF, 7, -8, 6, INF, INF, INF, INF, INF},
      {INF, INF, INF, INF, -7, 5, INF, INF, INF, INF},
      {INF, INF, 8, INF, 9, INF, INF, INF, INF, INF},
      {INF, INF, INF, INF, INF, 3, INF, INF, INF, INF},
      {INF, INF, INF, INF, INF, INF, INF, INF, INF, INF},
      {INF, INF, INF, INF, INF, INF, INF, INF, INF, INF},
      {INF, INF, INF, INF, INF, INF, INF, INF, INF, INF},
      {INF, INF, INF, INF, INF, INF, INF, INF, INF, INF},
      {INF, INF, INF, INF, INF, INF, INF, INF, INF, INF}};

  if (rank == 0) {
    Matrix res = expandMatrix(matrix, 4);
    ASSERT_EQ(expanded_matrix, res);
  }
}

TEST(moore_alg, test_parallel_first) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int start = 0;
  Matrix weight_matrix = {
      {INF, 4, INF, 6, INF, INF},   {INF, INF, 7, -8, 6, INF},
      {INF, INF, INF, INF, -7, 5},  {INF, INF, 8, INF, 9, INF},
      {INF, INF, INF, INF, INF, 3}, {INF, INF, INF, INF, INF, INF}};
  Vector ans = {0, 4, 4, -4, -3, 0};
  Vector res = MooreAlgParallel(weight_matrix, start);

  if (rank == 0) {
    ASSERT_EQ(ans, res);
  }
}

TEST(moore_alg, test_parallel_second) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int start = 0;
  Matrix weight_matrix = {
      {INF, 9, INF, 6, 11, INF},  {INF, INF, 8, INF, INF, INF},
      {INF, INF, INF, INF, 6, 9}, {INF, 5, 7, INF, 6, INF},
      {INF, 6, INF, INF, INF, 4}, {INF, INF, INF, INF, INF, INF}};
  Vector ans = MooreAlgSequential(weight_matrix, start);
  Vector res = MooreAlgParallel(weight_matrix, start);

  if (rank == 0) {
    ASSERT_EQ(ans, res);
  }
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
