// Copyright 2022 Krainev Alexander
#ifndef MODULES_TASK_2_KRAINEV_A_BUBBLE_SORT_ODD_EVEN_H_
#define MODULES_TASK_2_KRAINEV_A_BUBBLE_SORT_ODD_EVEN_H_

#include <vector>

std::vector<int> getRandomVector(int size);
std::vector<int> oddEvenSortSequential(const std::vector<int> &vec);
std::vector<int> oddEvenSortParallel(const std::vector<int> &vec);

#endif  // MODULES_TASK_2_KRAINEV_A_BUBBLE_SORT_ODD_EVEN_H_
