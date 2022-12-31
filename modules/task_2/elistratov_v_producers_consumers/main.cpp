// Copyright 2022 Elistratov Vladimir
#include <gtest/gtest.h>
#include <vector>
#include "./elistratov_v_producers_consumers.h"
#include <gtest-mpi-listener.hpp>

TEST(Parallel_Operations_MPI, Test_task_1) {
  int size, rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  int queue_size;
  int N1 = 2;
  int N2 = 3;

  if (rank == 0) {
    queue_size = master((N2 * (size / 2) + N1 * ((size - 1) / 2)));
  }
  sleep_seconds(0.5);
  if (rank != 0) {
    if (rank % 2 == 0) {
      for (int i = 0; i < N1; i++) {
        consume();
      }
    } else {
      for (int i = 0; i < N2; i++) {
        produce();
      }
    }
  }

  if (rank == 0) {
    ASSERT_EQ(queue_size, (N2 * (size / 2) - N1 * ((size - 1) / 2)));
  }
  MPI_Barrier(MPI_COMM_WORLD);
}

TEST(Parallel_Operations_MPI, Test_task_2) {
  int size, rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  int queue_size;
  int N1 = 5;
  int N2 = 5;
  MPI_Barrier(MPI_COMM_WORLD);
  if (rank == 0) {
    queue_size = master((N2 * (size / 2) + N1 * ((size - 1) / 2)));
  }

  if (rank != 0) {
    if (rank % 2 == 0) {
      for (int i = 0; i < N1; i++) {
        consume();
      }
    } else {
      for (int i = 0; i < N2; i++) {
        produce();
      }
    }
  }

  if (rank == 0) {
    ASSERT_EQ(queue_size, (N2 * (size / 2) - N1 * ((size - 1) / 2)));
  }
  MPI_Barrier(MPI_COMM_WORLD);
}

TEST(Parallel_Operations_MPI, Test_task_3) {
  int size, rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  int queue_size;
  int N1 = 2;
  int N2 = 4;

  if (rank == 0) {
    queue_size = master((N2 * (size / 2) + N1 * ((size - 1) / 2)));
  }

  if (rank != 0) {
    if (rank % 2 == 0) {
      for (int i = 0; i < N1; i++) {
        consume();
      }
    } else {
      for (int i = 0; i < N2; i++) {
        produce();
      }
    }
  }
  if (rank == 0) {
    ASSERT_EQ(queue_size, (N2 * (size / 2) - N1 * ((size - 1) / 2)));
  }
  MPI_Barrier(MPI_COMM_WORLD);
}

TEST(Parallel_Operations_MPI, Test_task_4) {
  int size, rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  int queue_size;
  int N1 = 5;
  int N2 = 8;

  if (rank == 0) {
    queue_size = master((N2 * (size / 2) + N1 * ((size - 1) / 2)));
  }

  if (rank != 0) {
    if (rank % 2 == 0) {
      for (int i = 0; i < N1; i++) {
        consume();
      }
    } else {
      for (int i = 0; i < N2; i++) {
        produce();
      }
    }
  }
  if (rank == 0) {
    ASSERT_EQ(queue_size, (N2 * (size / 2) - N1 * ((size - 1) / 2)));
  }
  MPI_Barrier(MPI_COMM_WORLD);
}

TEST(Parallel_Operations_MPI, Test_task_5) {
  int size, rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  int queue_size;
  int N1 = 10;
  int N2 = 10;

  if (rank == 0) {
    queue_size = master((N2 * (size / 2) + N1 * ((size - 1) / 2)));
  }

  if (rank != 0) {
    if (rank % 2 == 0) {
      for (int i = 0; i < N1; i++) {
        consume();
      }
    } else {
      for (int i = 0; i < N2; i++) {
        produce();
      }
    }
  }
  if (rank == 0) {
    ASSERT_EQ(queue_size, (N2 * (size / 2) - N1 * ((size - 1) / 2)));
  }
  MPI_Barrier(MPI_COMM_WORLD);
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
