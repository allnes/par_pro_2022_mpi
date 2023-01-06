// Copyright 2023 Kiselev Vladislav
#ifndef MODULES_TASK_1_KISELEV_V_INTEGR_TRAP_INTEGR_TRAP_H_
#define MODULES_TASK_1_KISELEV_V_INTEGR_TRAP_INTEGR_TRAP_H_

#include <functional>

double integrateSequential(double a, double b, int segQ, std::function<double(double)> fun);
double integrateParallel(double a, double b, int segQ, std::function<double(double)> fun);

#endif  // MODULES_TASK_1_KISELEV_V_INTEGR_TRAP_INTEGR_TRAP_H_
