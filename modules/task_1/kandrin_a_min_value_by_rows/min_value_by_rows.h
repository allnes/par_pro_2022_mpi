// Copyright 2022 Kandrin Alexey
#ifndef MODULES_TASK_1_KANDRIN_A_MIN_VALUE_BY_ROWS_MIN_VALUE_BY_ROWS_H_
#define MODULES_TASK_1_KANDRIN_A_MIN_VALUE_BY_ROWS_MIN_VALUE_BY_ROWS_H_

#include <random>
#include <vector>

//=============================================================================
// Class   : Matrix
// Purpose : Storing a two-dimensional matrix as a one-dimensional array.
//=============================================================================
template <class T>
class Matrix {
  size_t m_rowCount{0};
  size_t m_colCount{0};
  std::vector<T> m_matrixData{};

 public:
  Matrix() = default;
  Matrix(const Matrix& matrix) = default;
  Matrix(Matrix&& matrix) = default;
  Matrix& operator=(const Matrix&) = default;
  Matrix& operator=(Matrix&&) = default;

  Matrix(size_t rowCount, size_t colCount)
      : m_rowCount(rowCount),
        m_colCount(colCount),
        m_matrixData(rowCount * colCount, 0) {}

  typename std::vector<T>::iterator begin() { return m_matrixData.begin(); }

  typename std::vector<T>::const_iterator begin() const {
    return m_matrixData.begin();
  }

  typename std::vector<T>::iterator end() { return m_matrixData.end(); }

  typename std::vector<T>::const_iterator end() const {
    return m_matrixData.end();
  }

  T* data() { return m_matrixData.data(); }

  T* operator[](size_t index) {
    T* rowPtr = m_matrixData.data() + index * m_colCount;
    return rowPtr;
  }

  const T* operator[](size_t index) const {
    const T* rowPtr = m_matrixData.data() + index * m_colCount;
    return rowPtr;
  }

  size_t GetRowCount() const { return m_rowCount; }

  size_t GetColCount() const { return m_colCount; }
};

//=============================================================================
// Class   : WorkSplitter
// Purpose : Calculating how much work the worker needs to do.
//=============================================================================
class WorkSplitter {
  std::vector<size_t> m_workDistribution;

 public:
  // Constructor.
  // \param work Total amount of work.
  // \param workerCount Total number of workers who can do the work.
  WorkSplitter(size_t work, size_t workerCount);

  // Determining how much work a worker should do.
  // \param workerNumber The worker for whom we calculate how much work he has to do.
  // \return What part of the work should worker do.
  // (If there are fewer work than workerCount, then some workers will do
  // nothing. If work is proportional to the workerCount, then all workers will
  // do the same amount of work)
  size_t GetPartWork(size_t workerNumber) const;
};

//=============================================================================
// Function : GetRandomMatrix
// Purpose  : Generating a matrix of given sizes and filling it with random
//            values.
//=============================================================================
template <class T, class RandomFunctor>
Matrix<T> GetRandomMatrix(size_t rowCount, size_t colCount,
                          RandomFunctor&& random) {
  Matrix<T> matrix(rowCount, colCount);
  for (size_t i = 0; i < rowCount; ++i) {
    for (size_t j = 0; j < colCount; ++j) {
      matrix[i][j] = random();
    }
  }
  return matrix;
}

//=============================================================================
// Function : GetMinValuesByRowsSequential
// Purpose  : The function looks for the minimum value in each row of the
//            matrix and returns a vector of these values.
//            The function is executed sequentially by one process.
//=============================================================================
template <class T>
std::vector<T> GetMinValuesByRowsSequential(const Matrix<T>& matrix) {
  size_t rowCount = matrix.GetRowCount();
  size_t colCount = matrix.GetColCount();

  if (rowCount == 0 || colCount == 0) {
    return {};
  }

  std::vector<T> result(rowCount);
  for (size_t i = 0; i < rowCount; ++i) {
    T currentMin = matrix[i][0];
    for (size_t j = 1; j < colCount; ++j) {
      T currentValue = matrix[i][j];
      if (currentValue < currentMin) {
        currentMin = currentValue;
      }
    }
    result[i] = currentMin;
  }

  return result;
}

//=============================================================================
// Function : GetMinValuesByRowsParallel
// Purpose  : Same as GetMinValuesByRowsSequential, but the function is
//            executed parallel by several processes using MPI.
//            Also, the function accepts not a template, but an integer matrix.
//=============================================================================
std::vector<int> GetMinValuesByRowsParallel(const Matrix<int>& matrix);

#endif  // MODULES_TASK_1_KANDRIN_A_MIN_VALUE_BY_ROWS_MIN_VALUE_BY_ROWS_H_
