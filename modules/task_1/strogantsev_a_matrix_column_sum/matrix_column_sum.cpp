// Copyright 2022 Strogantsev Anton
#include <mpi.h>
#include <vector>
#include <random>
#include <algorithm>
#include "../../../modules/task_1/strogantsev_a_matrix_column_sum/matrix_column_sum.h"

std::vector<int> generateRandomMatrix(int xSize, int ySize) {
    std::mt19937 gen(10);
    int size = xSize * ySize;

    std::vector<int> matrix(size);
    for (int  i = 0; i < size; i++)
        matrix[i] = gen() % 100;

    return matrix;
}

int linearizeCoordinates(int x, int y, int xSize) {
    return y * xSize + x;
}

std::vector<int> calculateMatrixSumSequentially(
    const std::vector<int>& matrix,
    int xSize,
    int ySize,
    int fromX,
    int toX
) {
    std::vector<int> result;
    for (int x = fromX; x < toX; x++) {
        int columnSum = 0;
        for (int y = 0; y < ySize; y++) {
            int linearizedCoordinate = linearizeCoordinates(x, y, xSize);
            columnSum += matrix[linearizedCoordinate];
        }
        result.push_back(columnSum);
    }
    return result;
}

std::vector<int> calculateMatrixSumSequentially(const std::vector<int>& matrix, int xSize, int ySize) {
    return calculateMatrixSumSequentially(matrix, xSize, ySize, 0, xSize);
}

std::vector<int> calculateMatrixSumParallel(const std::vector<int>& matrix, int xSize, int ySize) {
    if (xSize == 0 || ySize == 0) return std::vector<int>();

    int workersCount, currentWorkerRank;
    MPI_Comm_size(MPI_COMM_WORLD, &workersCount);
    MPI_Comm_rank(MPI_COMM_WORLD, &currentWorkerRank);

    int delta = xSize / workersCount;
    delta += (xSize % workersCount == 0) ? 0 : 1;

    int matrixElementsCount = xSize * ySize;
    std::vector<int> localMatrix(matrixElementsCount);

    if (currentWorkerRank == 0) {
        localMatrix = matrix;
        MPI_Bcast(localMatrix.data(), matrixElementsCount, MPI_INT, 0, MPI_COMM_WORLD);
    } else {
        MPI_Bcast(localMatrix.data(), matrixElementsCount, MPI_INT, 0, MPI_COMM_WORLD);
    }

    const int lastColumn = std::min(xSize, delta * (currentWorkerRank + 1));
    auto localSum = calculateMatrixSumSequentially(localMatrix, xSize, ySize, delta * currentWorkerRank, lastColumn);
    localSum.resize(delta);

    if (currentWorkerRank == 0) {
        std::vector<int> globalSum(xSize + delta * workersCount);
        MPI_Gather(localSum.data(), localSum.size(), MPI_INT, globalSum.data(), delta, MPI_INT, 0, MPI_COMM_WORLD);
        globalSum.resize(xSize);
        return globalSum;
    } else {
        MPI_Gather(localSum.data(), localSum.size(), MPI_INT, 0, 0, 0, 0, MPI_COMM_WORLD);
        return std::vector<int>();
    }
}
