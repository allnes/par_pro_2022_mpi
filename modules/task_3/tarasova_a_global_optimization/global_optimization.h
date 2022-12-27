// Copyright 2022 Tarasova Anastasia
#ifndef MODULES_TASK_3_TARASOVA_A_GLOBAL_OPTIMIZATION_GLOBAL_OPTIMIZATION_H_
#define MODULES_TASK_3_TARASOVA_A_GLOBAL_OPTIMIZATION_GLOBAL_OPTIMIZATION_H_
#include <mpi.h>
#include <algorithm>
#include <cmath>
#include <functional>
#include <vector>

double GetGlobalOpt(const double a, const double b, std::function<double(double*)> func,
    const int part, const double E);
double GetGlobalOptParallel(const double a, const double b, std::function<double(double*)> func,
    const int part, const double E);

#endif  // MODULES_TASK_3_TARASOVA_A_GLOBAL_OPTIMIZATION_GLOBAL_OPTIMIZATION_H_
