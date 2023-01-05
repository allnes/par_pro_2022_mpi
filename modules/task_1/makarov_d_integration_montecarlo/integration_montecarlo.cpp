// Copyright 2022 Makarov Danila
#include "../../../modules/task_1/makarov_d_integration_montecarlo/integration_montecarlo.h"

#include <mpi.h>

#include <cmath>
#include <ctime>
#include <iostream>
#include <random>

double function1(double x) { return x / 2; }

double function2(double x) { return pow(x, 2) * 0.2; }

double function3(double x) { return sqrt(x); }

double function4(double x) { return sin(x) + 1; }

double notMPIintegration(int N, int a, int b, int h, double (*func)(double)) {
  if (b < a) throw -1;
  if (N == 0) throw -1;

  double x, y = 0;
  double result;
  int cnt = 0;

  std::mt19937 gen(time(0));
  std::uniform_real_distribution<> urdx(static_cast<double>(a),
                                        static_cast<double>(b));
  std::uniform_real_distribution<> urdy(0., static_cast<double>(h));

  for (int i = 0; i < N; i++) {
    x = urdx(gen);
    y = urdy(gen);
    if (y <= func(x)) cnt++;
  }
  result = (cnt / static_cast<double>(N)) * (b - a) * h;
  // std::cout << "Result is " << result << std::endl;
  return result;
}

double MPIintegration(int N, int a, int b, int h, double (*func)(double)) {
  if (b < a) throw -1;

  double tm;
  double y, x, result = 0.;

  int cntl = 0;
  int cntg = 0;

  int ProcNum, ProcRank;
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

  tm = MPI_Wtime();
  MPI_Bcast(&ProcNum, 1, MPI_INT, 0, MPI_COMM_WORLD);

  int Nlocal = N / ProcNum;
  if (ProcRank == 0) Nlocal += N % ProcNum;

  std::mt19937 gen(time(0) + ProcRank);
  std::uniform_real_distribution<> urdx(static_cast<double>(a),
                                        static_cast<double>(b));
  std::uniform_real_distribution<> urdy(0., static_cast<double>(h));

  for (int i = 1; i < Nlocal; i++) {
    x = urdx(gen);
    y = urdy(gen);
    if (y <= func(x)) cntl++;
  }

  // std::cout << "Local cnt " << ProcRank << " is " << cntl << std::endl;
  MPI_Reduce(&cntl, &cntg, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);

  if (ProcRank == 0) {
    result = (cntg / static_cast<double>(N)) * (b - a) * h;
    // std::cout << "Global cnt is " << cntg << std::endl;
    // std::cout << "Time is " << MPI_Wtime() - tm << std::endl;
    // std::cout << "MPI Result is " << result << std::endl;
  }

  return result;
}
