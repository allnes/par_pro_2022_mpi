// Copyright 2022 Tychinin Alexey


#ifndef MODULES_TASK_2_TYCHININ_A_SIMPLE_ITERATION_SIMPLE_ITERATION_H_
#define MODULES_TASK_2_TYCHININ_A_SIMPLE_ITERATION_SIMPLE_ITERATION_H_

#include <vector>

namespace task2 {
    double Norm(const std::vector<double>& b);

    std::vector<double> simpleIterationSeq(std::vector<double> a,
                                           std::vector<double> b,
                                           const double eps);

    std::vector<double> simpleIteration(std::vector<double> a,
                                        std::vector<double> b,
                                        const double eps);
}  // namespace task2


#endif  // MODULES_TASK_2_TYCHININ_A_SIMPLE_ITERATION_SIMPLE_ITERATION_H_
