// Copyright 2022 Makarov Danila
#ifndef MODULES_TASK_1_MAKAROV_D_INTEGRATION_MONTECARLO_INTEGRATION_MONTECARLO_H_
#define MODULES_TASK_1_MAKAROV_D_INTEGRATION_MONTECARLO_INTEGRATION_MONTECARLO_H_

double function1(double x);
double function2(double x);
double function3(double x);
double function4(double x);

double notMPIintegration(int N, int a, int b, int h, double (*func)(double));
double MPIintegration(int N, int a, int b, int h, double (*func)(double));

#endif  // MODULES_TASK_1_MAKAROV_D_INTEGRATION_MONTECARLO_INTEGRATION_MONTECARLO_H_
