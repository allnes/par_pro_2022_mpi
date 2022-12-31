// Copyright 2022 Mikerin Ilya
#ifndef MODULES_TASK_2_MIKERIN_I_CONTRAST_CONTRAST_H_
#define MODULES_TASK_2_MIKERIN_I_CONTRAST_CONTRAST_H_


#include <vector>
#include <string>

std::vector<int> getRandomVector(int  sz);
std::vector<std::vector<int>> ParallelContrast(std::vector<std::vector<int>> vec, int weight, int height);
std::vector<std::vector<int>> SequentialContrast(std::vector<std::vector<int>> vec, int weight, int height);

#endif  // MODULES_TASK_2_MIKERIN_I_CONTRAST_CONTRAST_H_
