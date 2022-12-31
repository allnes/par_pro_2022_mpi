// Copyright 2022 Frantsuzov Maksim
#ifndef MODULES_TASK_3_FRANTSUZOV_M_SIMPSON_METHOD_SIMPSON_METHOD_H_
#define MODULES_TASK_3_FRANTSUZOV_M_SIMPSON_METHOD_SIMPSON_METHOD_H_

#include <vector>
#include <string>

double f(double x, double y);
double simpson_method(int number_of_points_in_the_grid,
                                const double x_min, const double x_max,
                                const double y_min, const double y_max);
double simposon_method_with_MPI(int numberOfPointsInTheGrild,
                                const double x_min, const double x_max,
                                const double y_min, const double y_max);

#endif  // MODULES_TASK_3_FRANTSUZOV_M_SIMPSON_METHOD_SIMPSON_METHOD_H_
