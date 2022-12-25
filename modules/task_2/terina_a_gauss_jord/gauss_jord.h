// Copyright 2022 Terina Alina
#include <string>
#include <vector>

#ifndef MODULES_TASK_2_TERINA_A_GAUSS_JORD_GAUSS_JORD_H_
#define MODULES_TASK_2_TERINA_A_GAUSS_JORD_GAUSS_JORD_H_

std::vector<std::vector<double>> getRandomMatrix(std::vector<double> vec,
    int matrix_size);
std::vector<double> ordinaryGJ(std::vector<std::vector<double>> matrcoefs,
    std::vector<double> rightpart);
std::vector<double> getRandomRight(std::vector<double> vec, int matrix_size);
void carve_up(int matrix_size, int part, double* str, double* sendvec);
std::vector<double> ParGJ(std::vector<double> str, int matrix_size);
int evaluateforScatterv(int num_parts, int part_size, int it);
std::vector<double> matrix_random(std::vector<double> num_vec, int matrix_size);

#endif  // MODULES_TASK_2_TERINA_A_GAUSS_JORD_GAUSS_JORD_H_
