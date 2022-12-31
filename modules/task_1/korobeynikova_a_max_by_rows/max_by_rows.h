#pragma once
// Copyright 2022 Korobeynikova Alisa
#ifndef MODULES_TEST_TASKS_TEST_MPI_OPS_MPI_H_
#define MODULES_TEST_TASKS_TEST_MPI_OPS_MPI_H_

#include <random>
#include <string>
#include <vector>

template <typename T>
class Matrix {
 private:
  std::vector<T> data;
  int n = 0, m = 0;

 public:
  Matrix() {}
  Matrix(int _n, int _m) : data(std::vector<T>(_n * _m)), n(_n), m(_m) {
    std::random_device dev;
    std::mt19937 gen(dev());
    for (int i = 0; i < n * m; i++) {
      data[i] = gen() % 100;
    }
  }
  int GetN() const { return n; }
  int GetM() const { return m; }
  std::vector<T> GetData() { return data; }
  T* operator[](int pos) { return data.data() + pos * m; }
  const T* operator[](int pos) const { return data.data() + pos * m; }
};
std::vector<int> taskDistrib(const int proc_num, const int task_num);
int findMax(const std::vector<int>& vec);
std::vector<int> getParallelOperation(const Matrix<int>& global_matr);

#endif  // MODULES_TEST_TASKS_TEST_MPI_OPS_MPI_H_
