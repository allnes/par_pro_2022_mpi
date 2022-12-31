// Copyright Tarasova Anastasia 2022
#ifndef MODULES_TASK_3_TARASOVA_A_GLOBAL_OPTIMIZATION_GLOBAL_OPTIMIZATION_H_
#define MODULES_TASK_3_TARASOVA_A_GLOBAL_OPTIMIZATION_GLOBAL_OPTIMIZATION_H_
#include <mpi.h>
#include <algorithm>
#include <cmath>
#include <functional>
#include <vector>

double GetGlobalOpt(const double a, const double b, std::function<double(double*)> func,
    const int part, const double e);
double GetGlobalOptParallel(const double a, const double b, std::function<double(double*)> func,
    const int part, const double e);

#endif  // MODULES_TASK_3_TARASOVA_A_GLOBAL_OPTIMIZATION_GLOBAL_OPTIMIZATION_H_
