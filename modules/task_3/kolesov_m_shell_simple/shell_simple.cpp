// Copyright 2022 Kolesov Maxim
#include <mpi.h>
#include <random>
#include "../../../modules/task_3/kolesov_m_shell_simple/shell_simple.h"

std::vector<int> GenerateVector(int size) {
  std::random_device dev;
  std::mt19937 gen(dev());
  std::vector<int> vec(size);

  for (int  i = 0; i < size; i++) {
    vec[i] = static_cast<int>(gen() % 100);
  }

  return vec;
}

void ShellSequence(std::vector<int> *vec) {
  const int size = static_cast<int>(vec->size());
  int step = size / 2;

  while (step > 0) {
    for (int i = step; i < size; i++) {
      for (int j = i - step; (j >= 0) && (vec->at(j) > vec->at(j+step)); j -= step) {
        std::swap((*vec)[j], (*vec)[j+step]);
      }
    }

    step /= 2;
  }
}

std::vector<int> Merge(const std::vector<std::vector<int>> &vectors) {
  if (vectors.size() == 1) {
    return vectors[0];
  }

  std::vector<int> result(vectors[0].begin(), vectors[0].end());
  for (int i = 1; i < static_cast<int>(vectors.size()); i++) {
    const std::vector<int> &vec = vectors[i];
    auto it = vec.begin();
    int k = 0;

    while (k < static_cast<int>(result.size())) {
      if (it != vec.end() && result[k] > *it) {
        result.insert(result.begin() + k, *it);
        it += 1;
      }

      k += 1;
    }

    while (it != vec.end()) {
      result.insert(result.end(), *it);
      it += 1;
    }
  }

  return result;
}

std::vector<int> Shell(const std::vector<int> &vec, int vectorSize) {
  int size, rank;

  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  const int count = vectorSize / size;
  const int remain = vectorSize % size;
  std::vector<int> local(count);

  if (rank == 0) {
    for (int i = 1; i < size; i++) {
      MPI_Send(vec.data() + i * count, count, MPI_INT, i, 0, MPI_COMM_WORLD);
    }

    local = std::vector<int>(vec.begin(), vec.begin() + count);
  } else {
    MPI_Status status;
    MPI_Recv(local.data(), count, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
  }

  ShellSequence(&local);

  if (rank == 0) {
    std::vector<int> end(vec.end() - remain, vec.end());
    if (remain != 0) {
      ShellSequence(&end);
    }

    std::vector<std::vector<int>> parts;
    parts.push_back(local);
    MPI_Status status;
    for (int i = 1; i < size; i++) {
      MPI_Recv(local.data(), count, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
      parts.push_back(local);
    }

    if (remain != 0) {
      parts.push_back(end);
    }

    return Merge(parts);
  }

  MPI_Send(local.data(), count, MPI_INT, 0, 0, MPI_COMM_WORLD);
  return {};
}
