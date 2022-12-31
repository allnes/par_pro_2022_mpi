// Copyright 2022 Shokurov Daniil
#include <gtest/gtest.h>
#include <vector>
#include "./check_order.h"
#include <gtest-mpi-listener.hpp>

TEST(check_order_MPI, test_equal_size_1) {
    int rank;
    int ProcNum = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);

    std::string str1;
    std::string str2;
    int ans;
    if (rank == 0) {
        str1 = "11111113120";
        str2 = "11111110000";
    }

    ans = getOrder(str1, str2);
    if (rank == 0) EXPECT_EQ(ans, 1);
}

TEST(check_order_MPI, test_equal_size_2) {
    int rank;
    int ProcNum = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);

    std::string str1;
    std::string str2;
    int ans;
    if (rank == 0) {
        str1 = "11131113120";
        str2 = "11111110000";
    }

    ans = getOrder(str1, str2);
    if (rank == 0) EXPECT_EQ(ans, 1);
}

TEST(check_order_MPI, test_different_size_1) {
  int rank;
  int ProcNum = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);

  std::string str1;
  std::string str2;
  int ans;
  if (rank == 0) {
    str1 = "11";
    str2 = "212220000";
  }

  ans = getOrder(str1, str2);
  if (rank == 0) EXPECT_EQ(ans, -1);
}

TEST(check_order_MPI, test_different_size_2) {
  int rank;
  int ProcNum = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);

  std::string str1;
  std::string str2;
  int ans;
  if (rank == 0) {
    str1 = "dsg9893dlk24d111111";
    str2 = "dsg9893dlk24d";
  }

  ans = getOrder(str1, str2);
  if (rank == 0) EXPECT_EQ(ans, 1);
}

TEST(check_order_MPI, test_equal_string) {
  int rank;
  int ProcNum = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);

  std::string str1;
  std::string str2;
  int ans;
  if (rank == 0) {
    str1 = "dsg98934dlk24d";
    str2 = "dsg98934dlk24d";
  }

  ans = getOrder(str1, str2);
  if (rank == 0) EXPECT_EQ(ans, 0);
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
