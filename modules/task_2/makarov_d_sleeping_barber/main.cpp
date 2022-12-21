// Copyright 2022 Makarov Danila
#include <gtest/gtest.h>

#include <gtest-mpi-listener.hpp>

#include "./sleeping_barber.h"

TEST(Sleeping_barber, one_chair) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (size < 3) return;

  if (rank == 0) {
    barber(size - 2);
  } else if (rank == 1) {
    waitingRoom(1, size - 2);
  } else {
    client();
  }
  MPI_Barrier(MPI_COMM_WORLD);
}

TEST(Sleeping_barber, two_chairs) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  if (rank == 0) {
    barber(size - 2);
  } else if (rank == 1) {
    waitingRoom(2, size - 2);
  } else {
    client();
  }
  MPI_Barrier(MPI_COMM_WORLD);
}

TEST(Sleeping_barber, three_chairs) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (size < 3) return;

  if (rank == 0) {
    barber(size - 2);
  } else if (rank == 1) {
    waitingRoom(3, size - 2);
  } else {
    client();
  }
  MPI_Barrier(MPI_COMM_WORLD);
}

TEST(Sleeping_barber, four_chairs) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (size < 3) return;

  if (rank == 0) {
    barber(size - 2);
  } else if (rank == 1) {
    waitingRoom(4, size - 2);
  } else {
    client();
  }
  MPI_Barrier(MPI_COMM_WORLD);
}

TEST(Sleeping_barber, five_chairs) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (size < 3) return;

  if (rank == 0) {
    barber(size - 2);
  } else if (rank == 1) {
    waitingRoom(5, size - 2);
  } else {
    client();
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
