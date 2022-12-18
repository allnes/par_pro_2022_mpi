// Copyright 2022 Kolesnikov Denis
#include <gtest/gtest.h>
#include <gtest-mpi-listener.hpp>

#include "./matrix_mltpl_hor.h"





TEST(MATRIX_MLTPL_TEST, multiplication_zero_matrices) {
    int rank;
    int num;
    MPI_Comm_size(MPI_COMM_WORLD, &num);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (log2(num) == static_cast<int>(log2(num))) {
      vector<int> a;
      vector<int> b;
      vector<int> seq_c;

      if (rank == 0) {
        seq_c = MatrixMtlplSeq(
            a, 0, 0,
            b, 0, 0);
        ASSERT_EQ(vector<int>(), seq_c);
      }
    }
}
TEST(MATRIX_MLTPL_TEST, multiplication_8_8_matrices) {
  int rank;
  int num;
  MPI_Comm_size(MPI_COMM_WORLD, &num);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (log2(num) == static_cast<int>(log2(num))) {
    int a_height = 8;
    int a_width = 8;
    int b_height = 8;
    int b_width = 8;
    vector<int> a = GenRndMtrx(a_height, a_width);
    vector<int> b = GenRndMtrx(b_height, b_width);
    vector<int> seq_c;

    if (rank == 0) {
      seq_c = MatrixMtlplSeq(
          a, a_height, a_width,
          b, b_height, b_width);
    }
    vector<int> c = MatrixMtlplPrl(
        a, a_height, a_width,
        b, b_height, b_width);
    if (rank == 0) {
        ASSERT_EQ(c, seq_c);
    }
  }
}
TEST(MATRIX_MLTPL_TEST, multiplication_64_64_matrices) {
  int rank;
  int num;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &num);
  if (log2(num) == static_cast<int>(log2(num))) {
    int a_height = 64;
    int a_width = 64;
    int b_height = 64;
    int b_width = 64;
    vector<int> a = GenRndMtrx(a_height, a_width);
    vector<int> b = GenRndMtrx(b_height, b_width);
    vector<int> seq_c;
    if (rank == 0) {
      seq_c = MatrixMtlplSeq(
          a, a_height, a_width,
          b, b_height, b_width);
    }
    vector<int> c = MatrixMtlplPrl(
        a, a_height, a_width,
        b, b_height, b_width);
    if (rank == 0) {
        ASSERT_EQ(c, seq_c);
    }
  }
}
TEST(MATRIX_MLTPL_TEST, multiplication_128_128_matrices) {
  int rank;
  int num;
  MPI_Comm_size(MPI_COMM_WORLD, &num);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (log2(num) == static_cast<int>(log2(num))) {
    int a_height = 128;
    int a_width = 128;
    int b_height = 128;
    int b_width = 128;
    vector<int> a = GenRndMtrx(a_height, a_width);
    vector<int> b = GenRndMtrx(b_height, b_width);
    vector<int> seq_c;
    if (rank == 0) {
      seq_c = MatrixMtlplSeq(
          a, a_height, a_width,
          b, b_height, b_width);
    }
    vector<int> c = MatrixMtlplPrl(
        a, a_height, a_width,
        b, b_height, b_width);
    if (rank == 0) {
        ASSERT_EQ(c, seq_c);
    }
  }
}
TEST(MATRIX_MLTPL_TEST, multiplication_256_256_matrices) {
  int num;
  MPI_Comm_size(MPI_COMM_WORLD, &num);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (log2(num) == static_cast<int>(log2(num))) {
    int a_height = 256;
    int a_width = 256;
    int b_height = 256;
    int b_width = 256;
    vector<int> a = GenRndMtrx(a_height, a_width);
    vector<int> b = GenRndMtrx(b_height, b_width);
    vector<int> seq_c;
    if (rank == 0) {
      seq_c = MatrixMtlplSeq(
          a, a_height, a_width,
          b, b_height, b_width);
    }
    vector<int> c = MatrixMtlplPrl(
        a, a_height, a_width,
        b, b_height, b_width);
    if (rank == 0) {
      ASSERT_EQ(c, seq_c);
    }
  }
}
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  MPI_Init(&argc, &argv);

  ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
  ::testing::TestEventListeners &listeners =
      ::testing::UnitTest::GetInstance()->listeners();

  listeners.Release(listeners.default_result_printer());
  listeners.Release(listeners.default_xml_generator());

  listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
  return RUN_ALL_TESTS();
}
