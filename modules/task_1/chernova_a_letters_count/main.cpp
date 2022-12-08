// Copyright 2022 Anna Chernova
#include <gtest/gtest.h>
#include <mpi.h>

#include <gtest-mpi-listener.hpp>
#include <vector>

#include "../chernova_a_letters_count/letters_count.h"

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

TEST(LETTERS_COUNT, IS_ONE_THREAD_VALID) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    int single = countLetters("string12_");
    EXPECT_EQ(single, 6);
  }
}

TEST(LETTERS_COUNT, IS_EQUAL) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  string temp = "";
  int single, multi;
  if (rank == 0) {
    temp = randomStr(511);
    single = countLetters(temp);
  }
  multi = countLettersMPI(temp, 511);
  if (rank == 0) {
    EXPECT_EQ(single, multi);
  }
}

TEST(LETTERS_COUNT, NO_LETTERS) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  string temp = "";
  int multi;
  if (rank == 0) {
    for (int i = 0; i < 9; i++) {
      temp += i;
    }
  }
  multi = countLettersMPI(temp, 9);
  if (rank == 0) {
    EXPECT_EQ(0, multi);
  }
}

TEST(LETTERS_COUNT, SPACE_STRING) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  string temp = "";
  int multi;
  if (rank == 0) {
    for (int i = 0; i < 500; i++) {
      temp += ' ';
    }
  }
  multi = countLettersMPI(temp, 500);
  if (rank == 0) {
    EXPECT_EQ(0, multi);
  }
}

TEST(LETTERS_COUNT, EMPTY_STRING) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  string temp = "";
  int multi;
  multi = countLettersMPI(temp, 0);
  if (rank == 0) {
    EXPECT_EQ(0, multi);
  }
}
