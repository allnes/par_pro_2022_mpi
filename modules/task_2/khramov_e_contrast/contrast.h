// Copyright 2022 Khramov Egor
#ifndef MODULES_TASK_2_KHRAMOV_E_CONTRAST_CONTRAST_H_
#define MODULES_TASK_2_KHRAMOV_E_CONTRAST_CONTRAST_H_

#include <vector>

void printVector(std::vector<int> vector);

void printArray(int* arr, int n);

std::vector<int> getRandomVector(int size);

std::vector<int> getRandomMatrix(int w, int h);

float getContrastCoeff(int contrast);

int truncate(size_t value);

int* getSendCount(int matrixSize, int commSize);

int* getDispls(int matrixSize, int commSize);

std::vector<int> getContrastedMatrixSequential(std::vector<int> matrix);

std::vector<int> getContrastedMatrixParallel(std::vector<int> matrix, int size);

void testWithSize(int w, int h);

#endif  // MODULES_TASK_2_KHRAMOV_E_CONTRAST_CONTRAST_H_
