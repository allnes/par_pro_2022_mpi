// Copyright 2022 Eremin Aleksandr
#include <gtest/gtest.h>
#include <vector>
#include "./ops_mpi.h"
#include <gtest-mpi-listener.hpp>

TEST(Sleepeing_barber, two_places_in_the_queue) {
  int rank, size, queue_length = 2;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  if (rank == 0) {
    ASSERT_NO_THROW(Barber(queue_length, size - 2));
  } else if (rank == 1) {
    ASSERT_NO_THROW(queue(queue_length, size - 2));
  } else {
    ASSERT_NO_THROW(client(rank));
  }
}

TEST(Sleepeing_barber, five_places_in_the_queue) {
  int rank, size, queue_length = 5;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  if (rank == 0) {
    ASSERT_NO_THROW(Barber(queue_length, size - 2));
  } else if (rank == 1) {
    ASSERT_NO_THROW(queue(queue_length, size - 2));
  } else {
    ASSERT_NO_THROW(client(rank));
  }
}

TEST(Sleepeing_barber, ten_places_in_the_queue) {
  int rank, size, queue_length = 10;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  if (rank == 0) {
    ASSERT_NO_THROW(Barber(queue_length, size - 2));
  } else if (rank == 1) {
    ASSERT_NO_THROW(queue(queue_length, size - 2));
  } else {
    ASSERT_NO_THROW(client(rank));
  }
}

TEST(Sleepeing_barber, seventeen_places_in_the_queue) {
  int rank, size, queue_length = 17;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  if (rank == 0) {
    ASSERT_NO_THROW(Barber(queue_length, size - 2));
  } else if (rank == 1) {
    ASSERT_NO_THROW(queue(queue_length, size - 2));
  } else {
    ASSERT_NO_THROW(client(rank));
  }
}

TEST(Sleepeing_barber, thirtyseven_places_in_the_queue) {
  int rank, size, queue_length = 37;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  if (rank == 0) {
    ASSERT_NO_THROW(Barber(queue_length, size - 2));
  } else if (rank == 1) {
    ASSERT_NO_THROW(queue(queue_length, size - 2));
  } else {
    ASSERT_NO_THROW(client(rank));
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
