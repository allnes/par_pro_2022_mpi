// Copyright 2022 Chernova Anna

#include <gtest/gtest.h>

#include <gtest-mpi-listener.hpp>
#include <iostream>

#include "../chernova_a_gather/gather.h"

TEST(GATHER, IS_GATHER_INT) {
  int rank, numProc;
  MPI_Comm_size(MPI_COMM_WORLD, &numProc);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int sendBuff[10];
  int* recvBuff;
  recvBuff = new int[10 * numProc];
  random(sendBuff, 10);
  my_gather(sendBuff, 10, MPI_INT, recvBuff, 10, MPI_INT, 0, MPI_COMM_WORLD);
  if (rank == 0) {
    for (int i = 0; i < 10 * numProc; i++) {
      EXPECT_TRUE(recvBuff[i] <= MAX_RANDOM_INT &&
                  recvBuff[i] >= MIN_RANDOM_INT);
    }
  }
}

TEST(GATHER, IS_GATHER_DOUBLE) {
  int rank, numProc;
  MPI_Comm_size(MPI_COMM_WORLD, &numProc);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  double sendBuff[10];
  double* recvBuff;
  recvBuff = new double[10 * numProc];
  random(sendBuff, 10);
  my_gather(sendBuff, 10, MPI_DOUBLE, recvBuff, 10, MPI_DOUBLE, 0,
            MPI_COMM_WORLD);
  if (rank == 0) {
    for (int i = 0; i < 10 * numProc; i++) {
      EXPECT_TRUE(recvBuff[i] <= MAX_RANDOM_DOUBLE &&
                  recvBuff[i] >= MIN_RANDOM_DOUBLE);
    }
  }
}

TEST(GATHER, IS_GATHER_FLOAT) {
  int rank, numProc;
  MPI_Comm_size(MPI_COMM_WORLD, &numProc);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  float sendBuff[10];
  float* recvBuff;
  recvBuff = new float[10 * numProc];
  random(sendBuff, 10);
  my_gather(sendBuff, 10, MPI_FLOAT, recvBuff, 10, MPI_FLOAT, 0,
            MPI_COMM_WORLD);
  if (rank == 0) {
    for (int i = 0; i < 10 * numProc; i++) {
      EXPECT_TRUE(recvBuff[i] <= MAX_RANDOM_FLOAT &&
                  recvBuff[i] >= MIN_RANDOM_FLOAT);
    }
  }
}

TEST(GATHER, BIG_DATA) {
  int rank, numProc;
  MPI_Comm_size(MPI_COMM_WORLD, &numProc);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int sendBuff[1000];
  int* recvBuff;
  recvBuff = new int[1000 * numProc];
  random(sendBuff, 1000);
  my_gather(sendBuff, 1000, MPI_INT, recvBuff, 1000, MPI_INT, 0,
            MPI_COMM_WORLD);
  if (rank == 0) {
    for (int i = 0; i < 1000 * numProc; i++) {
      EXPECT_TRUE(recvBuff[i] <= MAX_RANDOM_INT &&
                  recvBuff[i] >= MIN_RANDOM_INT);
    }
  }
}

TEST(GATHER, IS_GENERATE_ERR) {
  int rank, numProc;
  MPI_Comm_size(MPI_COMM_WORLD, &numProc);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int sendBuff[10];
  double* recvBuff;
  recvBuff = new double[10 * numProc];
  random(sendBuff, 10);
  EXPECT_EQ(my_gather(sendBuff, 10, MPI_INT, recvBuff, 10, MPI_DOUBLE, 0,
                      MPI_COMM_WORLD),
            MPI_ERR_OTHER);
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
