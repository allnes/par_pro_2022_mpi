// Copyright 2022 Kruglikova Valeriia
#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include <array>
#include "../../../modules/task_3/kruglikova_v_horizontal_gauss/horizontal_gauss.h"
#include <gtest-mpi-listener.hpp>

TEST(Horizontal_Gauss_MPI, Test_Image_9_rows_9_cols) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  constexpr int rows{ 9 };
  constexpr int cols{ 9 };

  std::vector<unsigned char> src = getRandomImage(cols, rows);;
  std::vector<unsigned char> resPar;
  std::vector<unsigned char> resSeq;

  resPar = filterImageParallel(src, cols, rows);

  if (rank == 0) {
    resSeq = filterImageSequential(src, cols, rows);
#ifdef DEBUG
    std::cout << "Parallel result:" << std::endl;
    for (int i = 0; i < cols * rows; ++i) {
      std::cout << (unsigned int)resPar[i] << " ";
    }
    std::cout << std::endl;
    std::cout << "Sequential result:" << std::endl;
    for (int i = 0; i < cols* rows; ++i) {
      std::cout << (unsigned int)resSeq[i] << " ";
    }
#endif  // DEBUG
    ASSERT_EQ(resPar, resSeq);
  }
}

TEST(Horizontal_Gauss_MPI, Test_Image_27rows_27_cols) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  constexpr int rows{ 27 };
  constexpr int cols{ 27 };

  std::vector<unsigned char> src = getRandomImage(cols, rows);;
  std::vector<unsigned char> resPar;
  std::vector<unsigned char> resSeq;

  resPar = filterImageParallel(src, cols, rows);

  if (rank == 0) {
    resSeq = filterImageSequential(src, cols, rows);
#ifdef DEBUG
    std::cout << "Parallel result:" << std::endl;
    for (int i = 0; i < cols * rows; ++i) {
      std::cout << (unsigned int)resPar[i] << " ";
    }
    std::cout << std::endl;
    std::cout << "Sequential result:" << std::endl;
    for (int i = 0; i < cols* rows; ++i) {
      std::cout << (unsigned int)resSeq[i] << " ";
    }
#endif  // DEBUG

    ASSERT_EQ(resPar, resSeq);
  }
}

TEST(Horizontal_Gauss_MPI, Test_Image_10_rows_5_cols) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  constexpr int rows{ 10 };
  constexpr int cols{ 5 };

  std::vector<unsigned char> src = getRandomImage(cols, rows);;
  std::vector<unsigned char> resPar;
  std::vector<unsigned char> resSeq;

  resPar = filterImageParallel(src, rows, cols);

  if (rank == 0) {
    resSeq = filterImageSequential(src, rows, cols);
#ifdef DEBUG
    std::cout << "Parallel result:" << std::endl;
    for (int i = 0; i < cols * rows; ++i) {
      std::cout << (unsigned int)resPar[i] << " ";
      if ((i + 1) % cols == 0) std::cout << std::endl;
  }
    std::cout << std::endl;
    std::cout << "Sequential result:" << std::endl;
    for (int i = 0; i < cols* rows; ++i) {
      std::cout << (unsigned int)resSeq[i] << " ";
      if ((i + 1) % cols == 0) std::cout << std::endl;
    }
#endif  // DEBUG

    ASSERT_EQ(resPar, resSeq);
  }
}

TEST(Horizontal_Gauss_MPI, Test_Image_8_rows_11_cols) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  constexpr int rows{ 8 };
  constexpr int cols{ 11 };

  std::vector<unsigned char> src = getRandomImage(cols, rows);;
  std::vector<unsigned char> resPar;
  std::vector<unsigned char> resSeq;

  resPar = filterImageParallel(src, rows, cols);

  if (rank == 0) {
    resSeq = filterImageSequential(src, rows, cols);
#ifdef DEBUG
    std::cout << "Parallel result:" << std::endl;
    for (int i = 0; i < cols * rows; ++i) {
      std::cout << (unsigned int)resPar[i] << " ";
      if ((i + 1) % cols == 0) std::cout << std::endl;
    }
    std::cout << std::endl;
    std::cout << "Sequential result:" << std::endl;
    for (int i = 0; i < cols* rows; ++i) {
      std::cout << (unsigned int)resSeq[i] << " ";
      if ((i + 1) % cols == 0) std::cout << std::endl;
    }
#endif  // DEBUG

    ASSERT_EQ(resPar, resSeq);
  }
}


