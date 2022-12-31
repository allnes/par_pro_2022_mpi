// Copyright 2022 Frantsuzov Maksim
#include <mpi.h>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <cmath>
#include <iostream>
#include "../../../modules/task_3/frantsuzov_m_simpson_method/simpson_method.h"






double f(double x, double y) {
  return x * x + y * y;
}

double simpson_method(int number_of_points_in_the_grid, const double x_min,
                      const double x_max, const double y_min,
                      const double y_max) {
  const double x_step = (x_max - x_min) / number_of_points_in_the_grid;
  const double y_step = (y_max - y_min) / number_of_points_in_the_grid;
  double integral = 0.0;
  for (int i = 0; i < number_of_points_in_the_grid; i++) {
    for (int j = 0; j < number_of_points_in_the_grid; j++) {
      double x = x_min + i * x_step;
      double y = y_min + j * y_step;


      double weight = (i % 2 == 0) ? 2.0 : 4.0;
      weight = (j % 2 == 0) ? weight : weight * 4.0;
      integral += weight * f(x, y);
    }
  }


  integral *= (x_step * y_step) / (9.0 * pow(number_of_points_in_the_grid, 2));


  return integral;
}


double simposon_method_with_MPI(int number_of_points_in_the_grid,
                                const double x_min, const double x_max,
                                const double y_min, const double y_max) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);


  const int points_per_process = number_of_points_in_the_grid / sqrt(size);
  const double x_step = (x_max - x_min) / (points_per_process * size);
  const double y_step = (y_max - y_min) / (points_per_process * size);


  double partial_integral = 0.0;


  for (int i = rank * points_per_process; i < (rank + 1) * points_per_process;
       i++) {
    for (int j = rank * points_per_process; j < (rank + 1) * points_per_process;
      j++) {
      double x = x_min + i * x_step;
      double y = y_min + j * y_step;


      double weight = (i % 2 == 0) ? 2.0 : 4.0;
      weight = (j % 2 == 0) ? weight : weight * 4.0;
      partial_integral += weight * f(x, y);
    }
  }


  partial_integral *= (x_step * y_step) / (9.0 * pow(points_per_process, 2));


  double integral = 0.0;


  MPI_Reduce(&partial_integral, &integral, 1, MPI_DOUBLE, MPI_SUM, 0,
             MPI_COMM_WORLD);

  return integral;
}
