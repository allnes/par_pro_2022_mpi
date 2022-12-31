// Copyright 2022 Antonova Anastasya
#include <mpi.h>
#include <vector>
#include <random>
#include <ctime>
#include <algorithm>
#include "../../../modules/task_1/antonova_n_num_viol_order_vec/num_violation_order_vector.h"

std::vector<int> getRandomVector(int length) {
  if (length < 1)
    throw "WRONG_LEN";

  std::vector<int> vec(length);
  std::mt19937 gen;
  gen.seed(static_cast<unsigned int>(time(0)));

  for (int i = 0; i < length; i++) {
    vec[i] = gen() % 100;
  }
  return vec;
}

int getNumViolationOrderVector(std::vector<int> vec) {
  const int len = vec.size();
  int num = 0;

  for (int i = 1; i < len; i++) {
    if (vec[i-1] > vec[i])
      num++;
  }
  return num;
}

int getNumViolationOrderVectorParallel(std::vector<int> global_vec, int size_vector) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const int delta = size_vector / size;
  if (size_vector < size) {
    return getNumViolationOrderVector (global_vec);
  }
  if (rank == 0) {
    for (int proc = 1; proc < size; proc++) {
    MPI_Send(&global_vec[0] + (proc - 1) * delta, delta + 1, MPI_INT, proc, 0, MPI_COMM_WORLD);
    }
  }

  std::vector<int> local_vec;
  const int size_local_vector = delta + 1;

  if (rank == 0) {
    local_vec = std::vector<int>(global_vec.begin() + (size - 1) * delta, global_vec.end());
  } else {
    local_vec.resize(size_local_vector);
    MPI_Status status;
    MPI_Recv(&local_vec[0], size_local_vector, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
  }

  int local_num = getNumViolationOrderVector(local_vec);
  int global_num = 0;
  MPI_Reduce(&local_num, &global_num, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
  return global_num;
}
