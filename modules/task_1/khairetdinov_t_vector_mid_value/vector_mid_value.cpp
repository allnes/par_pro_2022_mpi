// Copyright 2022 Khairetdinov Timur
#include <mpi.h>
#include <iostream>
#include <random>
#include <ctime>
#include <numeric>
#include <vector>
#include <algorithm>
#include "../../../modules/task_1/khairetdinov_t_vector_mid_value/vector_mid_value.h"

std::vector<int> getRandomVector(int size) {
  std::mt19937 gen;
  gen.seed(static_cast<unsigned int>(time(0)));
  std::vector<int> vector(size);
  for (int i = 0; i < size; i++) {
    vector[i] = gen() % 10;
  }
  return vector;
}

double midValueOfVectorParallel(const std::vector <int> vector, int vector_size) {
  double global_sum = 0.0;
  double local_sum = 0.0;
  int comm_size, rank;
  MPI_Status status;
  MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (vector_size < comm_size) {
    return sumOfVectorSequential(vector) / static_cast<double>(vector_size);
  }
  const int interval = vector_size / comm_size;
  const int last_interval = vector_size % comm_size;

  std::vector<int>local_vector(interval);

  if (rank == 0) {
    if (interval != 0) {
      for (int proc = 1; proc < comm_size; proc++) {
        MPI_Send(&vector[0] + proc * interval + last_interval, interval, MPI_INT, proc, 0, MPI_COMM_WORLD);
      }
    }

    local_vector = std::vector<int>(vector.begin(), vector.begin() + interval + last_interval);
  } else if (interval != 0) {
    MPI_Recv(&local_vector[0], interval, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
  }

  local_sum = sumOfVectorSequential(local_vector);

  if (interval != 0) {
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
  }

  return global_sum / static_cast<double>(vector_size);
}

double sumOfVectorSequential(const std::vector<int> vector) {
  double sum = 0;
  int size = vector.size();
  for (int i = 0; i < size; i++) {
    sum += vector[i];
  }
  return sum;
}
