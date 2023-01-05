// Copyright 2022 Kruglikova Valeriia
#include <gtest/gtest.h>
#include "./allreducee.h"
#include <gtest-mpi-listener.hpp>

TEST(Allreduce_MPI, Test_on_Matrix_SIZEx10_INT) {
  int size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  const int cols = 10;

  int* recv = new int[cols];
  int* send = new int[cols];
  for (int i = 0; i < cols; ++i) send[i] = 1;

  Allreduce(send, recv, cols, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

  for (int i = 0; i < cols; ++i) ASSERT_EQ(recv[i], size);
}

TEST(Allreduce_MPI, Test_on_Matrix_SIZEx10_FLOAT) {
  int size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  const int cols = 10;

  float* recv = new float[cols];
  float* send = new float[cols];
  for (int i = 0; i < cols; ++i) send[i] = static_cast<float>(2.);

  Allreduce(send, recv, cols, MPI_FLOAT, MPI_SUM, MPI_COMM_WORLD);

  for (int i = 0; i < cols; ++i) ASSERT_EQ(recv[i], (float)2. * size);
}

TEST(Allreduce_MPI, Test_on_Matrix_SIZEx10_DOUBLE) {
  int size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  const int cols = 10;

  double* recv = new double[cols];
  double* send = new double[cols];
  for (int i = 0; i < cols; ++i) send[i] = 3.;

  Allreduce(send, recv, cols, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

  for (int i = 0; i < cols; ++i) ASSERT_EQ(recv[i], size * 3.);
}

TEST(Allreduce_MPI, Test_on_Matrix_SIZEx100000_INT) {
  int size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  const int cols = 100000;

  int* recv = new int[cols];
  int* send = new int[cols];
  for (int i = 0; i < cols; ++i) send[i] = 1;

  Allreduce(send, recv, cols, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

  for (int i = 0; i < cols; ++i) ASSERT_EQ(recv[i], size);
}

TEST(Allreduce_MPI, ASSERT_ANY_THROW_COUNT) {
  int size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  int* recv = new int[10];
  int* send = new int[10];
  for (int i = 0; i < 10; ++i) send[i] = 1;

  ASSERT_ANY_THROW(Allreduce(send, recv, 0, MPI_INT, MPI_SUM, MPI_COMM_WORLD));
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
