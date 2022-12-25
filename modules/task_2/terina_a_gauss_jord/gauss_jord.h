// Copyright 2022 Terina Alina
#include <string>
#include <vector>

#pragma once
std::vector<std::vector<double>> getRandomMatrix(std::vector<double> vec,
    int matrix_size);
int getParallelOperations(std::vector<int> global_vec, int count_size_vector,
    const std::string& ops);
std::vector<double> ordinaryGJ(std::vector<std::vector<double>> matrcoefs,
    std::vector<double> rightpart);
std::vector<double> getRandomRight(std::vector<double> vec, int matrix_size);
void carve_up(int matrix_size, int part, double* str, double* subtrahend);
std::vector<double> ParGJ(std::vector<double> str, int matrix_size);
int evaluateforScatterv(int num_parts, int part_size, int it);
std::vector<double> matrix_random(std::vector<double> num_vec, int matrix_size);
