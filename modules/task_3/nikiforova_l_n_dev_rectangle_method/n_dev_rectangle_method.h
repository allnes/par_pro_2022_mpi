// Copyright 2022 Nikiforova Lada
#ifndef MODULES_TASK_3_NIKIFOROVA_L_N_DEV_RECTANGLE_METHOD_N_DEV_RECTANGLE_METHOD_H_
#define MODULES_TASK_3_NIKIFOROVA_L_N_DEV_RECTANGLE_METHOD_N_DEV_RECTANGLE_METHOD_H_

#include <vector>
#include <functional>

double parallelIntegrall(const std::vector<double>& _ADots,
                         const std::vector<double>& _BDots,
                         std::function<double(std::vector<double>*)> _foo,
                         double _eps);

#endif  // MODULES_TASK_3_NIKIFOROVA_L_N_DEV_RECTANGLE_METHOD_N_DEV_RECTANGLE_METHOD_H_
