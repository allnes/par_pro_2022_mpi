// Copyright 2022 Chernova Anna
#include <gtest/gtest.h>
#include <iostream>
#include <gtest-mpi-listener.hpp>
#include "./gather.h"

bool Compare(int* arr1, int* arr2, int size, int begin) {
  for (int i = 0; i < size; i++) {
    // std::cout << arr1[i] << " " << arr2[i+begin]<< std::endl;
    if (arr1[i] != arr2[i + begin]) {
      return false;
    }
  }
  return true;
}

TEST(GATHER, IS_GATHER_INT) {
  int rank, numProc;
  MPI_Comm_size(MPI_COMM_WORLD, &numProc);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int root = 0;
  if (root >= numProc) {
    root = 0;
  }
  int sBuff[5];
  int* rBuff;
  rBuff = new int[5 * numProc];
  getRandomVector(sBuff, 5);
  chernovaGather(sBuff, 5, MPI_INT, rBuff, 5, MPI_INT, root, MPI_COMM_WORLD);
  if (rank == root) {
    for (int i = 0; i < 5 * numProc; i++) {
      EXPECT_TRUE(rBuff[i] <= MAX_INT && rBuff[i] >= MIN_INT);
    }
  }
}

TEST(GATHER, IS_GATHER_DOUBLE) {
  int rank, numProc;
  MPI_Comm_size(MPI_COMM_WORLD, &numProc);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int root = 0;
  if (root >= numProc) {
    root = 0;
  }
  double sBuff[5];
  double* rBuff;
  rBuff = new double[5 * numProc];
  getRandomVector(sBuff, 5);
  chernovaGather(sBuff, 5, MPI_DOUBLE, rBuff, 5, MPI_DOUBLE, root,
                   MPI_COMM_WORLD);
  if (rank == root) {
    for (int i = 0; i < 5 * numProc; i++) {
      EXPECT_TRUE(rBuff[i] <= MAX_DOUBLE && rBuff[i] >= MIN_DOUBLE);
    }
  }
}

TEST(GATHER, IS_GATHER_FLOAT) {
  int rank, numProc;
  MPI_Comm_size(MPI_COMM_WORLD, &numProc);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int root = 0;
  if (root >= numProc) {
    root = 0;
  }
  float sBuff[5];
  float* rBuff;
  rBuff = new float[5 * numProc];
  getRandomVector(sBuff, 5);
  chernovaGather(sBuff, 5, MPI_FLOAT, rBuff, 5, MPI_FLOAT, root,
                   MPI_COMM_WORLD);
  if (rank == root) {
    for (int i = 0; i < 5 * numProc; i++) {
      EXPECT_TRUE(rBuff[i] <= MAX_FLOAT && rBuff[i] >= MIN_FLOAT);
    }
  }
}

TEST(GATHER, EQAL_ROOT) {
  int rank, numProc;
  MPI_Comm_size(MPI_COMM_WORLD, &numProc);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int root = 1;
  if (root >= numProc) {
    root = 0;
  }
  int sBuff[5];
  int* rBuff;
  rBuff = new int[5 * numProc];

  getRandomVector(sBuff, 5);

  chernovaGather(sBuff, 5, MPI_INT, rBuff, 5, MPI_INT, root, MPI_COMM_WORLD);
  if (rank == root) {
    int* testBuff;
    testBuff = new int[5 * numProc];

    for (int i = 0; i < 5; i++) {
      testBuff[i] = sBuff[i];
    }

    EXPECT_TRUE(Compare(testBuff, rBuff, 5, 5 * root));
  }
}

TEST(GATHER, IS_GENERATE_ERR) {
  int rank, numProc;
  MPI_Comm_size(MPI_COMM_WORLD, &numProc);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int root = 0;
  if (root >= numProc) {
    root = 0;
  }
  int sBuff[5];
  double* rBuff;
  rBuff = new double[5 * numProc];
  getRandomVector(sBuff, 5);
  EXPECT_EQ(chernovaGather(sBuff, 5, MPI_INT, rBuff, 5, MPI_DOUBLE, root,
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
