// Copyright 2022 Antonova Anastasya
#include <mpi.h>
#include <vector>
#include <random>
#include <ctime>
#include <algorithm>
#include <numeric>
#include <stdexcept>
#include "../../../modules/task_2/antonova_n_smoothing_image/smoothing_image.h"

std::vector<int> getImg(const int rows, const int cols) {
  if (rows < 0 || cols < 0)
    throw - 1;
  std::mt19937 gen;
  gen.seed(static_cast<unsigned int>(time(0)));
  std::vector<int> Matrix(rows * cols);
  for (int i = 0; i < rows * cols; i++)
    Matrix[i] = gen() % 256;
  return Matrix;
}

std::vector<int> SequintialSmoothing(const std::vector<int>& img, int rows, int cols, int correct) {
  if (static_cast<int>(img.size()) != rows * cols || correct < 1) {
    throw - 1;
  }

  std::vector <int> res(rows * cols);
  int mid = 0;
  for (int i = 0; i < rows * cols; i++) {
    mid += img[i];
  }
  mid = mid / (rows * cols);
  float correction = 1.0 + static_cast<float>(correct) / 100;
  std::vector <int> palitra(256);
  for (int i = 0; i < 256; i++) {
    int delta = i - mid;
    palitra[i] = mid - correction * delta;
    if (palitra[i] > 256) {
      palitra[i] = 256;
    }
    if (palitra[i] < 0) {
      palitra[i] = 0;
    }
  }
  for (int i = 0; i < rows * cols; i++) {
    res[i] = palitra[img[i]];
  }
  return res;
}

std::vector<int> ParallelSmoothing(const std::vector<int>& img, int rows, int cols, int correct) {
  if (static_cast<int>(img.size()) != rows * cols || correct < 1) {
    throw - 1;
  }

  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int det = rows / size;
  int rem = rows % size;
  std::vector <int> res(rows * cols);
  std::vector<int> tmp(det * cols);

  int mid = 0;
  int middle = 0;
  if (rank == 0) {
    if (det > 0) {
      for (int proc = 1; proc < size; proc++) {
        MPI_Send(&img[(det * proc + rem) * cols], det * cols, MPI_INT, proc, 1, MPI_COMM_WORLD);
      }
    }
  } else if (det > 0) {
    MPI_Status status;
    MPI_Recv(&tmp[0], det * cols, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
  }

  if (rank == 0) {
    MPI_Status status;
    tmp = std::vector<int>(img.begin(), img.begin() + (det + rem) * cols);
    for (int i = 0; i < (det + rem) * cols; i++) {
      mid += tmp[i];
    }
    if (det > 0) {
      for (int proc = 1; proc < size; proc++) {
        MPI_Recv(&middle, 1, MPI_INT, proc, 0, MPI_COMM_WORLD, &status);
        mid += middle;
      }
    }
    mid = mid / (cols * rows);
  } else if (det > 0) {
    for (int i = 0; i < det * cols; i++) {
      mid += tmp[i];
    }
    MPI_Send(&mid, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
  }

  MPI_Bcast(&mid, 1, MPI_INT, 0, MPI_COMM_WORLD);
  int det_ = 256 / size;
  int rem_ = 256 % size;
  std::vector <int> palitra(256);
  float correction = 1.0 + static_cast<float>(correct) / 100;
  std::vector <int> tempA(det_);
  if (rank == 0) {
    MPI_Status status;
    for (int i = 0; i < det_ + rem_; i++) {
      int delta = i - mid;
      palitra[i] = mid - correction * delta;
      if (palitra[i] > 256) {
        palitra[i] = 256;
      } else if (palitra[i] < 0) {
        palitra[i] = 0;
      }
    }
    if (det_ > 0) {
      for (int proc = 1; proc < size; proc++) {
        MPI_Recv(&palitra[rem_ + det_ * proc], det_, MPI_INT, proc, 0, MPI_COMM_WORLD, &status);
      }
    }
  } else if (det_ > 0) {
    for (int i = 0; i < det_; i++) {
      int delta = i + rem_ + det_ * rank - mid;
      tempA[i] = mid - correction * delta;
      if (tempA[i] > 256) {
        tempA[i] = 256;
      }
      if (tempA[i] < 0) {
        tempA[i] = 0;
      }
    }
    MPI_Send(&tempA[0], det_, MPI_INT, 0, 0, MPI_COMM_WORLD);
  }
  MPI_Bcast(&palitra[0], 256, MPI_INT, 0, MPI_COMM_WORLD);
  std::vector <int> tempB(det * cols);
  if (rank == 0) {
    MPI_Status status;
    for (int i = 0; i < (det + rem) * cols; i++) {
      res[i] = palitra[img[i]];
    }
    if (det > 0) {
      for (int proc = 1; proc < size; proc++) {
        MPI_Recv(&res[(det * proc + rem) * cols], det * cols, MPI_INT, proc, 0, MPI_COMM_WORLD, &status);
      }
    }
  } else if (det > 0) {
    for (int i = 0; i < det * cols; i++) {
      tempB[i] = palitra[tmp[i]];
    }
    MPI_Send(&tempB[0], det * cols, MPI_INT, 0, 0, MPI_COMM_WORLD);
  }
  return res;
}
