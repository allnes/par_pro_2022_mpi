// Copyright 2022 Tuzhilkina Polina
#ifndef MODULES_TASK_1_TUZHILKINA_P_VECTOR_AVG_VECTOR_AVG_H_
#define MODULES_TASK_1_TUZHILKINA_P_VECTOR_AVG_VECTOR_AVG_H_

#include <string>
#include <vector>

std::vector<int> getRandomVec(int size);

double getSumSeq(const std::vector<int>& vec);

double getAvgSeq(const std::vector<int>& vec);

double getAvgPar(const std::vector<int>& globVec, int glob_vec_size);

void printVecElements(const std::vector<int>& vec);

#endif  // MODULES_TASK_1_TUZHILKINA_P_VECTOR_AVG_VECTOR_AVG_H_
