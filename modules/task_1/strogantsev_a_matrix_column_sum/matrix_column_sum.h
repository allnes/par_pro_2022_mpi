// Copyright 2022 Strogantsev Anton
#ifndef MODULES_TASK_1_STROGANTSEV_A_MATRIX_COLUMN_SUM_MATRIX_COLUMN_SUM_H_
#define MODULES_TASK_1_STROGANTSEV_A_MATRIX_COLUMN_SUM_MATRIX_COLUMN_SUM_H_

#include <vector>

std::vector<int> generateRandomMatrix(int xSize, int ySize);
int linearizeCoordinates(int x, int y, int xSize);

std::vector<int> calculateMatrixSumParallel(const std::vector<int>& matrix, int xSize, int ySize);
std::vector<int> calculateMatrixSumSequentially(const std::vector<int>& matrix, int xSize, int ySize);
std::vector<int> calculateMatrixSumSequentially(
    const std::vector<int>& matrix,
    int xSize,
    int ySize,
    int fromX,
    int toX
);

#endif  // MODULES_TASK_1_STROGANTSEV_A_MATRIX_COLUMN_SUM_MATRIX_COLUMN_SUM_H_
