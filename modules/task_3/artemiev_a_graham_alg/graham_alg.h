// Copyright 2022 Artemiev Aleksey
#ifndef MODULES_TASK_3_ARTEMIEV_A_GRAHAM_ALG_GRAHAM_ALG_H_
#define MODULES_TASK_3_ARTEMIEV_A_GRAHAM_ALG_GRAHAM_ALG_H_

#include <vector>
#include <functional>

std::vector<double> getRandomPoints(int count, double min = 0.0,
                                    double max = 100.0);

void printPoints(const std::vector<double>& points);

void sortPoints(std::vector<double>* points,
                std::function<bool(double, double, double, double)> less);

double rotate(double x1, double y1, double x2, double y2, double x3,
              double y3);

std::vector<double> grahamSequential(std::vector<double> points);
std::vector<double> grahamParallel(std::vector<double> points, int count);

#endif  // MODULES_TASK_3_ARTEMIEV_A_GRAHAM_ALG_GRAHAM_ALG_H_
