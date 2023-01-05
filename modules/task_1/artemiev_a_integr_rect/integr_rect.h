// Copyright 2022 Artemiev Aleksey
#ifndef MODULES_TASK_1_ARTEMIEV_A_INTEGR_RECT_INTEGR_RECT_H_
#define MODULES_TASK_1_ARTEMIEV_A_INTEGR_RECT_INTEGR_RECT_H_

double integrateSequential(double (*f)(double), double a, double b, int n);
double integrateParallel(double (*f)(double), double a, double b, int n);

#endif  // MODULES_TASK_1_ARTEMIEV_A_INTEGR_RECT_INTEGR_RECT_H_
