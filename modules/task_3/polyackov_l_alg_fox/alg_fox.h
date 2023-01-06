// Copyright 2022 Polyackov Lev
#ifndef MODULES_TASK_3_POLYACKOV_L_ALG_FOX_ALG_FOX_H_
#define MODULES_TASK_3_POLYACKOV_L_ALG_FOX_ALG_FOX_H_

#include <mpi.h>
#include <cmath>
#include <ctime>
#include <random>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

std::vector<double> getRandMatrix(int size);
void getResOfMatrMult(double* first_matr, double* second_matr, double* res_matr, int size);
void updateMatrToNewSize(std::vector<double>* matr, int old_size, int new_size);
void switchMatrixStorageType(std::vector<double>* main_vec, int block_size, int block_in_row);
std::vector<double> getResOfFoxMult(std::vector<double> first_matr, std::vector<double> second_matr, int matr_size);
void PrintMatr(std::vector<double> vec, int size);

#endif  // MODULES_TASK_3_POLYACKOV_L_ALG_FOX_ALG_FOX_H_
