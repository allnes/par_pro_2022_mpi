// Copyright 2022 Elistratov Vladimir
#include "../../../modules/task_1/elistratov_v_letter_count/letter_count.h"

#include <mpi.h>

#include <iostream>
#include <random>
#include <string>
#include <vector>

constexpr char str[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890!,.&^$#@*()-"
    "_+=";

int calculateCountSequental(std::string result) {
  return std::count_if(result.begin(), result.end(), [](char c) {
    return ((c >= 'A') && (c <= 'Z')) || ((c >= 'a') && (c <= 'z'));
  });
}

int calculateCountParallel(const std::string &result, int elements_count) {
  int size, rank;

  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int delta = elements_count / size;
  int remain = elements_count - delta * size;

  if (rank == 0) {
    for (int process_num = 1; process_num < size; process_num++)
      MPI_Send(&result[remain + process_num * delta], delta, MPI_CHAR,
               process_num, 0, MPI_COMM_WORLD);
  }

  std::string part_string;
  part_string.resize(rank == 0 ? delta + remain : delta);

  if (rank == 0) {
    part_string = {result.begin(), result.begin() + delta + remain};
  } else {
    MPI_Status status;
    MPI_Recv(&part_string[0], delta, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
  }

  int sum = 0;
  int part_sum = calculateCountSequental(part_string);

  MPI_Reduce(&part_sum, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  return sum;
}

std::string CreateRandomString(int elems_count) {
  std::string result;
  result.resize(elems_count);
  srand(static_cast<unsigned>(time(0)));
  for (int i = 0; i < elems_count; i++) result[i] = str[rand() % 77];
  std::string str("");
  return result;
}
