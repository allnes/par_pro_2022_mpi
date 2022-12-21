// Copyright 2022 Zorin Oleg
#ifndef MODULES_TASK_2_ZORIN_O_JACOBI_JACOBI_H_
#define MODULES_TASK_2_ZORIN_O_JACOBI_JACOBI_H_

#include <vector>

using Vector = std::vector<double>;
using Matrix = std::vector<Vector>;

Vector operator-(const Vector &X, const Vector &Y);

double normVector(const Vector &X);

Vector expandVector(const Vector &V, std::size_t shift);

Matrix expandMatrix(const Matrix &M, std::size_t shift);

Vector JacobiParallel(Matrix A, Vector B);

Vector JacobiSequential(const Matrix& A, const Vector& B);


#endif  // MODULES_TASK_2_ZORIN_O_JACOBI_JACOBI_H_
