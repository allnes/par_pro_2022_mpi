// Copyright 2022 Kosterin Alexey
#include "../../../modules/task_2/kosterin_a_gaus_lent_horiz/gaus_lent_horiz.h"
#include <mpi.h>
#include <algorithm>
#include <cmath>
#include <random>
#include <stdexcept>
#include <vector>

std::vector<double> newMatrix(int size) {
  std::random_device dev;
  std::mt19937 gen(dev());
  std::vector<double> arr(size * (size + 1));
  for (int i = 0; i < size * (size + 1); i++) {
    arr[i] = gen() % 10;
  }
  return arr;
}

std::vector<double> getGauss(const std::vector<double> &a, int size) {
  std::vector<double> res(size);
  std::vector<double> tmp(a);
  double m;
  for (int k = 0; k < size; k++) {
    for (int j = k; j < size; j++) {
      m = tmp[j * (size + 1) + k];
      for (int i = 0; i < size + 1; i++) {
        tmp[j * (size + 1) + i] /= m;
      }
      if (j != k) {
        for (int i = 0; i < size + 1; i++) {
          tmp[j * (size + 1) + i] =
              tmp[j * (size + 1) + i] - tmp[k * (size + 1) + i];
        }
      }
    }
  }
  for (int i = size - 1; i >= 0; i--) {
    res[i] = tmp[i * (size + 1) + size];
    for (int c = 0; c < i; c++) {
      tmp[c * (size + 1) + size] -= tmp[c * (size + 1) + i] * res[i];
    }
  }
  return res;
}

bool ChekTrueAnswer(const std::vector<double> &a, int size,
                    const std::vector<double> &x) {
  std::vector<double> res(size);
  double e = 0.5;
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      res[i] += a[(size + 1) * i + j] * x[j];
    }
  }
  for (int i = 0; i < size; i++) {
    if (std::abs(res[i] - a[i * (size + 1) + size]) > e) {
      return false;
    }
  }
  return true;
}

std::vector<double> getParGauss(const std::vector<double> &a, int size) {
  int sizeProc, rank;
  std::vector<double> vec(size + 1);
  MPI_Comm_size(MPI_COMM_WORLD, &sizeProc);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int delta = size / sizeProc;
  int ost = size % sizeProc;
  int tmp = (delta + (rank < ost ? 1 : 0));
  std::vector<double> temp(tmp * (size + 1));
  if (rank == 0) {
    for (int proc = sizeProc - 1; proc >= 0; proc--) {
      int index = 0;
      for (int i = proc; i < size; i += sizeProc) {
        for (int j = 0; j < size + 1; j++) {
          temp[index++] = a[j + i * (size + 1)];
        }
      }
      if (proc > 0) {
        MPI_Send(temp.data(), index, MPI_DOUBLE, proc, 1, MPI_COMM_WORLD);
      }
    }
  } else {
    MPI_Status status;
    MPI_Recv(temp.data(), temp.size(), MPI_DOUBLE, 0, 1, MPI_COMM_WORLD,
             &status);
  }
  int sizePlus = size + 1;
  std::vector<int> tapesCounts(sizeProc);
  std::vector<int> displ(sizeProc);
  std::vector<double> rowVec(sizePlus);
  displ[0] = 0;
  for (int i = 0; i < sizeProc; i++) {
    tapesCounts[i] = (delta + (i < ost ? 1 : 0)) * sizePlus;
    if (i > 0) {
      displ[i] = (displ[i - 1] + tapesCounts[i - 1]);
    }
  }
  for (int i = 0; i < displ[rank] / sizePlus; i++) {
    int root = 0;
    int sum = 0;
    for (int j = 0; j < sizeProc; j++, ++root) {
      sum += tapesCounts[j] / sizePlus;
      if (i < sum) {
        root = j;
        break;
      }
    }
    MPI_Bcast(rowVec.data(), sizePlus, MPI_DOUBLE, root, MPI_COMM_WORLD);
    for (int j = 0; j < tapesCounts[rank] / sizePlus; j++) {
      double s = rowVec[i] / temp[j * sizePlus + i];
      for (int k = i; k < sizePlus; k++) {
        temp[j * sizePlus + k] = s * temp[j * sizePlus + k] - rowVec[k];
      }
    }
  }
  for (int i = 0; i < tapesCounts[rank] / sizePlus; ++i) {
    for (int j = 0; j < sizePlus; j++) {
      rowVec[j] = temp[i * sizePlus + j];
    }
    MPI_Bcast(rowVec.data(), sizePlus, MPI_DOUBLE, rank, MPI_COMM_WORLD);
    for (int j = i + 1; j < tapesCounts[rank] / sizePlus; j++) {
      double s = rowVec[displ[rank] / sizePlus + i] /
                 temp[j * sizePlus + i + displ[rank] / sizePlus];
      for (int k = i + displ[rank] / sizePlus; k < sizePlus; k++) {
        temp[j * sizePlus + k] = s * temp[j * sizePlus + k] - rowVec[k];
      }
    }
  }
  int root = 0;
  for (int i = displ[rank] / sizePlus + tapesCounts[rank] / sizePlus; i < size;
       i++) {
    int sum = 0;
    for (int j = 0; j < sizeProc; j++, ++root) {
      sum += tapesCounts[j] / sizePlus;
      if (i < sum) {
        root = j;
        break;
      }
    }
    MPI_Bcast(rowVec.data(), sizePlus, MPI_DOUBLE, root, MPI_COMM_WORLD);
  }
  std::vector<double> tempRes(0);
  if (rank == 0)
    tempRes.resize(size * sizePlus);
  MPI_Gatherv(temp.data(), tmp * sizePlus, MPI_DOUBLE, tempRes.data(),
              tapesCounts.data(), displ.data(), MPI_DOUBLE, 0, MPI_COMM_WORLD);
  std::vector<double> finResult(0);
  if (rank == 0) {
    finResult.resize(size);
    for (int i = size - 1; i >= 0; i--) {
      double b = tempRes[i * sizePlus + sizePlus - 1];
      for (int j = size - 1; j >= i + 1; j--) {
        b -= tempRes[i * sizePlus + j] * finResult[j];
      }
      finResult[i] = b / tempRes[i * sizePlus + i];
    }
  }

  return finResult;
}
