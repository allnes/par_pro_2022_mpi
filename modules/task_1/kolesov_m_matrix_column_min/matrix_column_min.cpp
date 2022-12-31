// Copyright 2022 Kolesov Maxim
#include <mpi.h>
#include <random>
#include <limits>

#include "../../../modules/task_1/kolesov_m_matrix_column_min/matrix_column_min.h"

std::vector<int> generateMatrix(int n, int m) {
  std::random_device dev;
  std::mt19937 gen(dev());
  std::vector<int> vec(n*m);

  for (int  i = 0; i < n*m; i++) {
    vec[i] = static_cast<int>(gen() % 100);
  }
  return vec;
}

std::vector<int> transposeMatrix(const std::vector<int> &matrix, int n, int m) {
  size_t size = matrix.size();
  std::vector<int> vec(size);

  int k = 0;
  for (int j = 0; j < m; j++) {
    for (int i = 0; i < n; i++) {
      vec[k++] = matrix[i*m + j];
    }
  }

  return vec;
}

int getMinInSequence(const std::vector<int> &sec) {
  int min = std::numeric_limits<int>::max();
  for (int i : sec) {
    if (min > i) {
      min = i;
    }
  }

  return min;
}

std::vector<int> getColumnMinParalel(const std::vector<int> &matrix, int n, int m) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  const int delta = n;
  const int count = m / size;
  const int remain = m % size;
  int start = 0;

  std::vector<int> part(delta*count);
  std::vector<int> end(delta*remain);
  std::vector<int> localResult(m, std::numeric_limits<int>::max());
  std::vector<int> globalResult(m);

  if (rank == 0) {
    std::vector<int> t = transposeMatrix(matrix, n, m);
    start = count;
    for (int i = 1; i < size; i++) {
      MPI_Send(&start, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
      MPI_Send(t.data() + count * i * delta, delta * count, MPI_INT, i, 0, MPI_COMM_WORLD);
      start += count;
    }

    start = 0;

    part = std::vector<int>(t.begin(), t.begin() + delta * count);
    if (remain != 0) {
      end = std::vector<int>(t.end() - remain*delta, t.end());
    }
  } else {
    MPI_Status status;
    MPI_Recv(&start, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    MPI_Recv(part.data(), delta*count, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
  }

  for (int i = 0; i < count; i++) {
    localResult[start + i] = getMinInSequence(std::vector<int>(part.begin() + i*delta, part.begin() + i*delta + delta));
  }

  if (rank == 0 && remain != 0) {
    start = m - remain;
    for (int i = 0; i < remain; i++) {
      localResult[start + i] = getMinInSequence(std::vector<int>(end.begin() + i*delta, end.begin() + i*delta + delta));
    }
  }

  MPI_Reduce(localResult.data(), globalResult.data(), m, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);
  return globalResult;
}
