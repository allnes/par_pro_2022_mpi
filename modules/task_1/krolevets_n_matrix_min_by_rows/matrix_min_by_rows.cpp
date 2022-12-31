// Copyright 2022 me
#include "../../../modules/task_1/krolevets_n_matrix_min_by_rows/matrix_min_by_rows.h"

#include <random>

void generate_matrix(int* matrix, int size_x, int size_y) {
  assert(size_x > 0 && size_y > 0);
  std::random_device
      rd;  // Will be used to obtain a seed for the random number engine
  std::mt19937 gen(rd());  // Standard mersenne_twister_engine seeded with rd()
  std::uniform_int_distribution<> distrib(std::numeric_limits<int>::min(),
                                          std::numeric_limits<int>::max());
  for (int i = 0; i < size_x * size_y; ++i) {
    matrix[i] = distrib(gen);
  }
}

void min_by_rows(int* matrix, int* result, int size_x, int size_y) {
  assert(size_x > 0 && size_y > 0);

  int* local_matrix;
  int* local_result;

  int rank, pcount;
  MPI_Status status;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &pcount);

  int rows_per_process = size_y / pcount == 0 ? 1 : size_y / pcount;

  if (rank == 0) {
    for (int i = 1; i < std::min(pcount, size_y); ++i) {
      MPI_Send(matrix + size_x * i * rows_per_process,
               size_x * rows_per_process, MPI_INT, i, 0, MPI_COMM_WORLD);
    }
    for (int i = 1; i < std::min(pcount, size_y); ++i) {
      MPI_Recv(result + i * rows_per_process, rows_per_process, MPI_INT, i, 0,
               MPI_COMM_WORLD, &status);
    }

    for (int i = 0; i < rows_per_process; ++i) {
      result[i] =
          *(std::min_element(matrix + size_x * i, matrix + size_x * (i + 1)));
    }
    for (int i = rows_per_process * pcount; i < size_y; ++i) {
      result[i] =
          *(std::min_element(matrix + size_x * i, matrix + size_x * (i + 1)));
    }
  } else if (rank < size_y) {
    local_matrix = new int[size_x * rows_per_process];
    local_result = new int[rows_per_process];
    MPI_Recv(local_matrix, size_x * rows_per_process, MPI_INT, 0, 0,
             MPI_COMM_WORLD, &status);
    for (int i = 0; i < rows_per_process; ++i) {
      local_result[i] = *(std::min_element(local_matrix + size_x * i,
                                           local_matrix + size_x * (i + 1)));
    }
    MPI_Send(local_result, rows_per_process, MPI_INT, 0, 0, MPI_COMM_WORLD);
  }
}

void min_by_rows_seq(int* matrix, int* result, int size_x, int size_y) {
  for (int i = 0; i < size_y; ++i) {
    result[i] =
        *(std::min_element(matrix + i * size_x, matrix + (i + 1) * size_x));
  }
}
