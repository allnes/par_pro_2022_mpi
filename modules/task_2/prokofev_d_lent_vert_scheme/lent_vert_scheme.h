  // Copyright 2022 Prokofev Denis
#ifndef MODULES_TASK_2_PROKOFEV_D_LENT_VERT_SCHEME_LENT_VERT_SCHEME_H_
#define MODULES_TASK_2_PROKOFEV_D_LENT_VERT_SCHEME_LENT_VERT_SCHEME_H_
#include <mpi.h>
#include <vector>
#include <iostream>
#include <random>
std::vector<int> genMatr(int rows, int cols);
std::vector<int> genVect(int rows);
std::vector<int> lentVertScheme(const std::vector<int>& mat,
    const std::vector<int>& vect, const size_t rows, const size_t cols);
#endif  // MODULES_TASK_2_PROKOFEV_D_LENT_VERT_SCHEME_LENT_VERT_SCHEME_H_
