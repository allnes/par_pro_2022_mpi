// Copyright 2018 Nesterov Alexander
#include <mpi.h>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include "../../../modules/task_1/smirnov_a_matrix_row_sum/matrix_row_sum.h"


std::vector<int> getRandomMatrix(int countRows, int countColumns) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::vector<int> matrix(countRows * countColumns);
    for (int  i = 0; i < countRows * countColumns; i++) { matrix[i] = gen() % 100; }
    return matrix;
}

std::vector<int> calculateSequentialMatrixRowSum(const std::vector<int>& matrix, int countRows, int lenRow) {
  std::vector<int> result(countRows);
  for (size_t i = 0; i < countRows; i++) {
    for (size_t j = i * lenRow; j < i * lenRow + lenRow; j++) {
      result[i] += matrix[j];
    }
  }
  return result;
}

int calculateSumRow(std::vector<int> row) {
  int result = 0;
  for (size_t i = 0; i < row.size(); i++) { result += row[i]; }
  return result;
}

std::vector<int> calculateParallelMatrixRowSum(std::vector<int> global_matrix, int countRows, int lenRow) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int dataPerProc = countRows / size;
  int remainder = countRows % size;

  std::vector<int> localBuff;
  std::vector<int> localResult;
  std::vector<int> result;

  if (dataPerProc == 0) {
    if (rank == 0) {
      result.resize(countRows);
      result = calculateSequentialMatrixRowSum(global_matrix, countRows, lenRow);
    }
  } else {
    if (rank == 0) {
      localBuff.resize(dataPerProc * lenRow);
      result.resize(countRows);
      MPI_Scatter(global_matrix.data(), dataPerProc * lenRow, MPI_INT,
        localBuff.data(), dataPerProc * lenRow, MPI_INT, 0, MPI_COMM_WORLD);
      localResult = calculateSequentialMatrixRowSum(localBuff, dataPerProc, lenRow);
      MPI_Gather(localResult.data(), dataPerProc, MPI_INT,
        result.data(), dataPerProc, MPI_INT, 0, MPI_COMM_WORLD);
      if (remainder != 0) {
        int calculatedIndex = dataPerProc * lenRow * size;
        std::vector<int> tmp(global_matrix.begin() + calculatedIndex, global_matrix.begin() +
          calculatedIndex + remainder * lenRow);
        localResult = calculateSequentialMatrixRowSum(tmp, remainder, lenRow);

        for (size_t i = dataPerProc * size; i < dataPerProc * size + remainder; i++) {
          result[i] = localResult[i - dataPerProc * size];
        }
      }
    } else {
      localBuff.resize(dataPerProc * lenRow);
      MPI_Scatter(nullptr, dataPerProc * lenRow, MPI_INT,
        localBuff.data(), dataPerProc * lenRow, MPI_INT, 0, MPI_COMM_WORLD);
      localResult = calculateSequentialMatrixRowSum(localBuff, dataPerProc, lenRow);
      MPI_Gather(localResult.data(), dataPerProc, MPI_INT,
        nullptr, dataPerProc, MPI_INT, 0, MPI_COMM_WORLD);
    }
  }
  return result;
}
