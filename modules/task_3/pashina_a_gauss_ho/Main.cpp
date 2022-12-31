// Copyright 2022 Pashina Alina

#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include "../../../modules/task_3/pashina_a_gauss_ho/pashina_a_gauss_h.h"
#include <gtest-mpi-listener.hpp>

TEST(generateImage_MPI, can_create_img) {
  int t_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &t_rank);
  if (t_rank == 0) {
    int heightIm = 6;
    int widhtIm = 5;
    ASSERT_NO_THROW(generateNewImage(heightIm, widhtIm));
  }
}

  TEST(gauss_filt_MPI, gauss_is_working) {
    int t_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &t_rank);
    int heightIm = 5;
    int widhtIm = 10;
    std::vector<unsigned char> testImg = generateNewImage(heightIm, widhtIm);
    if (t_rank == 0) {
      ASSERT_NO_THROW(gauss_filt(testImg, heightIm, widhtIm));
    }
  }

  TEST(doParallel_fGauss_MPI, Gauss_works6_3) {
    int t_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &t_rank);
    std::vector<unsigned char> resThPar;
    std::vector<unsigned char> resStraight;
    int heightIm = 6;
    int widhtIm = 3;
    std::vector<unsigned char> testImg = generateNewImage(heightIm, widhtIm);
    if (t_rank == 0) {
      resStraight = gauss_filt(testImg, heightIm, widhtIm);
      ASSERT_EQ(resStraight, resStraight);
    }
  }

  TEST(doParallel_fGauss_MPI, Gauss_work7_20) {
    int t_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &t_rank);
    std::vector<unsigned char> resThPar;
    std::vector<unsigned char> resStraight;
    int heightIm = 7;
    int widhtIm = 20;
    std::vector<unsigned char> testImg = generateNewImage(heightIm, widhtIm);

    if (t_rank == 0) {
      resStraight = gauss_filt(testImg, heightIm, widhtIm);
      ASSERT_EQ(resStraight, resStraight);
    }
  }

  TEST(doParallel_fGauss_MPI, TryLen50_50) {
    int tmp = 0;
    int t_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &t_rank);
    std::vector<unsigned char> resThPar;
    std::vector<unsigned char> resStraight;
    int heightIm = 50;
    int widhtIm = 50;
    std::vector<unsigned char> testImg = generateNewImage(heightIm, widhtIm);
    if (t_rank == 0) {
      resStraight = gauss_filt(testImg, heightIm, widhtIm);
      ASSERT_EQ(resStraight, resStraight);
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

