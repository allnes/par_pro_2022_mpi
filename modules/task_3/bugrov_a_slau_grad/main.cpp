// Copyright 2022 Bugrov Andrey
#include <gtest/gtest.h>

#include <gtest-mpi-listener.hpp>

#include "./slau_grad.h"

#define kEps 0.001

bool is_nearby(const Vector& one, const Vector& two) {
  int n = one.size();
  int i;
  for (i = 0; i < n; i++) {
    if (abs(one[i] - two[i]) > kEps) {
      return false;
    }
  }
  return true;
}

TEST(slau_grad, positive_2_matr_equal_diag) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int n = 2;
  Vector A(n * n);
  Vector b(n);

  A[0] = 4;
  A[1] = 2;
  A[2] = 2;
  A[3] = 4;

  b[0] = 1;
  b[1] = 2;
  Vector x_seq;
  Vector x_par;
  x_par = getParSlauGrad(A, b, n);
  if (rank == 0) {
    x_seq = getSeqSlauGrad(A, b, n);
    ASSERT_EQ(is_nearby(x_seq, x_par), true);
  }
}

TEST(slau_grad, positive_2_matr) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int n = 2;
  Vector A(n * n);
  Vector b(n);

  A[0] = 4;
  A[1] = 2;
  A[2] = 2;
  A[3] = 5;

  b[0] = 1;
  b[1] = 2;
  Vector x_seq;
  Vector x_par;
  x_par = getParSlauGrad(A, b, n);
  if (rank == 0) {
    x_seq = getSeqSlauGrad(A, b, n);
    ASSERT_EQ(is_nearby(x_seq, x_par), true);
  }
}

TEST(slau_grad, positive_3_matr) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int n = 3;
  Vector A(n * n);
  Vector b(n);

  A[0] = 5;
  A[1] = 2;
  A[2] = 1;
  A[3] = 2;
  A[4] = 8;
  A[5] = 3;
  A[6] = 1;
  A[7] = 3;
  A[8] = 6;

  b[0] = 1;
  b[1] = 2;
  b[2] = 3;
  Vector x_seq;
  Vector x_par;
  x_par = getParSlauGrad(A, b, n);
  if (rank == 0) {
    x_seq = getSeqSlauGrad(A, b, n);
    ASSERT_EQ(x_seq, x_par);
    ASSERT_EQ(is_nearby(x_seq, x_par), true);
  }
}

TEST(slau_grad, not_positive_2_matr) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int n = 2;
  Vector A(n * n);
  Vector b(n);

  A[0] = 4;
  A[1] = -1;
  A[2] = -1;
  A[3] = 5;

  b[0] = 1;
  b[1] = 2;
  Vector x_seq;
  Vector x_par;
  x_par = getParSlauGrad(A, b, n);
  if (rank == 0) {
    x_seq = getSeqSlauGrad(A, b, n);
    ASSERT_EQ(is_nearby(x_seq, x_par), true);
  }
}

TEST(slau_grad, not_positive_3_matr) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int n = 3;
  Vector A(n * n);
  Vector b(n);

  A[0] = 5;
  A[1] = 2;
  A[2] = -1;
  A[3] = 2;
  A[4] = 8;
  A[5] = -3;
  A[6] = -1;
  A[7] = 3;
  A[8] = 6;

  b[0] = 1;
  b[1] = 2;
  b[2] = 3;
  Vector x_seq;
  Vector x_par;
  x_par = getParSlauGrad(A, b, n);
  if (rank == 0) {
    x_seq = getSeqSlauGrad(A, b, n);
    ASSERT_EQ(x_seq, x_par);
    ASSERT_EQ(is_nearby(x_seq, x_par), true);
  }
}

TEST(slau_grad, not_positive_4_matr) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int n = 4;
  Vector A(n * n);
  Vector b(n);

  A[0] = 6;
  A[1] = -1;
  A[2] = 3;
  A[3] = 1;
  A[4] = -1;
  A[5] = 11;
  A[6] = 0;
  A[7] = 6;
  A[8] = 3;
  A[9] = 0;
  A[10] = 8;
  A[11] = 4;
  A[12] = 1;
  A[13] = 6;
  A[14] = 4;
  A[15] = 13;

  b[0] = 1;
  b[1] = 2;
  b[2] = 3;
  b[3] = 4;
  Vector x_seq;
  Vector x_par;
  x_par = getParSlauGrad(A, b, n);
  if (rank == 0) {
    x_seq = getSeqSlauGrad(A, b, n);
    ASSERT_EQ(x_seq, x_par);
    ASSERT_EQ(is_nearby(x_seq, x_par), true);
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
