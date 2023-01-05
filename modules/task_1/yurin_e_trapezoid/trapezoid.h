// Copyright 2022 Yurin Evgeny
#ifndef MODULES_TASK_1_YURIN_E_TRAPEZOID_TRAPEZOID_H_
#define MODULES_TASK_1_YURIN_E_TRAPEZOID_TRAPEZOID_H_
#include <mpi.h>

#include <functional>

template <typename F>
double trapezoidalIntegral(double a, double b, int n, F f) {
  const double h = (b - a) / n;
  double summ = 0;
  summ += h * ((f(a) + f(b)) / 2);
  for (int step = 1; step < n; step++) {
    const double x = a + (step)*h;
    summ += f(x) * h;
  }
  return summ;
}

template <typename F>
double trapezoidalIntegralParallel(double a, double b, int n, F f) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double globalSumm;

  const double h = (b - a) / n;
  double localSumm = 0;

  for (int step = rank; step < n; step += size) {
    const double x1 = a + step * h;
    const double x2 = a + (step + 1) * h;
    localSumm += 0.5 * h * (f(x1) + f(x2));
  }

  MPI_Reduce(&localSumm, &globalSumm, 1, MPI_DOUBLE, MPI_SUM, 0,
             MPI_COMM_WORLD);

  return globalSumm;
}

#endif  // MODULES_TASK_1_YURIN_E_TRAPEZOID_TRAPEZOID_H_
