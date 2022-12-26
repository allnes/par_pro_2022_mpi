// Copyright 2022 Popov Andrey
#include <gtest/gtest.h>

#include <gtest-mpi-listener.hpp>
#include <random>
#include <vector>

#include "../../../modules/task_1/Popov_A_close_elems_on_vector/close_elems_on_vector.h"

TEST(close_elems_on_vector, close_elems_on_vector_one) {
  int lvl;
  MPI_Comm_rank(MPI_COMM_WORLD, &lvl);
  std::vector<int> vect = {0, 6, 8, 16, 20, 30};
  if (lvl == 0) {
    int differ = getSeriallOps(vect);
    ASSERT_EQ(differ, 2);
  }
}

TEST(close_elems_on_vector, close_elems_on_vector_oneElem) {
  int lvl;
  MPI_Comm_rank(MPI_COMM_WORLD, &lvl);
  std::vector<int> vect;
  vect = getRandVect(1);
  int differ = getParallelOps(vect);
  if (lvl == 0) {
    ASSERT_EQ(differ, getSeriallOps(vect));
  }
}

TEST(close_elems_on_vector, close_elems_on_vector_one_oneElem) {
  int lvl;
  MPI_Comm_rank(MPI_COMM_WORLD, &lvl);
  std::vector<int> vect = {3};
  if (lvl == 0) {
    int differ = getSeriallOps(vect);
    ASSERT_EQ(differ, 0);
  }
}

TEST(close_elems_on_vector, close_elems_on_vector_10Elem) {
  int lvl;
  MPI_Comm_rank(MPI_COMM_WORLD, &lvl);
  std::vector<int> vect;
  vect = getRandVect(10);
  int differ = getParallelOps(vect);
  if (lvl == 0) {
    ASSERT_EQ(differ, getSeriallOps(vect));
  }
}

TEST(close_elems_on_vector, close_elems_on_vector_50Elem) {
  int lvl;
  MPI_Comm_rank(MPI_COMM_WORLD, &lvl);
  std::vector<int> vect;
  vect = getRandVect(50);
  int differ = getParallelOps(vect);
  if (lvl == 0) {
    ASSERT_EQ(differ, getSeriallOps(vect));
  }
}

TEST(close_elems_on_vector, close_elems_on_vector_100Elem) {
  int lvl;
  MPI_Comm_rank(MPI_COMM_WORLD, &lvl);
  std::vector<int> vect;
  vect = getRandVect(100);
  int differ = getParallelOps(vect);
  if (lvl == 0) {
    ASSERT_EQ(differ, getSeriallOps(vect));
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
