// Copyright 2022 Kandrin Alexey
#include <mpi.h>
#include <algorithm>
#include "../../../modules/task_1/kandrin_a_min_value_by_rows/min_value_by_rows.h"

//=============================================================================
// Function : WorkSplitter::WorkSplitter
// Purpose  : Constructor.
//=============================================================================
WorkSplitter::WorkSplitter(size_t work, size_t workerCount)
    : m_workDistribution(workerCount, 0) {
  if (work <= workerCount) {
    for (size_t currentWorker = 0; currentWorker < work; ++currentWorker) {
      m_workDistribution[currentWorker] = 1;
    }
  } else {
    for (size_t currentWorker = 0; work != 0; ++currentWorker) {
      size_t workForCurrentWorker = work / workerCount;
      m_workDistribution[currentWorker] = work / workerCount;
      work -= workForCurrentWorker;
      workerCount -= 1;
    }
  }
}

//=============================================================================
// Function : GetPartWork
// Purpose  : Determining how much work a worker should do.
//=============================================================================
size_t WorkSplitter::GetPartWork(size_t workerNumber) const {
  return m_workDistribution[workerNumber];
}

//=============================================================================
// Function : GetMinValuesByRowsParallel
// Purpose  : Same as GetMinValuesByRowsSequential, but the function is
//            executed parallel by several processes using MPI.
//            Also, the function accepts not a template, but an integer matrix.
//=============================================================================
std::vector<int> GetMinValuesByRowsParallel(const Matrix<int>& matrix) {
  int mpiReturnValue = 0;
  MPI_Comm_size(MPI_COMM_WORLD, &mpiReturnValue);
  size_t proccessCount = static_cast<size_t>(mpiReturnValue);

  MPI_Comm_rank(MPI_COMM_WORLD, &mpiReturnValue);
  size_t rank = static_cast<size_t>(mpiReturnValue);

  size_t rowCount = 0, colCount = 0;

  if (rank == 0) {
    rowCount = matrix.GetRowCount();
    colCount = matrix.GetColCount();
  }

  MPI_Bcast(&rowCount, 1, MPI_UNSIGNED_LONG, 0, MPI_COMM_WORLD);
  MPI_Bcast(&colCount, 1, MPI_UNSIGNED_LONG, 0, MPI_COMM_WORLD);

  // now variables rowCount, colCount store actual values in all processes

  // WorkSplitter lets each process know how many rows it is processing.
  // This allows you not to send data about the number of processed rows to
  // processes, but to send only the rows themselves.
  WorkSplitter workSplitter(rowCount, proccessCount);

  // how many rows should the current process process
  size_t rowsForCurrentProccess = workSplitter.GetPartWork(rank);

  if (rowsForCurrentProccess == 0) {
    // no work for current proccess
    return {};
  }

  if (rank == 0) {
    const int* sendPtr = matrix[rowsForCurrentProccess];
    for (size_t proccess = 1; proccess < proccessCount; ++proccess) {
      size_t rowsForProccess = workSplitter.GetPartWork(proccess);
      if (rowsForProccess > 0) {
        MPI_Send(sendPtr, rowsForProccess * colCount, MPI_INT, proccess, 0,
                 MPI_COMM_WORLD);
        sendPtr += rowsForProccess * colCount;
      }
    }
  }

  // The matrix is used to store the rows that the process needs to process
  // sequentially.
  Matrix<int> localMatrix(rowsForCurrentProccess, colCount);

  if (rank == 0) {
    std::copy(matrix.begin(),
              matrix.begin() + rowsForCurrentProccess * colCount,
              localMatrix.begin());

  } else {
    MPI_Status status;

    MPI_Recv(localMatrix.data(), rowsForCurrentProccess * colCount, MPI_INT, 0,
             0, MPI_COMM_WORLD, &status);
  }

  // vector is used to store the final result computed by the null process.
  std::vector<int> globalMinValuesByRows;

  // vector is used to store the partial result that each process computed.
  std::vector<int> localMinValuesByRows =
      GetMinValuesByRowsSequential(localMatrix);

  if (rank == 0) {
    globalMinValuesByRows = std::vector<int>(rowCount);
    std::copy(localMinValuesByRows.begin(), localMinValuesByRows.end(),
              globalMinValuesByRows.begin());

    int* recvPtr = globalMinValuesByRows.data() + rowsForCurrentProccess;

    for (size_t proccess = 1; proccess < proccessCount; ++proccess) {
      size_t rowsByProccess = workSplitter.GetPartWork(proccess);
      if (rowsByProccess > 0) {
        MPI_Status status;

        MPI_Recv(recvPtr, rowsByProccess, MPI_INT, proccess, 0, MPI_COMM_WORLD,
                 &status);

        recvPtr += rowsByProccess;
      }
    }
  } else {
    MPI_Send(localMinValuesByRows.data(), localMinValuesByRows.size(), MPI_INT,
             0, 0, MPI_COMM_WORLD);
  }

  return globalMinValuesByRows;
}
