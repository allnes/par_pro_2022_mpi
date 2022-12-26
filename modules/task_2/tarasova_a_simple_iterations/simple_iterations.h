// Copyright 2022 Tarasova Anastasia

#ifndef MODULES_TASK_2_TARASOVA_A_SIMPLE_ITERATIONS_SIMPLE_ITERATIONS_H_
#define MODULES_TASK_2_TARASOVA_A_SIMPLE_ITERATIONS_SIMPLE_ITERATIONS_H_

#include <mpi.h>
#include <random>
#include <vector>

std::vector<double> CreateVector(const int size, const int min, const int max);
std::vector<std::vector<double>> CreateMatrix(const int size);
std::vector<double> GetSimpleIter(std::vector<std::vector<double>>& a, std::vector<double>& b, const int size, const double eps);
std::vector<double> GetSimpleIterParallel(std::vector<std::vector<double>>& a, std::vector<double>& b, const int size, const double eps);


#endif  // MODULES_TASK_2_TARASOVA_A_SIMPLE_ITERATIONS_SIMPLE_ITERATIONS_H_
