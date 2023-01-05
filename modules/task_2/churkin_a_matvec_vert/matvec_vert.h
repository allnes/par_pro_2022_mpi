// Copyright 2022 Churkin Alexander
#ifndef MODULES_TASK_2_CHURKIN_A_MATVEC_VERT_MATVEC_VERT_H_
#define MODULES_TASK_2_CHURKIN_A_MATVEC_VERT_MATVEC_VERT_H_

#include <vector>
#include <string>

// Creates random int vector of size n
std::vector<int> getRandomVector(int n);

// Creates random matrix of size m x n
std::vector<int> getRandomMatrix(int m, int n);

// Prints vector as usual
void printVector(const std::vector<int>& vec);

// Prints matrix (for matrices stored as columns one by one (!))
void printMatrix(const std::vector<int>& matrix, int m, int n);

// Sequential algorithm
std::vector<int> getMultSequential(const std::vector<int>& matrix, const std::vector<int>& vec, int m, int n);

// Parallel algorithm
std::vector<int> getMultParallel(const std::vector<int>& matrix, const std::vector<int>& vec, int m, int n);

#endif  // MODULES_TASK_2_CHURKIN_A_MATVEC_VERT_MATVEC_VERT_H_
