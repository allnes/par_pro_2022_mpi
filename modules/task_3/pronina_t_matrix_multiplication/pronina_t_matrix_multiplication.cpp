// Copyright 2022 Pronina Tatiana
#include "../../../modules/task_3/pronina_t_matrix_multiplication/pronina_t_matrix_multiplication.h"
#include <mpi.h>
#include <ctime>
#include <random>
#include <vector>

// Converting a matrix to columnar storage
SparseMatrix CCS(const std::vector<double>& _newMatrix, const int _newColumns,
  const int _newRows) {
  SparseMatrix res;
  res.columns = _newColumns;
  res.rows = _newRows;
  res.non_zero = 0;

  res.col_ptr.push_back(0);
  for (int col = 0; col < _newColumns; col++) {
    int nnzCount = 0;
    for (int i = col; i <= (_newRows - 1) * _newColumns + col;
      i += _newColumns) {
      if (_newMatrix[i] == 0) {
        continue;
      }
      nnzCount++;
      res.val.push_back(_newMatrix[i]);
      res.row_index.push_back((i - col) / _newColumns);
    }
    res.col_ptr.push_back(res.col_ptr.back() + nnzCount);
    res.non_zero += nnzCount;
  }

  return res;
}

// Multiplication operator
const std::vector<double> operator*(const SparseMatrix& _A,
  const SparseMatrix& _B) {
  if (_A.columns != _B.rows) {
    throw "incorrect size";
  }

  std::vector<double> res(_A.rows * _B.columns, 0);

  for (int a_col = 0; a_col < _A.columns; a_col++) {
    for (int b_col = 0; b_col < _B.columns; b_col++) {
      for (int i = _A.col_ptr[a_col]; i <= _A.col_ptr[a_col + 1] - 1; i++) {
        if (_B.col_ptr[b_col + 1] - _B.col_ptr[b_col] == 0) {
          continue;
        }

        for (int j = _B.col_ptr[b_col];
          j < _B.col_ptr[b_col + 1]; j++) {
          if (_B.row_index[j] == a_col) {
            // [interpreting a two-dimensional array into a one-dimensional one]
            res[_A.row_index[i] * _B.columns + b_col]
              += _A.val[i] * _B.val[j];
          }
        }
      }
    }
  }

  return res;
}

// Parallel multiplication
std::vector<double> Multiply(SparseMatrix _A, SparseMatrix _B) {
  int ProcRank, ProcNum;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);

  if (ProcNum == 1) {
    return _A * _B;
  }

  // Data is sent to all processes
  MPI_Bcast(&_A.columns, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&_A.rows, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&_A.non_zero, 1, MPI_INT, 0, MPI_COMM_WORLD);

  MPI_Bcast(&_B.columns, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&_B.rows, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&_B.non_zero, 1, MPI_INT, 0, MPI_COMM_WORLD);

  if (_A.non_zero == 0 || _B.non_zero == 0) {
    // The main process does the multiplication sequentially
    if (ProcRank == 0) {
      return _A * _B;
    } else {
      // the rest of the processes get an empty vector to get something,
      // in order to avoid an error
      return std::vector<double>();
    }
  }

  if (_A.columns != _B.rows) {
    throw "incorrect size";
  }
  // if the number of columns is less than the number of processes
  if (_A.columns < ProcNum) {
    if (ProcRank == 0) {
      return _A * _B;  // The main process does the multiplication sequentially
    } else {
      // the rest of the processes get an empty vector to get something,
      // in order to avoid an error
      return std::vector<double>();
    }
  }

  if (ProcRank != 0) {
    _A.val.resize(_A.non_zero);  // Changing the size
    _A.row_index.resize(_A.non_zero);
    _A.col_ptr.resize(_A.columns + 1);
    _B.val.resize(_B.non_zero);
    _B.row_index.resize(_B.non_zero);
    _B.col_ptr.resize(_B.columns + 1);
  }
  // send data about matrix A to all processes
  MPI_Bcast(&_A.val[0], _A.non_zero, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Bcast(&_A.row_index[0], _A.non_zero, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&_A.col_ptr[0], _A.columns + 1, MPI_INT, 0, MPI_COMM_WORLD);

  // send data about matrix B to all processes
  MPI_Bcast(&_B.val[0], _B.non_zero, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Bcast(&_B.row_index[0], _B.non_zero, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&_B.col_ptr[0], _B.columns + 1, MPI_INT, 0, MPI_COMM_WORLD);

  // distribute to each process its own piece of the matrix A
  int delta = _A.columns / ProcNum;
  // Denoting the boundaries
  int leftBound = ProcRank * delta;
  int rightBound = (ProcRank + 1) * delta;

  if (ProcRank == ProcNum - 1) {
    rightBound = _A.columns;
  }

  // Declaring a vector to store the local result
  // (each process has its own result)
  std::vector<double> localResult(_A.rows * _B.columns);

  for (int a_col = leftBound; a_col < rightBound; a_col++) {
    for (int b_col = 0; b_col < _B.columns; b_col++) {
      for (int i = _A.col_ptr[a_col]; i <= _A.col_ptr[a_col + 1] - 1; i++) {
        if (_B.col_ptr[b_col + 1] - _B.col_ptr[b_col] == 0) {
          continue;
        }

        for (int j = _B.col_ptr[b_col];
          j <= _B.col_ptr[b_col + 1] - 1; j++) {
          if (_B.row_index[j] == a_col) {
            localResult[_A.row_index[i] * _B.columns + b_col]
              += _A.val[i] * _B.val[j];
          }
        }
      }
    }
  }

  // Declaring a vector to collect local results
  std::vector<double> globalResult;
  if (ProcRank == 0) {
    globalResult.resize(_A.rows * _B.columns);
  }

  if (ProcRank == 0) {
    MPI_Reduce(&localResult[0], &globalResult[0], _A.rows * _B.columns,
      MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
  } else {
    MPI_Reduce(&localResult[0], MPI_IN_PLACE, _A.rows * _B.columns,
      MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
  }

  return globalResult;
}

std::vector<double> RandMatrix(const int _columns, const int _rows) {
  if (_rows <= 0 || _columns <= 0) {
    throw "incorrect size";
  }

  std::srand(std::time(nullptr));
  std::vector<double> result(_columns * _rows);

  for (int i = 0; i < _rows * _columns; i++) {
    double randVal = static_cast<double>(std::rand() % 50 + 1);
    if (randVal < 4) {
      result[i] = randVal;
    } else {
      result[i] = 0;
    }
  }
  return result;
}
