// Copyright 2022 Khairetdinov Timur
#include <mpi.h>
#include <vector>
#include <iostream>
#include <random>
#include <ctime>
#include <numeric>
#include <algorithm>
#include <stdexcept>
#include <utility>
#include "../../../modules/task_3/khairetdinov_t_rectangle_integ/rectang_integ.h"

using std::vector;
using std::pair;

double RectParall(double (*func)(vector<double>), vector <pair<double, double>> cordinate, const int leN) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int n = cordinate.size();
  vector<double> step(n);
  vector <pair<double, double>> coord(n);
  int allElem;

  if (rank == 0) {
    allElem = 1;
    for (int i = 0; i < n; ++i) {
      step[i] = (cordinate[i].second - cordinate[i].first) / leN;
      coord[i] = cordinate[i];
    }
    int r = 0;
    while (r != n - 1) {
      allElem *= leN;
      r++;
    }
  }
  int ln = leN;
  int root = 0;
  MPI_Bcast(&allElem, 1, MPI_INT, root, MPI_COMM_WORLD);
  MPI_Bcast(&ln, n, MPI_INT, root, MPI_COMM_WORLD);
  MPI_Bcast(&step[0], n, MPI_DOUBLE, root, MPI_COMM_WORLD);
  MPI_Bcast(&coord[0], 2 * n, MPI_DOUBLE, root, MPI_COMM_WORLD);

  int ost = allElem % size;
  int count = allElem / size;
  if (rank < ost) {
    count += 1;
  }
  int temp = 0;
  if (rank < ost) {
    temp = rank * count;
  } else {
    temp = ost + rank * count;
  }

  vector<vector<double>> answVec(count);
  for (int i = 0; i < count; ++i) {
    int number = temp + i;
    for (int j = 0; j < n - 1; ++j) {
      answVec[i].push_back(coord[j].first + (number % leN + 0.5) * step[j]);
    }
  }
  double result = 0.0;
  for (int i = 0; i < count; ++i) {
    for (int j = 0; j < leN; ++j) {
      answVec[i].push_back(coord[n - 1].first + (j + 0.5) * step[n - 1]);
      result += func(answVec[i]);
      answVec[i].pop_back();
    }
  }
  double totalInteg = 0.0;
  MPI_Reduce(&result, &totalInteg, 1, MPI_DOUBLE, MPI_SUM, root, MPI_COMM_WORLD);
  for (int i = 0; i < n; ++i) {
    totalInteg *= step[i];
  }
  return totalInteg;
}


double RectSequen(double (*func)(vector<double>), vector <pair<double, double>> cordinate, const int leN) {
  int  membs = 1;
  int razmer = cordinate.size();
  vector<double> h(razmer);
  for (int i = 0; i < razmer; ++i) {
    h[i] = (cordinate[i].second - cordinate[i].first) / leN;
    membs = membs * leN;
  }
  vector <double> answVec(razmer);
  double result = 0.0;
  for (int i = 0; i < membs; ++i) {
    for (int j = 0; j < razmer; ++j) {
      answVec[j] = cordinate[j].first + (i % leN + 0.5) * h[j];
    }
    result += func(answVec);
  }
  for (int i = 0; i < razmer; ++i) {
    result *= h[i];
  }
  return result;
}
