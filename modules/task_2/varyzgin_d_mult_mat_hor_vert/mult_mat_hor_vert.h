// Copyright 2023 Varyzgin Dmitriy
#ifndef MODULES_TASK_2_VARYZGIN_D_MULT_MAT_HOR_VERT_MULT_MAT_HOR_VERT_H_
#define MODULES_TASK_2_VARYZGIN_D_MULT_MAT_HOR_VERT_MULT_MAT_HOR_VERT_H_

#include <mpi.h>

#include <iostream>
#include <random>
#include <vector>

std::vector<int> genRandomMatrix(int m, int n);
std::vector<int> transpose(const std::vector<int>& B, int k);
std::vector<int> multMat(const std::vector<int>& A, const std::vector<int>& B,
                         int n);
std::vector<int> LocToGlob(const std::vector<int>& globRes, int k, int start,
                           const std::vector<int>& locRes, int x, int y);
std::vector<int> multMatParallel(const std::vector<int>& A,
                                 const std::vector<int>& B, int n);

#endif  // MODULES_TASK_2_VARYZGIN_D_MULT_MAT_HOR_VERT_MULT_MAT_HOR_VERT_H_
