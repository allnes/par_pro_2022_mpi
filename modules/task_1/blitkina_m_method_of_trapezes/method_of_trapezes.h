// Copyright 2022 Blitkina Maria
#ifndef MODULES_TASK_1_BLITKINA_M_METHOD_OF_TRAPEZES_METHOD_OF_TRAPEZES_H_
#define MODULES_TASK_1_BLITKINA_M_METHOD_OF_TRAPEZES_METHOD_OF_TRAPEZES_H_

#include <mpi.h>
#include <math.h>
#include <iostream>
using std::cout;
using std::endl;


double getIntegral(int nIntervals, double(*f)(double), double a, double b);


#endif  // MODULES_TASK_1_BLITKINA_M_METHOD_OF_TRAPEZES_METHOD_OF_TRAPEZES_H_
