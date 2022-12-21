// Copyright 2022 Churkin Alexander
#ifndef MODULES_TASK_3_CHURKIN_A_GLOB_SEARCH_GLOB_SEARCH_H_
#define MODULES_TASK_3_CHURKIN_A_GLOB_SEARCH_GLOB_SEARCH_H_

#include <functional>
#include <utility>
#include <vector>

#define DOUBLE_MIN (std::numeric_limits<double>::min())
#define DOUBLE_MAX (std::numeric_limits<double>::max())
const double r = 2.0;
const int FOUND = 1;
const int NOT_FOUND = 0;

double compute_M(const std::vector<double>& y, std::function<double(double)> f);

std::vector<std::pair<double, int>> compute_R(const std::vector<double>& y,
                                std::function<double(double)> f, double m);

double compute_x(double y0, double y1, std::function<double(double)> f,
                 double m);

double globalSearchOneDimSequential(std::function<double(double)> f, double a,
                                    double b, double epsilon);

double globalSearchOneDimParallel(std::function<double(double)> f, double a,
                                  double b, double epsilon);

#endif  // MODULES_TASK_3_CHURKIN_A_GLOB_SEARCH_GLOB_SEARCH_H_
