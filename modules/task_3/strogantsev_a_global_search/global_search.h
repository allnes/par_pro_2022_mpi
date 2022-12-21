// Copyright 2022 Strogantsev Anton
#ifndef MODULES_TASK_3_STROGANTSEV_A_GLOBAL_SEARCH_GLOBAL_SEARCH_H_
#define MODULES_TASK_3_STROGANTSEV_A_GLOBAL_SEARCH_GLOBAL_SEARCH_H_

#include <functional>
#include <utility>
#include <vector>

namespace TAG {
    const int FINDED = 1;
    const int UNFINDED = 2;
}

const double parameterR = 2.0;

using RAndIndex = std::pair<double, int>;

double getMiddle(double a, double b);
double getParameterM(const std::vector<double>& points, std::function<double(double)> fun);
double getParameterLipschitz(double parameterM, double parameterR);
std::vector<RAndIndex> getParametersR(
    const std::vector<double>& points,
    std::function<double(double)> fun,
    double parameter_m
);
double getNextX(double point0, double point1, std::function<double(double)> fun, double parameter_m);
double getNextX(
    const std::vector<double>& points,
    std::function<double(double)> fun,
    int indexOfMaxR,
    double parameter_m
);

double globalSearchParallel(std::function<double(double)> fun, double a, double b, double epsilone);
double globalSearchSequentially(std::function<double(double)> fun, double a, double b, double epsilone);

#endif  // MODULES_TASK_3_STROGANTSEV_A_GLOBAL_SEARCH_GLOBAL_SEARCH_H_
