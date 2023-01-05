// Copyright 2023 Pankov Anatoliy
#ifndef MODULES_TASK_3_PANKOV_A_JARVIS_JARVIS_H_
#define MODULES_TASK_3_PANKOV_A_JARVIS_JARVIS_H_

#include <functional>
#include <vector>

void printDoubleVec(const std::vector<double>& vec);

std::vector<double> createRandomPoints(double xyMin, double xyMax, int p_count);

std::vector<double> jarvisSequential(std::vector<double> points);
std::vector<double> jarvisParallel(std::vector<double> points, int p_count);

#endif  // MODULES_TASK_3_PANKOV_A_JARVIS_JARVIS_H_
