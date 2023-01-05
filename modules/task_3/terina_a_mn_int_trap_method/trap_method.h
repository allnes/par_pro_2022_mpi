// Copyright 2022 Terina Alina
#ifndef MODULES_TASK_3_TERINA_A_MN_INT_TRAP_METHOD_TRAP_METHOD_H_
#define MODULES_TASK_3_TERINA_A_MN_INT_TRAP_METHOD_TRAP_METHOD_H_
#include <mpi.h>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

int getLocalSize(int rank, int count_sz, int remain);
double First(std::vector<double> coefs);
double Second(std::vector<double> coefs);
std::vector<double> getRandomVector(int sz);
double ParInt(std::vector<double> first_c, std::vector<double> second_c,
    double (*PodIntF)(std::vector<double>), int n);
double OrdinaryInt(std::vector<double> first_c, std::vector<double> second_c,
    double (*PodIntF)(std::vector<double>), int n);

#endif  //  MODULES_TASK_3_TERINA_A_MN_INT_TRAP_METHOD_TRAP_METHOD_H_
