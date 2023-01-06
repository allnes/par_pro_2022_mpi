// Copyright 2022 Bushmakin Maksim
#include <gtest/gtest.h>

#include <gtest-mpi-listener.hpp>

#include "./sleep_barber.h"

TEST(SLEEPING_BARBER, ONE_SEAT) {
  int ProcRank, ProcNum;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);

  if (ProcNum < 3) return;

  if (ProcRank == 0) barber(ProcNum - 2);
  if (ProcRank == 1) queue(1, ProcNum - 2);
  if (ProcRank > 1) customer();
}

TEST(SLEEPING_BARBER, TWO_SEATS) {
  int ProcRank, ProcNum;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);

  if (ProcNum < 3) return;

  if (ProcRank == 0) barber(ProcNum - 2);
  if (ProcRank == 1) queue(2, ProcNum - 2);
  if (ProcRank > 1) customer();
}

TEST(SLEEPING_BARBER, THREE_SEATS) {
  int ProcRank, ProcNum;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);

  if (ProcNum < 3) return;

  if (ProcRank == 0) barber(ProcNum - 2);
  if (ProcRank == 1) queue(3, ProcNum - 2);
  if (ProcRank > 1) customer();
}

TEST(SLEEPING_BARBER, FIVE_SEATS) {
  int ProcRank, ProcNum;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);

  if (ProcNum < 3) return;

  if (ProcRank == 0) barber(ProcNum - 2);
  if (ProcRank == 1) queue(5, ProcNum - 2);
  if (ProcRank > 1) customer();
}

TEST(SLEEPING_BARBER, TEN_SEATS) {
  int ProcRank, ProcNum;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);

  if (ProcNum < 3) return;

  if (ProcRank == 0) barber(ProcNum - 2);
  if (ProcRank == 1) queue(10, ProcNum - 2);
  if (ProcRank > 1) customer();
}

TEST(SLEEPING_BARBER, TWENTY_SEATS) {
  int ProcRank, ProcNum;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);

  if (ProcNum < 3) return;

  if (ProcRank == 0) barber(ProcNum - 2);
  if (ProcRank == 1) queue(20, ProcNum - 2);
  if (ProcRank > 1) customer();
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
