// Copyright 2022 Kolesov Maxim
#include <gtest/gtest.h>
#include <vector>
#include "./ring.h"
#include <gtest-mpi-listener.hpp>

TEST(ring_mpi, testDir) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  std::vector<int> ranks;

  if (size != 1) {
    if (rank == 0) {
      ASSERT_EQ(ChooseDirection(0, 1, MPI_COMM_WORLD, &ranks), 1);
    }
  }

  if (size > 2) {
    if (rank == 0) {
      ASSERT_EQ(ChooseDirection(1, 0, MPI_COMM_WORLD, &ranks), -1);
    }
  }
}

TEST(ring_mpi, test1) {
  int rank, size;
  int test_data;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (size != 1) {
    if (rank == 0) {
      test_data = 42;
    }

    RingSend(&test_data, 1, MPI_INT, 0, 1, 0, MPI_COMM_WORLD);

    if (rank == 1) {
      ASSERT_EQ(test_data, 42);
    }
  }
}

TEST(ring_mpi, test2) {
  int rank, size;
  int test_data;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (size != 1) {
    if (rank == 0) {
      test_data = 42;
    }

    RingSend(&test_data, 1, MPI_INT, 0, size - 1, 0, MPI_COMM_WORLD);

    if (rank == size - 1) {
      ASSERT_EQ(test_data, 42);
    }
  }
}

TEST(ring_mpi, test3) {
  int rank, size;
  int test_data;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (size != 1) {
    if (rank == size - 1) {
      test_data = 42;
    }

    RingSend(&test_data, 1, MPI_INT, size - 1, 0, 0, MPI_COMM_WORLD);

    if (rank == 0) {
      ASSERT_EQ(test_data, 42);
    }
  }
}

TEST(ring_mpi, test4) {
  int rank, size;
  int test_data;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (size != 1) {
    if (rank == size - 1) {
      test_data = 42;
    }

    RingSend(&test_data, 1, MPI_INT, size - 1, size - 2, 0, MPI_COMM_WORLD);

    if (rank == size - 2) {
      ASSERT_EQ(test_data, 42);
    }
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