TEST(Horizontal_Gauss_MPI, Test_Image_16_rows_1_cols) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  constexpr int rows{ 16 };
  constexpr int cols{ 1 };

  std::vector<unsigned char> src = getRandomImage(cols, rows);;
  std::vector<unsigned char> resPar;
  std::vector<unsigned char> resSeq;

  resPar = filterImageParallel(src, rows, cols);

  if (rank == 0) {
    resSeq = filterImageSequential(src, rows, cols);
#ifdef DEBUG
    std::cout << "Parallel result:" << std::endl;
    for (int i = 0; i < cols * rows; ++i) {
      std::cout << (unsigned int)resPar[i] << " ";
      if ((i + 1) % cols == 0) std::cout << std::endl;
    }
    std::cout << std::endl;
    std::cout << "Sequential result:" << std::endl;
    for (int i = 0; i < cols* rows; ++i) {
      std::cout << (unsigned int)resSeq[i] << " ";
      if ((i + 1) % cols == 0) std::cout << std::endl;
    }
#endif  // DEBUG

    ASSERT_EQ(resPar, resSeq);
  }
}

TEST(Horizontal_Gauss_MPI, Test_Image_1_rows_16_cols) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  constexpr int rows{ 16 };
  constexpr int cols{ 1 };

  std::vector<unsigned char> src = getRandomImage(cols, rows);;
  std::vector<unsigned char> resPar;
  std::vector<unsigned char> resSeq;

  resPar = filterImageParallel(src, rows, cols);

  if (rank == 0) {
    resSeq = filterImageSequential(src, rows, cols);
#ifdef DEBUG
    std::cout << "Parallel result:" << std::endl;
    for (int i = 0; i < cols * rows; ++i) {
      std::cout << (unsigned int)resPar[i] << " ";
      if ((i + 1) % cols == 0) std::cout << std::endl;
    }
    std::cout << std::endl;
    std::cout << "Sequential result:" << std::endl;
    for (int i = 0; i < cols* rows; ++i) {
      std::cout << (unsigned int)resSeq[i] << " ";
      if ((i + 1) % cols == 0) std::cout << std::endl;
    }
#endif  // DEBUG

    ASSERT_EQ(resPar, resSeq);
  }
}

// TEST(Horizontal_Gauss_MPI, Test_Image_500_rows_500_cols) {
// int rank;
// MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//
// constexpr int rows{ 500 };
// constexpr int cols{ 500 };
//
// std::vector<unsigned char> src  = getRandomImage(cols, rows);;
// std::vector<unsigned char> resPar;
// std::vector<unsigned char> resSeq;
//
// double parTime1 = MPI_Wtime();
// resPar = filterImageParallel(src, cols, rows);
// double parTime2 = MPI_Wtime();
//
// if (rank == 0) {
//   double seqTime1 = MPI_Wtime();
//   resSeq = filterImageSequential(src, cols, rows);
//   double seqTime2 = MPI_Wtime();
// #ifdef DEBUG
//   for (int i = 0; i < cols * rows; ++i) {
//     std::cout << (unsigned int)resPar[i] << " ";
//   }
//   std::cout << std::endl;
//   for (int i = 0; i < cols* rows; ++i) {
//     std::cout << (unsigned int)resSeq[i] << " ";
//   }
// #endif  // DEBUG
//   std::cout << "ParTime " << parTime2 - parTime1 << std::endl;
//   std::cout << "SeqTime " << seqTime2 - seqTime1 << std::endl;
//   ASSERT_EQ(resPar, resSeq);
// }
// }

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  MPI_Init(&argc, &argv);

  ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
  ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();

  listeners.Release(listeners.default_result_printer());
  listeners.Release(listeners.default_xml_generator());

  listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
  return RUN_ALL_TESTS();
}
