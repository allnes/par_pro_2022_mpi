// Copyright 2022 Krainev Alexander
#include <gtest/gtest.h>

#include <gtest-mpi-listener.hpp>

#include "./matrix_ccs.h"

bool chech_eq(std::vector<double> v1, std::vector<double> v2) {
  for (size_t i = 0; i < v1.size(); i++) {
    if (abs(v1[i] - v2[i]) > 1e-9) {
      return false;
    }
  }

  return true;
}

bool test(int rows, int same, int cols) {
  bool correct = true;
  int proc_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);

  Matrix A, B;
  if (proc_rank == 0) {
    A = gen_random_matrix(rows, same);
    B = gen_random_matrix(same, cols);
  } else {
    A = gen_random_matrix(1, 1);
    B = gen_random_matrix(1, 1);
  }

  std::vector<double> par;

  double start, end;
  double par_time, seq_time;

  start = MPI_Wtime();
  par = parallel(A, B);
  if (proc_rank == 0) {
    end = MPI_Wtime();

    par_time = end - start;

    start = MPI_Wtime();
    std::vector<double> seq = sequential(A, B);
    end = MPI_Wtime();

    seq_time = end - start;

    std::cout << "Parallel: " << par_time << "\nSequential: " << seq_time
              << '\n';

    correct = chech_eq(par, seq);
  }

  return correct;
}

TEST(Matrix_CCS, Matrix_10x10) { ASSERT_TRUE(test(10, 10, 10)); }

TEST(Matrix_CCS, Matrix_100x100) { ASSERT_TRUE(test(100, 100, 100)); }

TEST(Matrix_CCS, Matrix_407x407) { ASSERT_TRUE(test(407, 407, 407)); }

TEST(Matrix_CCS, Matrix_100x50_50x100) { ASSERT_TRUE(test(100, 50, 100)); }

TEST(Matrix_CCS, Matrix_51x100_100x51) { ASSERT_TRUE(test(51, 100, 51)); }

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
