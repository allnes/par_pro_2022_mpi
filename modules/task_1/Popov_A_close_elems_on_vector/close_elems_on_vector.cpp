// Copyright 2022 Popov Andrey
#include "../../../modules/task_1/Popov_A_close_elems_on_vector/close_elems_on_vector.h"

#include <mpi.h>


#include <algorithm>
#include <climits>
#include <cmath>

#include <random>

#include <vector>

std::vector<int> getRandVect(int size) {
  std::random_device rand_d;
  std::mt19937 eng(rand_d());
  std::vector<int> vect(size);
  for (int k = 0; k < size; k++) {
    vect[k] = eng() % 100;
  }
  return vect;
}

int getSeriallOps(const std::vector<int> &vect) {
  int sz = vect.size();
  int Minimal_Diff = INT_MAX;
  if (sz != 0) {
    for (int j = 0; j < static_cast<int>(vect.size()) - 1; j++) {
      if (Minimal_Diff > abs(vect[j] - vect[j + 1]))
        Minimal_Diff = abs(vect[j] - vect[j + 1]);
    }
  }
  if (sz == 1) {
    return 0;
  }
  return Minimal_Diff;
}

int getParallelOps(const std::vector<int> &vect_glob) {
  int Minimal_Diff = INT_MAX;
  int sz, level;

  MPI_Comm_size(MPI_COMM_WORLD, &sz);
  MPI_Comm_rank(MPI_COMM_WORLD, &level);

  MPI_Status Status;
  int vect_size = vect_glob.size() - 1;
  int buffer_size = vect_size / sz;
  int result = vect_size % sz;

  if (level == 0) {
    if (buffer_size > 0) {
      for (int j = 1; j < sz; j++)
        MPI_Send(&vect_glob[result] + buffer_size * j, buffer_size, MPI_INT, j,
                 0, MPI_COMM_WORLD);
    }
  } else {
    if (buffer_size > 0) {
      std::vector<int> tmp(buffer_size + 1);
      MPI_Recv(&tmp[0], buffer_size + 1, MPI_INT, 0, MPI_ANY_TAG,
               MPI_COMM_WORLD, &Status);
      int MinDif = getSeriallOps(tmp);
      MPI_Send(&MinDif, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
  }

  if (level == 0) {
    int tmp1;

    std::vector<int> t(vect_glob.begin(),
                       vect_glob.begin() + 1 + buffer_size + result);

    Minimal_Diff = getSeriallOps(t);

    if (buffer_size >= 1) {
      for (int m = 1; m < sz; m++) {
        MPI_Recv(&tmp1, 1, MPI_INT, m, 0, MPI_COMM_WORLD, &Status);
        Minimal_Diff = std::min(Minimal_Diff, tmp1);
      }
    }
  }
  return Minimal_Diff;
}
