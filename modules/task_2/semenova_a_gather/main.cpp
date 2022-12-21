// Copyright 2022 Semenova Veronika
#include <gtest/gtest.h>

#include "./gather.h"

#include <gtest-mpi-listener.hpp>

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution < int > dist(0, 10);

TEST(Parallel_Operations_MPI, correct_operation_of_Gather10_INT) {
  int rank, ProcNum, root;
  MPI_Comm_rank(MPI_COMM_WORLD, & rank);
  MPI_Comm_size(MPI_COMM_WORLD, & ProcNum);
  int n = 10;
  int nP = n / ProcNum;
  int * V = new int[n];
  int * res = new int[n];
  int * buf = new int[n];

  if (rank == 0) {
    for (int i = 0; i < n; i++)
      V[i] = i;
    root = dist(gen) % ProcNum;
  }

  MPI_Bcast(& root, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Scatter(V, nP, MPI_INT, buf, nP, MPI_INT, 0, MPI_COMM_WORLD);

  Gather(buf, nP, MPI_INT, res, nP, MPI_INT, root, MPI_COMM_WORLD);

  if (rank == root) {
    for (int i = 0; i < n / ProcNum * ProcNum; i++)
      ASSERT_EQ(i, res[i]);
  }
}
TEST(Parallel_Operations_MPI, correct_operation_of_Gather25_INT) {
  int rank, ProcNum, root;
  MPI_Comm_rank(MPI_COMM_WORLD, & rank);
  MPI_Comm_size(MPI_COMM_WORLD, & ProcNum);
  int n = 25;
  int nP = n / ProcNum;
  int * V = new int[n];
  int * res = new int[n];
  int * buf = new int[n];

  if (rank == 0) {
    for (int i = 0; i < n; i++)
      V[i] = i;
    root = dist(gen) % ProcNum;
  }

  MPI_Bcast(& root, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Scatter(V, nP, MPI_INT, buf, nP, MPI_INT, 0, MPI_COMM_WORLD);

  Gather(buf, nP, MPI_INT, res, nP, MPI_INT, root, MPI_COMM_WORLD);

  if (rank == root) {
    for (int i = 0; i < n / ProcNum * ProcNum; i++)
      ASSERT_EQ(i, res[i]);
  }
}
TEST(Parallel_Operations_MPI, correct_operation_of_Gather_with_Random10_INT) {
  int rank, ProcNum, root;
  double time1, time2;
  MPI_Comm_rank(MPI_COMM_WORLD, & rank);
  MPI_Comm_size(MPI_COMM_WORLD, & ProcNum);
  int n = 10;
  int nP = n / ProcNum;
  int * V = new int[n];
  int * buf = new int[n];
  int * res1 = new int[n];
  int * res2 = new int[n];

  if (rank == 0) {
    randVec(V, n);
    root = dist(gen) % ProcNum;
  }

  MPI_Bcast(& root, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Scatter(V, nP, MPI_INT, buf, nP, MPI_INT, 0, MPI_COMM_WORLD);

  MPI_Gather(buf, nP, MPI_INT, res2, nP, MPI_INT, root, MPI_COMM_WORLD);
  Gather(buf, nP, MPI_INT, res1, nP, MPI_INT, root, MPI_COMM_WORLD);

  if (rank == root) {
    for (int i = 0; i < n; i++)
      ASSERT_EQ(res1[i], res2[i]);
  }
}
TEST(Parallel_Operations_MPI, correct_operation_of_Gather_with_time10_INT) {
  int rank, ProcNum, root;
  double time1, time2;
  MPI_Comm_rank(MPI_COMM_WORLD, & rank);
  MPI_Comm_size(MPI_COMM_WORLD, & ProcNum);
  int n = 10;
  int nP = n / ProcNum;
  int * V = new int[n];
  int * res1 = new int[n];
  int * res2 = new int[n];
  int * buf = new int[n];

  if (rank == 0) {
    randVec(V, n);
    root = dist(gen) % ProcNum;
  }

  MPI_Bcast(& root, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Scatter(V, nP, MPI_INT, buf, nP, MPI_INT, 0, MPI_COMM_WORLD);

  if (rank == root) time1 = MPI_Wtime();
  MPI_Gather(buf, nP, MPI_INT, res2, nP, MPI_INT, root, MPI_COMM_WORLD);

  if (rank == root) {
    time1 = MPI_Wtime() - time1;
    time2 = MPI_Wtime();
  }

  Gather(buf, nP, MPI_INT, res1, nP, MPI_INT, root, MPI_COMM_WORLD);

  if (rank == root) {
    time2 = MPI_Wtime() - time2;
    ASSERT_LT(abs(time2 - time1), 1);
  }
}
TEST(Parallel_Operations_MPI, correct_operation_of_Gather_with_time25_INT) {
  int rank, ProcNum, root;
  double time1, time2;
  MPI_Comm_rank(MPI_COMM_WORLD, & rank);
  MPI_Comm_size(MPI_COMM_WORLD, & ProcNum);
  int n = 25;
  int nP = n / ProcNum;
  int * V = new int[n];
  int * res1 = new int[n];
  int * res2 = new int[n];
  int * buf = new int[n];

  if (rank == 0) {
    randVec(V, n);
    root = dist(gen) % ProcNum;
  }

  MPI_Bcast(& root, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Scatter(V, nP, MPI_INT, buf, nP, MPI_INT, 0, MPI_COMM_WORLD);

  if (rank == root) time1 = MPI_Wtime();
  MPI_Gather(buf, nP, MPI_INT, res2, nP, MPI_INT, root, MPI_COMM_WORLD);

  if (rank == root) {
    time1 = MPI_Wtime() - time1;
    time2 = MPI_Wtime();
  }

  Gather(buf, nP, MPI_INT, res1, nP, MPI_INT, root, MPI_COMM_WORLD);

  if (rank == root) {
    time2 = MPI_Wtime() - time2;
    ASSERT_LT(abs(time2 - time1), 1);
  }
}
TEST(Parallel_Operations_MPI, correct_operation_of_Gather10_DOUBLE) {
  int rank, ProcNum, root;
  MPI_Comm_rank(MPI_COMM_WORLD, & rank);
  MPI_Comm_size(MPI_COMM_WORLD, & ProcNum);
  int n = 10;
  int nP = n / ProcNum;
  double * V = new double[n];
  double * res = new double[n];
  double * buf = new double[n];

  if (rank == 0) {
    for (int i = 0; i < n; i++)
      V[i] = i;
    root = dist(gen) % ProcNum;
  }

  MPI_Bcast(& root, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Scatter(V, nP, MPI_DOUBLE, buf, nP, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  Gather(buf, nP, MPI_DOUBLE, res, nP, MPI_DOUBLE, root, MPI_COMM_WORLD);

  if (rank == root) {
    for (int i = 0; i < n / ProcNum * ProcNum; i++)
      ASSERT_EQ(i, res[i]);
  }
}
TEST(Parallel_Operations_MPI, correct_operation_of_Gather25_DOUBLE) {
  int rank, ProcNum, root;
  MPI_Comm_rank(MPI_COMM_WORLD, & rank);
  MPI_Comm_size(MPI_COMM_WORLD, & ProcNum);
  int n = 25;
  int nP = n / ProcNum;
  double * V = new double[n];
  double * res = new double[n];
  double * buf = new double[n];

  if (rank == 0) {
    for (int i = 0; i < n; i++)
      V[i] = i;
    root = dist(gen) % ProcNum;
  }

  MPI_Bcast(& root, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Scatter(V, nP, MPI_DOUBLE, buf, nP, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  Gather(buf, nP, MPI_DOUBLE, res, nP, MPI_DOUBLE, root, MPI_COMM_WORLD);

  if (rank == root) {
    for (int i = 0; i < n / ProcNum * ProcNum; i++)
      ASSERT_EQ(i, res[i]);
  }
}
TEST(Parallel_Operations_MPI, correct_operation_of_Gather_with_Random10_DOUBLE) {
  int rank, ProcNum, root;
  double time1, time2;
  MPI_Comm_rank(MPI_COMM_WORLD, & rank);
  MPI_Comm_size(MPI_COMM_WORLD, & ProcNum);
  int n = 10;
  int nP = n / ProcNum;
  double * V = new double[n];
  double * res1 = new double[n];
  double * res2 = new double[n];
  double * buf = new double[n];

  if (rank == 0) {
    randVec(V, n);
    root = dist(gen) % ProcNum;
  }

  MPI_Bcast(& root, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Scatter(V, nP, MPI_DOUBLE, buf, nP, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  MPI_Gather(buf, nP, MPI_DOUBLE, res2, nP, MPI_DOUBLE, root, MPI_COMM_WORLD);
  Gather(buf, nP, MPI_DOUBLE, res1, nP, MPI_DOUBLE, root, MPI_COMM_WORLD);

  if (rank == root) {
    for (int i = 0; i < n; i++)
      ASSERT_EQ(res1[i], res2[i]);
  }
}
TEST(Parallel_Operations_MPI, correct_operation_of_Gather_with_time10_DOUBLE) {
  int rank, ProcNum, root;
  double time1, time2;
  MPI_Comm_rank(MPI_COMM_WORLD, & rank);
  MPI_Comm_size(MPI_COMM_WORLD, & ProcNum);
  int n = 10;
  int nP = n / ProcNum;
  double * V = new double[n];
  double * res1 = new double[n];
  double * res2 = new double[n];
  double * buf = new double[n];

  if (rank == 0) {
    randVec(V, n);
    root = dist(gen) % ProcNum;
  }

  MPI_Bcast(& root, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Scatter(V, nP, MPI_DOUBLE, buf, nP, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  if (rank == root) time1 = MPI_Wtime();
  MPI_Gather(buf, nP, MPI_DOUBLE, res2, nP, MPI_DOUBLE, root, MPI_COMM_WORLD);

  if (rank == root) {
    time1 = MPI_Wtime() - time1;
    time2 = MPI_Wtime();
  }

  Gather(buf, nP, MPI_DOUBLE, res1, nP, MPI_DOUBLE, root, MPI_COMM_WORLD);

  if (rank == root) {
    time2 = MPI_Wtime() - time2;
    ASSERT_LT(abs(time2 - time1), 1);
  }
}
TEST(Parallel_Operations_MPI, correct_operation_of_Gather_with_time25_DOUBLE) {
  int rank, ProcNum, root;
  double time1, time2;
  MPI_Comm_rank(MPI_COMM_WORLD, & rank);
  MPI_Comm_size(MPI_COMM_WORLD, & ProcNum);
  int n = 25;
  int nP = n / ProcNum;
  double * V = new double[n];
  double * res1 = new double[n];
  double * res2 = new double[n];
  double * buf = new double[n];

  if (rank == 0) {
    randVec(V, n);
    root = dist(gen) % ProcNum;
  }

  MPI_Bcast(& root, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Scatter(V, nP, MPI_DOUBLE, buf, nP, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  if (rank == root) time1 = MPI_Wtime();
  MPI_Gather(buf, nP, MPI_DOUBLE, res2, nP, MPI_DOUBLE, root, MPI_COMM_WORLD);

  if (rank == root) {
    time1 = MPI_Wtime() - time1;
    time2 = MPI_Wtime();
  }

  Gather(buf, nP, MPI_DOUBLE, res1, nP, MPI_DOUBLE, root, MPI_COMM_WORLD);

  if (rank == root) {
    time2 = MPI_Wtime() - time2;
    ASSERT_LT(abs(time2 - time1), 1);
  }
}
TEST(Parallel_Operations_MPI, correct_operation_of_Gather10_FLOAT) {
  int rank, ProcNum, root;
  MPI_Comm_rank(MPI_COMM_WORLD, & rank);
  MPI_Comm_size(MPI_COMM_WORLD, & ProcNum);
  int n = 10;
  int nP = n / ProcNum;
  float * V = new float[n];
  float * res = new float[n];
  float * buf = new float[n];

  if (rank == 0) {
    for (int i = 0; i < n; i++)
      V[i] = i;
    root = dist(gen) % ProcNum;
  }

  MPI_Bcast(& root, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
  MPI_Scatter(V, nP, MPI_FLOAT, buf, nP, MPI_FLOAT, 0, MPI_COMM_WORLD);

  Gather(buf, nP, MPI_FLOAT, res, nP, MPI_FLOAT, root, MPI_COMM_WORLD);

  if (rank == root) {
    for (int i = 0; i < n / ProcNum * ProcNum; i++)
      ASSERT_EQ(i, res[i]);
  }
}
TEST(Parallel_Operations_MPI, correct_operation_of_Gather25_FLOAT) {
  int rank, ProcNum, root;
  MPI_Comm_rank(MPI_COMM_WORLD, & rank);
  MPI_Comm_size(MPI_COMM_WORLD, & ProcNum);
  int n = 25;
  int nP = n / ProcNum;
  float * V = new float[n];
  float * res = new float[n];
  float * buf = new float[n];

  if (rank == 0) {
    for (int i = 0; i < n; i++)
      V[i] = i;
    root = dist(gen) % ProcNum;
  }

  MPI_Bcast(& root, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
  MPI_Scatter(V, nP, MPI_FLOAT, buf, nP, MPI_FLOAT, 0, MPI_COMM_WORLD);

  Gather(buf, nP, MPI_FLOAT, res, nP, MPI_FLOAT, root, MPI_COMM_WORLD);

  if (rank == root) {
    for (int i = 0; i < n / ProcNum * ProcNum; i++)
      ASSERT_EQ(i, res[i]);
  }
}
TEST(Parallel_Operations_MPI, correct_operation_of_Gather_with_Random10_FLOAT) {
  int rank, ProcNum, root;
  double time1, time2;
  MPI_Comm_rank(MPI_COMM_WORLD, & rank);
  MPI_Comm_size(MPI_COMM_WORLD, & ProcNum);
  int n = 10;
  int nP = n / ProcNum;
  float * V = new float[n];
  float * res1 = new float[n];
  float * res2 = new float[n];
  float * buf = new float[n];

  if (rank == 0) {
    randVec(V, n);
    root = dist(gen) % ProcNum;
  }

  MPI_Bcast(& root, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
  MPI_Scatter(V, nP, MPI_FLOAT, buf, nP, MPI_FLOAT, 0, MPI_COMM_WORLD);

  MPI_Gather(buf, nP, MPI_FLOAT, res2, nP, MPI_FLOAT, root, MPI_COMM_WORLD);
  Gather(buf, nP, MPI_FLOAT, res1, nP, MPI_FLOAT, root, MPI_COMM_WORLD);

  if (rank == root) {
    for (int i = 0; i < n; i++)
      ASSERT_EQ(res1[i], res2[i]);
  }
}
TEST(Parallel_Operations_MPI, correct_operation_of_Gather_with_time10_FLOAT) {
  int rank, ProcNum, root;
  double time1, time2;
  MPI_Comm_rank(MPI_COMM_WORLD, & rank);
  MPI_Comm_size(MPI_COMM_WORLD, & ProcNum);
  int n = 10;
  int nP = n / ProcNum;
  float * V = new float[n];
  float * res1 = new float[n];
  float * res2 = new float[n];
  float * buf = new float[n];

  if (rank == 0) {
    randVec(V, n);
    root = dist(gen) % ProcNum;
  }

  MPI_Bcast(& root, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
  MPI_Scatter(V, nP, MPI_FLOAT, buf, nP, MPI_FLOAT, 0, MPI_COMM_WORLD);

  if (rank == root) time1 = MPI_Wtime();
  MPI_Gather(buf, nP, MPI_FLOAT, res2, nP, MPI_FLOAT, root, MPI_COMM_WORLD);

  if (rank == root) {
    time1 = MPI_Wtime() - time1;
    time2 = MPI_Wtime();
  }

  Gather(buf, nP, MPI_FLOAT, res1, nP, MPI_FLOAT, root, MPI_COMM_WORLD);

  if (rank == root) {
    time2 = MPI_Wtime() - time2;
    ASSERT_LT(abs(time2 - time1), 1);
  }
}
TEST(Parallel_Operations_MPI, correct_operation_of_Gather_with_time25_FLOAT) {
  int rank, ProcNum, root;
  double time1, time2;
  MPI_Comm_rank(MPI_COMM_WORLD, & rank);
  MPI_Comm_size(MPI_COMM_WORLD, & ProcNum);
  int n = 25;
  int nP = n / ProcNum;
  float * V = new float[n];
  float * res1 = new float[n];
  float * res2 = new float[n];
  float * buf = new float[n];

  if (rank == 0) {
    randVec(V, n);
    root = dist(gen) % ProcNum;
  }

  MPI_Bcast(& root, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
  MPI_Scatter(V, nP, MPI_FLOAT, buf, nP, MPI_FLOAT, 0, MPI_COMM_WORLD);

  if (rank == root) time1 = MPI_Wtime();
  MPI_Gather(buf, nP, MPI_FLOAT, res2, nP, MPI_FLOAT, root, MPI_COMM_WORLD);

  if (rank == root) {
    time1 = MPI_Wtime() - time1;
    time2 = MPI_Wtime();
  }

  Gather(buf, nP, MPI_FLOAT, res1, nP, MPI_FLOAT, root, MPI_COMM_WORLD);

  if (rank == root) {
    time2 = MPI_Wtime() - time2;
    ASSERT_LT(abs(time2 - time1), 1);
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
