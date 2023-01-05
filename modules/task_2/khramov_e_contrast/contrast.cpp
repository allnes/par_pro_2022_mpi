// Copyright 2022 Khramov Egor

#include <mpi.h>
#include <iostream>
#include <algorithm>
#include <random>
#include <vector>
#include "../../../modules/task_2/khramov_e_contrast/contrast.h"

void printVector(std::vector<int> vector) {
    for (int i = 0; i < vector.size(); i++) {
        std::cout << vector[i] << " ";
    }
    std::cout << std::endl;
}

void printArray(int* arr, int n) {
    for (int i = 0; i < n; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}

std::vector<int> getRandomVector(int size) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::vector<int> vec(size);
    for (int i = 0; i < size; i++) {
        vec[i] = gen() % 255;
    }
    return vec;
}

std::vector<int> getRandomMatrix(int w, int h) {
    return getRandomVector(w * h);
}

float getContrastCoeff(int contrast) {
    return (259.0 * (contrast + 255.0)) / (255.0 * (259.0 - contrast));
}

int truncate(size_t value) {
    if (value < 0) return 0;
    if (value > 255) return 255;

    return value;
}

std::vector<int> getContrastedMatrixSequential(std::vector<int> matrix) {
    int length = matrix.size();

    std::vector<int> result(length);

    float coeff = getContrastCoeff(10);

    for (int i = 0; i < length; i++) {
        int color = matrix[i];
        int newColor = truncate(static_cast<int>(coeff * (color - 128) + 128));
        result[i] = newColor;
    }

    return result;
}

int* getSendCount(int matrixSize, int commSize) {
    int* sendCount = new int[commSize];
    int div = matrixSize / commSize;
    int ren = matrixSize % commSize;
    for (int i = 0; i < commSize; i++) {
        sendCount[i] = div;
    }
    if (ren != 0) {
        sendCount[commSize - 1] = sendCount[commSize - 1] + ren;
    }
    return sendCount;
}

int* getDispls(int matrixSize, int commSize) {
    int* displs = new int[commSize];
    int div = matrixSize / commSize;
    displs[0] = 0;
    for (int i = 1; i < commSize; i++) {
        displs[i] = displs[i - 1] + div;
    }
    return displs;
}

std::vector<int> getContrastedMatrixParallel(std::vector<int> matrix,
                                             int size) {
    int rank, commSize;

    int recvCount;

    std::vector<int> result(size);

    MPI_Comm_size(MPI_COMM_WORLD, &commSize);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank != commSize - 1) {
        recvCount = size / commSize;
    } else {
        recvCount = size / commSize + size % commSize;
    }

    int* sendCount = getSendCount(size, commSize);
    int* displs = getDispls(size, commSize);

    std::vector<int> localVec(recvCount);
    std::vector<int> localResult(recvCount);

    MPI_Scatterv(matrix.data(), sendCount, displs, MPI_INT, localVec.data(),
                 recvCount, MPI_INT, 0, MPI_COMM_WORLD);

    localResult = getContrastedMatrixSequential(localVec);

    MPI_Gatherv(localResult.data(), recvCount, MPI_INT, result.data(),
                sendCount, displs, MPI_INT, 0, MPI_COMM_WORLD);

    return result;
}
