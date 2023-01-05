// Copyright 2022 Frantsuzov Maksim
#include <gtest/gtest.h>
#include <stdlib.h>
#include <iostream>
#include <random>
#include <vector>
#include "./bubble_sort.h"
#include <gtest-mpi-listener.hpp>


TEST(Parallel_Operations_MPI, Test1) {
  int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int size = 10;
  int* array = new int[size];
  int* ref_array;

  std::random_device dev;
  std::mt19937 gen(dev());
  std::uniform_int_distribution<> dist(0, 99);

  for (int i = 0; i < size; i++) {
    array[i] = dist(gen);
  }

  int* arrWithMPI = bubbleSortWithMPI(array, size);

  if (rank == 0) {
    int* arr = bubble_sort_for_main(array, size);
    for (int i = 0; i < size; i++) {
      ASSERT_EQ(arrWithMPI[i], arr[i]);
    }
  }
}

TEST(Parallel_Operations_MPI, Test2) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int size = 0;
  int* array = new int[size];
  int* ref_array;

  std::random_device dev;
  std::mt19937 gen(dev());
  std::uniform_int_distribution<> dist(0, 99);

  for (int i = 0; i < size; i++) {
    array[i] = dist(gen);
  }

  int* arrWithMPI = bubbleSortWithMPI(array, size);

  if (rank == 0) {
    int* arr = bubble_sort_for_main(array, size);
    for (int i = 0; i < size; i++) {
      ASSERT_EQ(arrWithMPI[i], arr[i]);
    }
  }
}

TEST(Parallel_Operations_MPI, Test3) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int size = 1;
  int* array = new int[size];
  int* ref_array;

  std::random_device dev;
  std::mt19937 gen(dev());
  std::uniform_int_distribution<> dist(0, 99);

  for (int i = 0; i < size; i++) {
    array[i] = dist(gen);
  }

  int* arrWithMPI = bubbleSortWithMPI(array, size);

  if (rank == 0) {
    int* arr = bubble_sort_for_main(array, size);
    for (int i = 0; i < size; i++) {
      ASSERT_EQ(arrWithMPI[i], arr[i]);
    }
  }
}

TEST(Parallel_Operations_MPI, Test4) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int size = 100;
  int* array = new int[size];
  int* ref_array;

  std::random_device dev;
  std::mt19937 gen(dev());
  std::uniform_int_distribution<> dist(0, 99);

  for (int i = 0; i < size; i++) {
    array[i] = dist(gen);
  }

  int* arrWithMPI = bubbleSortWithMPI(array, size);

  if (rank == 0) {
    int* arr = bubble_sort_for_main(array, size);
    for (int i = 0; i < size; i++) {
      ASSERT_EQ(arrWithMPI[i], arr[i]);
    }
  }
}

TEST(Parallel_Operations_MPI, Test5) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int size = 2;
  int* array = new int[size];
  int* ref_array;

  std::random_device dev;
  std::mt19937 gen(dev());
  std::uniform_int_distribution<> dist(0, 99);

  for (int i = 0; i < size; i++) {
    array[i] = dist(gen);
  }

  int* arrWithMPI = bubbleSortWithMPI(array, size);

  if (rank == 0) {
    int* arr = bubble_sort_for_main(array, size);
    for (int i = 0; i < size; i++) {
      ASSERT_EQ(arrWithMPI[i], arr[i]);
    }
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
