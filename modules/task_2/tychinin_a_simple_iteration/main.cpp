// Copyright 2022 Alexey Tychinin


#include <gtest/gtest.h>
#include <numeric>
#include <vector>
#include "../../../modules/task_2/tychinin_a_simple_iteration/simple_iteration.h"
#include <gtest-mpi-listener.hpp>

TEST(Simple_Iteration, Norm) {
  std::vector<double> test = { 0, -7, 4, 6 };
  EXPECT_EQ(task2::Norm(test), 7);
}

TEST(Simple_Iteration, NonEqualDims) {
  std::vector<double> A{ 10, 1, 1, 2, 10, 1, 2, 2 };
  std::vector<double> b{ 12, 13, 14 };
  ASSERT_ANY_THROW(task2::simpleIteration(A, b, 1e-5));
}

TEST(Simple_Iteration, SimpleIterationSeq1) {
  std::vector<double> A{ 10, 1, 2, 3, 1, 10, 2, 3, 1, 0, 10, 2, 2, 0, 0, 10 };
  std::vector<double> b{ 12, 3, 11, 2 };
  std::vector<double> x(4);
  std::vector<double> tmp{ 1, 0, 1, 0 };
  x = task2::simpleIterationSeq(A, b, 1e-12);
  ASSERT_NEAR(task2::Norm(x), task2::Norm(tmp), 1e-12);
}

TEST(Simple_Iteration, SimpleIterationSeq2) {
  std::vector<double> A{ 3, 1, 1, 2, 4, 1, 5, 7, 10 };
  std::vector<double> b{ 10, 11, 47 };
  std::vector<double> x(3);
  std::vector<double> tmp{ 2, 1, 3 };
  x = task2::simpleIterationSeq(A, b, 1e-7);
  ASSERT_NEAR(task2::Norm(x), task2::Norm(tmp), 1e-7);
}

TEST(Simple_Iteration, SimpleIterationSeq3) {
  std::vector<double> A{ 4, 1, 1, 1, 1, 4, 1, 1, 1, 1, 4, 1, 1, 1, 1, 4 };
  std::vector<double> b{ 10, 13, 16, 10 };
  std::vector<double> x(4);
  std::vector<double> tmp{ 1, 2, 3, 1 };
  x = task2::simpleIterationSeq(A, b, 1e-7);
  ASSERT_NEAR(task2::Norm(x), task2::Norm(tmp), 1e-7);
}

TEST(Simple_Iteration, SimpleIteration1) {
  std::vector<double> A{ 10, 1, 1, 2, 10, 1, 2, 2, 10 };
  std::vector<double> b{ 12, 13, 14 };
  std::vector<double> x(3);
  std::vector<double> tmp{ 1, 1, 1 };
  x = task2::simpleIteration(A, b, 1e-12);
  ASSERT_NEAR(task2::Norm(x), task2::Norm(tmp), 1e-12);
}

TEST(Simple_Iteration, SimpleIteration2) {
  std::vector<double> A{ 3, 1, 1, 2, 4, 1, 5, 7, 10 };
  std::vector<double> b{ 10, 11, 47 };
  std::vector<double> x(3);
  std::vector<double> tmp{ 2, 1, 3 };
  x = task2::simpleIterationSeq(A, b, 1e-7);
  ASSERT_NEAR(task2::Norm(x), task2::Norm(tmp), 1e-7);
}

TEST(Simple_Iteration, SimpleIteration3) {
  std::vector<double> A{ 4, 1, 1, 1, 1, 4, 1, 1, 1, 1, 4, 1, 1, 1, 1, 4 };
  std::vector<double> b{ 10, 13, 16, 10 };
  std::vector<double> x(4);
  std::vector<double> tmp{ 1, 2, 3, 1 };
  x = task2::simpleIteration(A, b, 1e-7);
  ASSERT_NEAR(task2::Norm(x), task2::Norm(tmp), 1e-7);
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
