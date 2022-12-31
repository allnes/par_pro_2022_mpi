// Copyright 2022 Yashina Anastasia
#ifndef MODULES_TASK_2_YASHINA_A_MULT_MAT_VECT_HOR_MULT_MAT_VECT_HOR_H_
#define MODULES_TASK_2_YASHINA_A_MULT_MAT_VECT_HOR_MULT_MAT_VECT_HOR_H_

#include <mpi.h>
#include <iostream>
#include <random>
#include <vector>

std::vector<int> Vector(int n);
std::vector<int> Matrix(int row, int col);
std::vector<int> ConstVector(int n, int c);
std::vector<int> Multip(const std::vector <int> &matr, int row, int col, const std::vector <int> &vect);

#endif  // MODULES_TASK_2_YASHINA_A_MULT_MAT_VECT_HOR_MULT_MAT_VECT_HOR_H_
