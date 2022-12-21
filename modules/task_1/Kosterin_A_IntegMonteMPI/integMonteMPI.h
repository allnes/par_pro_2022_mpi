// Copyright 2018 Nesterov Alexander

#ifndef MODULES_TASK_1_KOSTERIN_A_INTEGMONTEMPI_INTEGMONTEMPI_H_
#define MODULES_TASK_1_KOSTERIN_A_INTEGMONTEMPI_INTEGMONTEMPI_H_

#include <iostream>
#include <stdio.h>
#include <Windows.h>

double monteCarlo(double low, double high, int count, double (*f)(double));
#endif // MODULES_TASK_1_KOSTERIN_A_INTEGMONTEMPI_INTEGMONTEMPI_H_