// Copyright 2022 Kolesov Maxim
#include <gtest/gtest.h>
#include <vector>
#include "./shell_simple.h"
#include <gtest-mpi-listener.hpp>

TEST(kolesov_shell_simple, test1) {
  int rank;
  std::vector<int> vec;
  int size = 8;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0) {
    vec = GenerateVector(size);
  }

  std::vector<int> result = Shell(vec, size);

  if (rank == 0) {
    ShellSequence(&vec);
    ASSERT_EQ(vec, result);
  }
}

TEST(kolesov_shell_simple, test2) {
  int rank;
  std::vector<int> vec;
  int size = 10;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0) {
    vec = GenerateVector(size);
  }

  std::vector<int> result = Shell(vec, size);

  if (rank == 0) {
    ShellSequence(&vec);
    ASSERT_EQ(vec, result);
  }
}

TEST(kolesov_shell_simple, test3) {
  int rank;
  std::vector<int> vec;
  int size = 9;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0) {
    vec = GenerateVector(size);
  }

  std::vector<int> result = Shell(vec, size);

  if (rank == 0) {
    ShellSequence(&vec);
    ASSERT_EQ(vec, result);
  }
}

TEST(kolesov_shell_simple, test4) {
  int rank;
  std::vector<int> vec;
  int size = 20;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0) {
    vec = GenerateVector(size);
  }

  std::vector<int> result = Shell(vec, size);

  if (rank == 0) {
    ShellSequence(&vec);
    ASSERT_EQ(vec, result);
  }
}

TEST(kolesov_shell_simple, test5) {
  int rank;
  std::vector<int> vec;
  int size = 25;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0) {
    vec = GenerateVector(size);
  }

  std::vector<int> result = Shell(vec, size);

  if (rank == 0) {
    ShellSequence(&vec);
    ASSERT_EQ(vec, result);
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
