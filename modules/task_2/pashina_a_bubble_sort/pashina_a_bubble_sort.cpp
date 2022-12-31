// Copyright 2022 Pashina Alina

#include <mpi.h>
#include <algorithm>
#include <ctime>
#include <iostream>
#include <random>
#include <utility>

#include "../../../modules/task_2/pashina_a_bubble_sort/pashina_a_bubble_sort.h"

void typical_bubble_s(int *myarray, int mysize) {
  for (int i = 0; i < mysize - 1; i++)
    for (int j = 0; j < mysize - i - 1; j++)
      if (myarray[j] > myarray[j + 1]) std::swap(myarray[j], myarray[j + 1]);
}

int *doParallel_BubbleS(int *ourarr, int arr_size_in) {
  int len = arr_size_in;
  int th_size;
  MPI_Comm_size(MPI_COMM_WORLD, &th_size);
  int block_size = len / th_size;
  std::srand(time(nullptr));
  int *sub_arr = new int[block_size];
  MPI_Scatter(ourarr, block_size, MPI_INT, sub_arr, block_size, MPI_INT, 0,
              MPI_COMM_WORLD);

  typical_bubble_s(sub_arr, block_size);
  MPI_Gather(sub_arr, block_size, MPI_INT, ourarr, block_size, MPI_INT, 0,
             MPI_COMM_WORLD);
  int th_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &th_rank);
  if (th_rank == 0) typical_bubble_s(ourarr, len);
  return ourarr;
}

int *bubble_s_with_return(int *arr, int n) {
  for (int i = 0; i < n - 1; i++)
    for (int j = 0; j < n - i - 1; j++)
      if (arr[j] > arr[j + 1]) std::swap(arr[j], arr[j + 1]);
  return arr;
}

int generateNewArr(int al) {
  std::mt19937 generator;
  generator.seed(static_cast<unsigned int>(time(0)));
    return static_cast<int>(generator() % 500);
}
