// Copyright 2022 Pashina Alina

#include <gtest/gtest.h>
#include <vector>
#include "./pashina_a_lex_compare_strings.h"  ////
#include <gtest-mpi-listener.hpp>

TEST(generateString_MPI_Checks, get_rand_str) {
  ASSERT_NO_THROW(generateString(11));
}

TEST(compareCharVectors_MPI_Checks, can_define_equil_strings) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::string firststr = "hhhhh";
  std::string secststr = "hhhhh";
  int myres = compareStrings(firststr, secststr);
  int res = 100;
  if (rank == 0) ASSERT_EQ(myres, res);
}

TEST(doParallel_MPI_Checks, doParallel_equally) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::string firststr = "ddddd";
  std::string secststr = "ddddd";

  int myres =
      doParallel(firststr, secststr, firststr.length(), secststr.length());

  if (rank == 0) {
    int wanted_result = 100;
    ASSERT_EQ(myres, wanted_result);
  }
}

TEST(doParallel_MPI_Checks, doParallel_find_small) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::string firststr = "fffffffffffffffff";
  std::string secststr = "fgfffffffffffffff";

  int myres =
      doParallel(firststr, secststr, firststr.length(), secststr.length());
  if (rank == 0) {
    int wanted_result = 99;
    ASSERT_EQ(myres, wanted_result);
  }
}

TEST(doParallel_MPI_Checks, doParallel_for_genstr) {
  int myrank;
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
  std::string first_str, sec_str;
  const size_t length = 100;

  if (myrank == 0) {
    first_str = generateString(length);
  }
  if (myrank == 0) {
    sec_str = generateString(length);
  }
  int parallelResult = doParallel(first_str, sec_str, length, length);

  if (myrank == 0) {
    int compRes = compareStrings(first_str, sec_str);
    ASSERT_EQ(parallelResult, compRes);
  }
}

TEST(doParallel_MPI_Checks, doParallel_find_big) {
  int myrank;
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
  std::vector<char>::size_type len = 13;
  std::string first_str = "fxffffffffffffffffk";
  std::string sec_str = "ffffffffffffffffffk";

  int myres =
      doParallel(first_str, sec_str, first_str.length(), sec_str.length());

  if (myrank == 0) {
    int compRes = compareStrings(first_str, sec_str);
    int wanted_result = 101;
    ASSERT_EQ(myres, wanted_result);
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

