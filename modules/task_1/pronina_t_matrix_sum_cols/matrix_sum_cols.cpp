// Copyright 2022 Pronina Tatiana
#include <mpi.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include "../../../modules/task_1/pronina_t_matrix_sum_cols/matrix_sum_cols.h"

int* CreateRandMatrix(int rows, int cols) {
  if (rows <= 0 || cols <= 0) {
    throw -1;
  }
  int min = 1, max = 100;
  int* Matrix = new int[rows * cols];
  for (int i = 0; i < rows * cols; i++) {
    Matrix[i] =
        min + (std::rand() * static_cast<int>((max + 1) - min) / RAND_MAX);
  }
  return Matrix;
}

int* LinearMetod(int* Matrix, int rows, int cols) {
  int* sum = new int[cols];

  for (int i = 0; i < cols; i++) {
    sum[i] = 0;
  }

  for (int i = 0; i < cols; i++) {
    for (int j = 0; j < rows; j++) {
      sum[i] = sum[i] + Matrix[i + (j * cols)];
    }
  }

  return sum;
}

int* MPIMethod(int* Matrix_init, int rs, int cs) {
  if (rs <= 0 || cs <= 0) {
    throw -1;
  }
  int rows = 0, cols = 0, req = 0;
  int *Matrix = nullptr, *sum_of_col;
  int Size, Rank;
  MPI_Comm_size(MPI_COMM_WORLD, &Size);
  MPI_Comm_rank(MPI_COMM_WORLD, &Rank);

  if (Rank == 0) {
    if (rs % Size > 0) {
      req = ((rs / Size) + 1) * Size - rs;
    }

    Matrix = new int[(rs + req) * cs];

    for (int i = 0; i < rs * cs; i++) {
      Matrix[i] = Matrix_init[i];
    }
    for (int i = rs * cs; i < (rs + req) * cs; i++) {
      Matrix[i] = 0;
    }

    rows = rs + req;
    cols = cs;
  }

  MPI_Bcast(&rows, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&cols, 1, MPI_INT, 0, MPI_COMM_WORLD);

  rows = rows / Size;

  int* BuffMatrix = new int[rows * cols];

  MPI_Scatter(Matrix, rows * cols, MPI_INT, BuffMatrix, rows * cols, MPI_INT, 0,
              MPI_COMM_WORLD);

  sum_of_col = new int[cols];

  int* sum = LinearMetod(BuffMatrix, rows, cols);

  MPI_Reduce(sum, sum_of_col, cols, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  return sum_of_col;
}
