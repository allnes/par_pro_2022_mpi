// Copyright 2022 Yurin Evgeny
#ifndef MODULES_TASK_3_YURIN_E_RECTANGLE_MAIN_H_
#define MODULES_TASK_3_YURIN_E_RECTANGLE_MAIN_H_
#include <mpi.h>

template <typename F>
double integrateRectangle(double xmin, double xmax, double ymin, double ymax,
                          int xdiv, int ydiv, F f) {
  double result = 0;

  for (int i = 0; i < xdiv; i++) {
    for (int j = 0; j < ydiv; j++) {
      double xval = xmin + (xmax - xmin) * (i + 0.5) / xdiv;
      double yval = ymin + (ymax - ymin) * (j + 0.5) / ydiv;
      result += f(xval, yval);
    }
  }
  double dx = (xmax - xmin) / xdiv;
  double dy = (ymax - ymin) / ydiv;
  result *= dx * dy;

  return result;
}

template <typename F>
double integrateRectangleParallel(double xmin, double xmax, double ymin,
                                   double ymax, int xdiv, int ydiv, F f) {
  int ProcNum, ProcRank;
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

  double dx = (xmax - xmin) / xdiv;
  double dy = (ymax - ymin) / ydiv;

  int xstart = ProcRank * xdiv / ProcNum;
  int xend = (ProcRank + 1) * xdiv / ProcNum - 1;
  int ystart = 0, yend = ydiv - 1;

  double result = 0;
  for (int i = xstart; i <= xend; i++) {
    for (int j = ystart; j <= yend; j++) {
      double xval = xmin + (xmax - xmin) * (i + 0.5) / xdiv;
      double yval = ymin + (ymax - ymin) * (j + 0.5) / ydiv;
      result += f(xval, yval);
    }
  }
  result *= dx * dy;

  double totalResult;
  MPI_Reduce(&result, &totalResult, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

  if (ProcRank == 0) {
    return totalResult;
  }
  return totalResult;
}
#endif  //  MODULES_TASK_3_YURIN_E_RECTANGLE_MAIN_H_
