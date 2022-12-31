// Copyright 2022 Pashina Alina

#include <gtest/gtest.h>
#include "../../../modules/task_2/pashina_a_bubble_sort/pashina_a_bubble_sort.h"
#include <gtest-mpi-listener.hpp>

TEST(doParallel_BubbleS, par_test_2) {
  int th_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &th_rank);
  int mysize = 2;
  int* start_arr = new int[mysize];
  int* ref_array;
  for (int i = 0; i < mysize; i++) {
    start_arr[i] = generateNewArr(mysize);
  }
  int* res_arr_Par = doParallel_BubbleS(start_arr, mysize);
  if (th_rank == 0) {
    int* arr = bubble_s_with_return(start_arr, mysize);
    ASSERT_EQ(res_arr_Par, arr);
  }
}

TEST(doParallel_BubbleOddEven_MPI, par_test_11) {
  int th_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &th_rank);
  int mysize = 11;
  int* start_arr = new int[mysize];
  int* ref_array;

  for (int i = 0; i < mysize; i++) {
    start_arr[i] = generateNewArr(mysize);
  }
  int* res_arr_Par = doParallel_BubbleS(start_arr, mysize);

  if (th_rank == 0) {
    int* arr = bubble_s_with_return(start_arr, mysize);
    ASSERT_EQ(res_arr_Par, arr);
  }
}

TEST(doParallel_BubbleOddEven_MPI, par_test_15) {
  int th_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &th_rank);
  int mysize = 11;
  int* start_arr = new int[mysize];
  int* ref_array;

  for (int i = 0; i < mysize; i++) {
    start_arr[i] = generateNewArr(mysize);
  }
  int* res_arr_Par = doParallel_BubbleS(start_arr, mysize);

  if (th_rank == 0) {
    int* arr = bubble_s_with_return(start_arr, mysize);
    ASSERT_EQ(res_arr_Par, arr);
  }
}

TEST(doParallel_BubbleOddEven_MPI, par_test_20) {
  int th_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &th_rank);
  int mysize = 11;
  int* start_arr = new int[mysize];
  int* ref_array;

  for (int i = 0; i < mysize; i++) {
    start_arr[i] = generateNewArr(mysize);
  }
  int* res_arr_Par = doParallel_BubbleS(start_arr, mysize);

  if (th_rank == 0) {
    int* arr = bubble_s_with_return(start_arr, mysize);
    ASSERT_EQ(res_arr_Par, arr);
  }
}
TEST(doParallel_BubbleOddEven_MPI, par_test_3) {
  int th_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &th_rank);
  int mysize = 3;
  int* start_arr = new int[mysize];
  int* ref_array;

  for (int i = 0; i < mysize; i++) {
    start_arr[i] = generateNewArr(mysize);
  }
  int* res_arr_Par = doParallel_BubbleS(start_arr, mysize);

  if (th_rank == 0) {
    int* arr = bubble_s_with_return(start_arr, mysize);
    ASSERT_EQ(res_arr_Par, arr);
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
