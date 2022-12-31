// Copyright 2022 Nikitin Alexander
#include <gtest/gtest.h>
#include "./nikitin_a_topology_ring.h"
#include <gtest-mpi-listener.hpp>

TEST(Ring_Topology, ring_topology) { ASSERT_NO_THROW(ring(MPI_COMM_WORLD)); }

TEST(Ring_Topology, left_border_of_0_process) {
  int size = 0;
  int rank = 0;
  int borders[2];
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm localRing = ring(MPI_COMM_WORLD);

  if (rank == 0) {
    MPI_Cart_shift(localRing, 0, 1, &borders[0], &borders[1]);
    ASSERT_EQ(size - 1, borders[0]);
  }
}

TEST(Ring_Topology, from_1_to_4) {
  int rank = 0;
  int size = 0;
  int send = 16;
  int get = 0;
  MPI_Status status;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm localRing = ring(MPI_COMM_WORLD);
  if (size > 4) {
    if (rank == 1) {
      MPI_Send(&send, 1, MPI_INT, 4, 1, localRing);
    }
    if (rank == 4) {
      MPI_Recv(&get, 1, MPI_INT, 1, 1, localRing, &status);
      ASSERT_EQ(send, get);
    }
  }
}

TEST(Ring_Topology, from_4_to_1) {
  int rank = 0;
  int size = 0;
  int send = 8;
  int get = 0;
  MPI_Status status;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm localRing = ring(MPI_COMM_WORLD);
  if (size > 4) {
    if (rank == 4) {
      MPI_Send(&send, 1, MPI_INT, 1, 1, localRing);
    }
    if (rank == 1) {
      MPI_Recv(&get, 1, MPI_INT, 4, 1, localRing, &status);
      ASSERT_EQ(send, get);
    }
  }
}

TEST(Ring_Topology, right_border_of_0_process) {
  int size = 0;
  int rank = 0;
  int borders[2];
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm localRing = ring(MPI_COMM_WORLD);

  if (size > 1 && rank == 0) {
    MPI_Cart_shift(localRing, 0, 1, &borders[0], &borders[1]);
    ASSERT_EQ(1, borders[1]);
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
