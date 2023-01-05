// Copyright 2018 Nesterov Alexander
#include "../../../modules/task_1/frantsuzov_m_matrix_max_in_rows/matrix_max_in_rows.h"

#include <mpi.h>

#include <algorithm>
#include <cstdlib>
#include <iostream>

// const int N = 10;

int* findMaxElementsInRows(const int rowSize, const int columnSize) {
  int** matrix = new int*[rowSize];
  for (int i = 0; i < rowSize; i++) {
    matrix[i] = new int[columnSize];
  }

  int* row = new int[rowSize];

  for (int i = 0; i < rowSize; i++) {
    for (int j = 0; j < columnSize; j++) {
      matrix[i][j] = i + j;
    }
  }
  int maxValue = -1;

  for (int i = 0; i < rowSize; i++) {
    maxValue = matrix[i][0];
    for (int j = 0; j < columnSize; j++) {
      if (maxValue < matrix[i][j]) {
        maxValue = matrix[i][j];
      }
    }
    row[i] = maxValue;
  }

  /*for (int i = 0; i < rowSize; i++) {
    for (int j = 0; j < columnSize; j++) {
      std::cout << matrix[i][j] << " ";
    }
    std::cout << std::endl;
  }*/

  /*for (int i = 0; i < rowSize; i++) {
    std::cout << row[i] << " ";
  }*/
  return row;
}

int* findMaxElementsInRowsMPI(const int rowSize, const int columnSize) {
  // Get the number of processes
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  // Get the rank of the process
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  // Create the matrix and fill it with some values
  // int rowSize = 4, columnSize = 5;
  int** mat = new int*[rowSize];
  // if (world_rank == 0) {
  for (int i = 0; i < rowSize; i++) {
    mat[i] = new int[columnSize];
    for (int j = 0; j < columnSize; j++) {
      mat[i][j] = i + j;  // use a different formula to fill the matrix
    }
  }
  //}

  // MPI_Bcast(mat, rowSize * columnSize, MPI_INT, 0, MPI_COMM_WORLD);

  // MPI_Scatter(mat, rowSize * columnSize, MPI_INT, mat, rowSize * columnSize,
  // MPI_INT, 0, MPI_COMM_WORLD);

  // Calculate the number of rows each process should handle
  int rows_per_process = rowSize / world_size;
  int start_index = world_rank * rows_per_process;
  if (world_rank == world_size - 1) {
    rows_per_process += rowSize % world_size;
  }
  int end_index = start_index + rows_per_process;

  // Find the maximum element of each row in the matrix
  int* max = new int[rowSize];  // dynamically allocate the max array

  for (int i = start_index; i < end_index; i++) {
    max[i - start_index] = mat[i][0];  // initialize the maximum element with
                                       // the first
                                       // element of the row
    for (int j = 1; j < columnSize; j++) {
      // update the maximum element if a larger element is found
      if (mat[i][j] > max[i - start_index]) {
        max[i - start_index] = mat[i][j];
      }
    }
  }

  /*for (int i = 0; i < rowSize; i++) {
    if (i < start_index || i >= end_index) {
      max[i] = (world_rank + 1) * 100 + i;
    }
  }*/

  /*std::cout << std::endl;
  std::cout << "Array max: " << std::endl;
  for (int i = 0; i < rowSize; i++) {
    std::cout << max[i] << " ";
  }
  std::cout << std::endl;*/

  // Gather the maximum elements from each process in the root process
  int* max_global =
      NULL;  // new int[rowSize];  // dynamically allocate the max_global array
  if (world_rank == 0) {
    max_global = new int[rowSize];
  }
  /*int rCount = rows_per_process;
  if (world_rank == world_size - 1) {
    rCount -= rowSize % world_size;
  }*/
  int* send_counts = new int[world_size];
  int* displacements = new int[world_size];
  for (int i = 0; i < world_size; i++) {
    send_counts[i] = rowSize / world_size;
    if (i == world_size - 1) {
      send_counts[i] += rowSize % world_size;
    }
    if (i == 0) {
      displacements[i] = 0;
    } else {
      displacements[i] = displacements[i - 1] + send_counts[i - 1];
    }
  }

  // MPI_Gather(max, rows_per_process, MPI_INT, max_global, rows_per_process,
  // MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Gatherv(max, rows_per_process, MPI_INT, max_global, send_counts,
              displacements, MPI_INT, 0, MPI_COMM_WORLD);

  // Print the maximum elements in the root process
  /*if (world_rank == 0) {
    std::cout << "The maximum element of each row is: ";
    for (int i = 0; i < rowSize; i++) {
      std::cout << max_global[i] << " ";
    }
    std::cout << std::endl;
  }*/

  // MPI_Finalize();

  return max_global;
}
/*// Deallocate the max_global array
delete[] max_global;

// Deallocate the max array
delete[] max;

// Deallocate the matrix
for (int i = 0; i < rowSize; i++) {
 delete[] mat;
}*/
