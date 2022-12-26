// Copyright 2022 Kosterin Alexey
#ifndef MODULES_TASK_2_KOSTERIN_A_GAUS_LENT_HORIZ_GAUS_LENT_HORIZ_H_
#define MODULES_TASK_2_KOSTERIN_A_GAUS_LENT_HORIZ_GAUS_LENT_HORIZ_H_
#include <vector>
std::vector<double> newMatrix(int size);
std::vector<double> getGauss(const std::vector<double> &a, int size);
std::vector<double> getParGauss(const std::vector<double> &a, int size);
bool ChekTrueAnswer(const std::vector<double> &a, int size,
                    const std::vector<double> &x);
#endif  // MODULES_TASK_2_KOSTERIN_A_GAUS_LENT_HORIZ_GAUS_LENT_HORIZ_H_
