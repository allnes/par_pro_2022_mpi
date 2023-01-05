// Copyright 2022 Shaposhnikova Ekaterina
#include <gtest/gtest.h>
#include <string>
#include <random>
#include <iostream>
#include "./reduceall.h"
#include <gtest-mpi-listener.hpp>



TEST(reduceAll_MPI, Test_INT) {
  int size;
  int rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const int cols = 10;

  int* recv = new int[cols];
  int* send = new int[cols];


  for (int i = 0; i < cols; ++i) send[i] = 1;
  double s1 = 0;
  double e1 = 0;
  double s2 = 0;
  double e2 = 0;


  s1 = MPI_Wtime();
  MPI_Allreduce(send, recv, cols, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
  e1 = MPI_Wtime();


  s2 = MPI_Wtime();
  reduceAll(send, recv, cols, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
  e2 = MPI_Wtime();
  if (rank == 0) {
    std::cout << "MPI_AllReduce Time = " << e1 - s1 << std::endl;
    std::cout << "Own AllReduce Time = " << e2 - s2 << std::endl;
  }

  for (int i = 0; i < cols; ++i) ASSERT_EQ(recv[i], size);
}

TEST(reduceAll_MPI, Test_FLOAT) {
  int size;
  int rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const int cols = 10;

  float* recv = new float[cols];
  float* send = new float[cols];


  for (int i = 0; i < cols; ++i) send[i] = static_cast<float>(2.);;
  double s1 = 0;
  double e1 = 0;
  double s2 = 0;
  double e2 = 0;


  s1 = MPI_Wtime();
  MPI_Allreduce(send, recv, cols, MPI_FLOAT, MPI_SUM, MPI_COMM_WORLD);
  e1 = MPI_Wtime();


  s2 = MPI_Wtime();
  reduceAll(send, recv, cols, MPI_FLOAT, MPI_SUM, MPI_COMM_WORLD);
  e2 = MPI_Wtime();


  if (rank == 0) {
    std::cout << "MPI_AllReduce Time = " << e1 - s1 << std::endl;
    std::cout << "Own AllReduce Time = " << e2 - s2 << std::endl;
  }

  for (int i = 0; i < cols; ++i) ASSERT_EQ(recv[i], (float)2. * size);
}

TEST(reduceAll_MPI, Test_DOUBLE) {
  int size;
  int rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const int cols = 10;

  double* recv = new double[cols];
  double* send = new double[cols];


  for (int i = 0; i < cols; ++i) send[i] = 3.;
  double s1 = 0;
  double e1 = 0;
  double s2 = 0;
  double e2 = 0;


  s1 = MPI_Wtime();
  MPI_Allreduce(send, recv, cols, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
  e1 = MPI_Wtime();


  s2 = MPI_Wtime();
  reduceAll(send, recv, cols, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
  e2 = MPI_Wtime();


  if (rank == 0) {
    std::cout << "MPI_AllReduce Time = " << e1 - s1 << std::endl;
    std::cout << "Own AllReduce Time = " << e2 - s2 << std::endl;
  }

  for (int i = 0; i < cols; ++i) ASSERT_EQ(recv[i], size * 3.);
}

TEST(reduceAll_MPI, Test_INT_100) {
  int size;
  int rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const int cols = 100;

  int* recv = new int[cols];
  int* send = new int[cols];


  for (int i = 0; i < cols; ++i) send[i] = 1;
  double s1 = 0;
  double e1 = 0;
  double s2 = 0;
  double e2 = 0;


  s1 = MPI_Wtime();
  MPI_Allreduce(send, recv, cols, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
  e1 = MPI_Wtime();


  s2 = MPI_Wtime();
  reduceAll(send, recv, cols, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
  e2 = MPI_Wtime();


  if (rank == 0) {
    std::cout << "MPI_AllReduce Time = " << e1 - s1 << std::endl;
    std::cout << "Own AllReduce Time = " << e2 - s2 << std::endl;
  }

  for (int i = 0; i < cols; ++i) ASSERT_EQ(recv[i], size);
}

TEST(reduceAll_MPI, Test_FLOAT_100) {
  int size;
  int rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const int cols = 100;

  float* recv = new float[cols];
  float* send = new float[cols];


  for (int i = 0; i < cols; ++i) send[i] = static_cast<float>(2.);;
  double s1 = 0;
  double e1 = 0;
  double s2 = 0;
  double e2 = 0;


  s1 = MPI_Wtime();
  MPI_Allreduce(send, recv, cols, MPI_FLOAT, MPI_SUM, MPI_COMM_WORLD);
  e1 = MPI_Wtime();


  s2 = MPI_Wtime();
  reduceAll(send, recv, cols, MPI_FLOAT, MPI_SUM, MPI_COMM_WORLD);
  e2 = MPI_Wtime();


  if (rank == 0) {
    std::cout << "MPI_AllReduce Time = " << e1 - s1 << std::endl;
    std::cout << "Own AllReduce Time = " << e2 - s2 << std::endl;
  }

  for (int i = 0; i < cols; ++i) ASSERT_EQ(recv[i], (float)2. * size);
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
