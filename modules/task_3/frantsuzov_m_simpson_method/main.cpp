// Copyright 2022 Frantsuzov Maksim
#include <gtest/gtest.h>
#include <vector>
#include "./simpson_method.h"
#include <gtest-mpi-listener.hpp>


const int number_of_points_in_the_grid = 1000;

TEST(Parallel_Operations_MPI, Test1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const double x_min = 0;
    const double x_max = 100;
    const double y_min = 0;
    const double y_max = 100;

    const double eps = 0.1;

    double numberWithMPI = simposon_method_with_MPI(
        number_of_points_in_the_grid, x_min, x_max, y_min, y_max);
    if (rank == 0) {
      double numberWithoutMPI = simpson_method(number_of_points_in_the_grid,
                                                x_min, x_max, y_min, y_max);
      ASSERT_NEAR(numberWithoutMPI, numberWithMPI, eps);
    }
}

TEST(Parallel_Operations_MPI, Test2) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  const double x_min = 1;
  const double x_max = 10;
  const double y_min = 1;
  const double y_max = 10;

  const double eps = 0.1;

  double numberWithMPI = simposon_method_with_MPI(number_of_points_in_the_grid,
                                                  x_min, x_max, y_min, y_max);
  if (rank == 0) {
    double numberWithoutMPI = simpson_method(number_of_points_in_the_grid,
                                             x_min, x_max, y_min, y_max);
    ASSERT_NEAR(numberWithoutMPI, numberWithMPI, eps);
  }
}

TEST(Parallel_Operations_MPI, Test3) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  const double x_min = 0;
  const double x_max = 0;
  const double y_min = 0;
  const double y_max = 0;

  const double eps = 0.1;

  double numberWithMPI = simposon_method_with_MPI(number_of_points_in_the_grid,
                                                  x_min, x_max, y_min, y_max);
  if (rank == 0) {
    double numberWithoutMPI = simpson_method(number_of_points_in_the_grid,
                                             x_min, x_max, y_min, y_max);
    ASSERT_NEAR(numberWithoutMPI, numberWithMPI, eps);
  }
}

TEST(Parallel_Operations_MPI, Test4) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  const double x_min = 1;
  const double x_max = 1;
  const double y_min = 1;
  const double y_max = 1;

  const double eps = 0.1;

  double numberWithMPI = simposon_method_with_MPI(number_of_points_in_the_grid,
                                                  x_min, x_max, y_min, y_max);
  if (rank == 0) {
    double numberWithoutMPI = simpson_method(number_of_points_in_the_grid,
                                             x_min, x_max, y_min, y_max);
    ASSERT_NEAR(numberWithoutMPI, numberWithMPI, eps);
  }
}

TEST(Parallel_Operations_MPI, Test5) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  const double x_min = 10;
  const double x_max = 100;
  const double y_min = 10;
  const double y_max = 100;

  const double eps = 0.1;

  double numberWithMPI = simposon_method_with_MPI(number_of_points_in_the_grid,
                                                  x_min, x_max, y_min, y_max);
  if (rank == 0) {
    double numberWithoutMPI = simpson_method(number_of_points_in_the_grid,
                                             x_min, x_max, y_min, y_max);
    ASSERT_NEAR(numberWithoutMPI, numberWithMPI, eps);
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
