// Copyright 2022 Zorin Oleg
#ifndef MODULES_TASK_3_ZORIN_O_MOORE_ALG_MOORE_ALG_H_
#define MODULES_TASK_3_ZORIN_O_MOORE_ALG_MOORE_ALG_H_

#include <vector>

#define INF 999
using Vector = std::vector<int>;
using Matrix = std::vector<Vector>;

Vector expandVector(const Vector& V, int shift);
Matrix expandMatrix(const Matrix& M, int shift);
Vector MooreAlgSequential(const Matrix& weight_matrix, int start);
Vector MooreAlgParallel(const Matrix& weight_matrix, int start);

#endif  // MODULES_TASK_3_ZORIN_O_MOORE_ALG_MOORE_ALG_H_
