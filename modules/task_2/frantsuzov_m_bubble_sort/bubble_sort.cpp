// Copyright 2022 Frantsuzov Maksim
#include <mpi.h>
#include <time.h>
#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <cstdlib>
#include "../../../modules/task_2/frantsuzov_m_bubble_sort/bubble_sort.h"


// Function to swap two elements
void swap(int &a, int &b) {
  int temp = a;
  a = b;
  b = temp;
}

// Function to sort an array using bubble sort
void bubble_sort(int *arr, int n) {
  for (int i = 0; i < n - 1; i++)
    for (int j = 0; j < n - i - 1; j++)
      if (arr[j] > arr[j + 1]) swap(arr[j], arr[j + 1]);
}

int* bubble_sort_for_main(int *arr, int n) {
  for (int i = 0; i < n - 1; i++)
    for (int j = 0; j < n - i - 1; j++)
      if (arr[j] > arr[j + 1]) swap(arr[j], arr[j + 1]);
  return arr;
}

int* bubbleSortWithMPI(int* arr, int size) {
  // Get the number of processes
  int num_procs;
  MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

  // Get the rank of the process
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  // Read the size of the array from the command line
  int n = size;


  // Seed the random number generator
  std::srand(time(nullptr));

  // Divide the array into equal-sized blocks for each process
  int block_size = n / num_procs;
  int *sub_arr = new int[block_size];

  // Scatter the array to all processes
  MPI_Scatter(arr, block_size, MPI_INT, sub_arr, block_size, MPI_INT, 0,
              MPI_COMM_WORLD);

  // Sort the sub-array on each process
  bubble_sort(sub_arr, block_size);

  // Gather the sorted sub-arrays back to the root process
  MPI_Gather(sub_arr, block_size, MPI_INT, arr, block_size, MPI_INT, 0,
             MPI_COMM_WORLD);

  // Root process sorts the entire array using bubble sort
  if (rank == 0) bubble_sort(arr, n);

  // Print the sorted array
  // for (int i = 0; i < n; i++) std::cout << arr[i] << " ";
  // std::cout << std::endl;
  return arr;
}

