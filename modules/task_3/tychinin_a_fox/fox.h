// Copyright 2022 Tychinin Alexey


#ifndef MODULES_TASK_3_TYCHININ_A_FOX_FOX_H_
#define MODULES_TASK_3_TYCHININ_A_FOX_FOX_H_

#include <vector>

namespace task3 {
    std::vector<double> generateMatrix(size_t size);

    std::vector<double> simpleMult(const std::vector<double>& a,
                                   const std::vector<double>& b,
                                   size_t size);

    std::vector<double> foxMult(std::vector<double> a,
                                std::vector<double> b,
                                size_t data);
}  // namespace task3


#endif  //  MODULES_TASK_3_TYCHININ_A_FOX_FOX_H_
