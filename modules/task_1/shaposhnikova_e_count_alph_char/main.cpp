// Copyright 2022 Shaposhnikova Ekaterina
#include <gtest/gtest.h>
#include "./count_alph_char.h"
#include <gtest-mpi-listener.hpp>

TEST(Parallel_Operations_MPI, test1) {
  std::string test = "";

  int rank, size;

  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int gl_count = parallelCharCount(test);

  if (rank == 0) {
    int loc_count = sequentialCharCount(test);

    ASSERT_EQ(loc_count, gl_count);
  }

}

TEST(Parallel_Operations_MPI, test2) {
  std::string test = "test 2 ";

  int rank, size;

  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int gl_count = parallelCharCount(test);

  if (rank == 0) {
    int loc_count = sequentialCharCount(test);

    ASSERT_EQ(loc_count, gl_count);
  }
}

TEST(Parallel_Operations_MPI, test3) {
  std::string test = "parallel programming";

  int rank, size;

  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int gl_count = parallelCharCount(test);

  if (rank == 0) {
    int loc_count = sequentialCharCount(test);

    ASSERT_EQ(loc_count, gl_count);
  }
}

TEST(Parallel_Operations_MPI, test4) {
  std::string test =
    "Rosemary Fell was not exactly beautiful.She was young, brilliant,"
    "extremely modern, well dressedand amazingly well read in the newest"
    "of the new books.Rosemary had been married two years, and her husband"
    "was very fond of her.They were rich, really rich, not just "
    "well - off, so if Rosemary wanted to shop, she would go to Paris as"
    "you and I would go to Bond Street.";

  int rank, size;

  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int gl_count = parallelCharCount(test);

  if (rank == 0) {
    int loc_count = sequentialCharCount(test);

    ASSERT_EQ(loc_count, gl_count);
  }
}

TEST(Parallel_Operations_MPI, test5) {
  std::string test = "..";

  int rank, size;

  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int gl_count = parallelCharCount(test);

  if (rank == 0) {
    int loc_count = sequentialCharCount(test);

    ASSERT_EQ(loc_count, gl_count);
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
