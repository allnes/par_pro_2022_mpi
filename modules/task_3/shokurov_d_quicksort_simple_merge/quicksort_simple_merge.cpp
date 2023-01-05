// Copyright 2022 Shokurov Daniil
#include "../../../modules/task_3/shokurov_d_quicksort_simple_merge/quicksort_simple_merge.h"

#include <mpi.h>
#include <algorithm>
#include <vector>
#include <iostream>
#include <utility>

std::pair<int, int> split(int* arr, int n) {
  int val = arr[n / 2];
  int l = 0, r = n - 1;
  while (l <= r) {
    while (arr[l] < val) ++l;
    while (arr[r] > val) --r;
    if (l <= r) {
      std::swap(arr[l], arr[r]);
      l++;
      r--;
    }
  }
  return {l, r};
}

void single_quick_sort(int* arr, int n) {
  if (n < 2) return;
  std::pair<int, int> pos = split(arr, n);
  if (pos.second > 0) single_quick_sort(arr, pos.second + 1);
  if (pos.first < n) single_quick_sort(&arr[pos.first], n - pos.first);
}

void merge(int* arr1, int size1, int size2) {
  int* arr2 = &arr1[size1];
  int com_size = size1 + size2;
  int* arr = new int[com_size];
  int l, r;
  l = r = 0;
  int i = 0;
  while (l < size1 && r < size2) {
    if (arr1[l] < arr2[r]) {
      arr[i] = arr1[l];
      i++;
      l++;
    } else {
      arr[i] = arr2[r];
      i++;
      r++;
    }
  }
  while (l < size1) arr[i++] = arr1[l++];
  while (r < size2) arr[i++] = arr2[r++];

  for (i = 0; i < com_size; ++i) {
    arr1[i] = arr[i];
  }
  delete[] arr;
}

void parallel_quick_sort(int* arr, size_t n) {
  int rank = 0;
  int ProcNum = 0;
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0) {
    MPI_Status status;
    int my_count = (n % ProcNum);
    int other_count = (n - my_count) / ProcNum;
    my_count += other_count;
    for (int i = 1; i < ProcNum; ++i) {
      MPI_Send(&arr[my_count + other_count * (i - 1)], other_count, MPI_INT, i,
               123, MPI_COMM_WORLD);
    }
    single_quick_sort(arr, my_count);

    for (int i = 1; i < ProcNum; ++i) {
      MPI_Recv(&arr[my_count + other_count * (i - 1)], other_count, MPI_INT, i,
               456, MPI_COMM_WORLD, &status);
    }

    for (int i = 1; i < ProcNum; ++i) {
      merge(arr, my_count + other_count * (i - 1), other_count);
    }
  } else {
    MPI_Status status;
    int* arr;
    int count;
    MPI_Probe(0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    MPI_Get_count(&status, MPI_INT, &count);
    arr = new int[count];
    MPI_Recv(arr, count, MPI_INT, 0, 123, MPI_COMM_WORLD, &status);

    single_quick_sort(arr, count);

    MPI_Send(arr, count, MPI_INT, 0, 456, MPI_COMM_WORLD);

    delete[] arr;
  }
}

bool check(int* arr, int n) {
  for (int i = 0; i < n - 1; ++i) {
    if (arr[i] > arr[i + 1]) return false;
  }
  return true;
}
