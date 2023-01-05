// Copyright 2022 Tuzhilkina Polina
#include "./vector_avg.h"
#include <mpi.h>
#include <algorithm>
#include <iostream>
#include <random>
#include <string>
#include <vector>

std::vector<int> getRandomVec(int size) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::vector<int> vec(size);
    for (int i = 0; i < size; i++) {
        vec[i] = gen() % 10;
    }
    return vec;
}

double getSumSeq(const std::vector<int>& vec) {
    const int size = vec.size();
    int result = 0;
    for (int i = 0; i < size; i++) result += vec[i];
    return result;
}

double getAvgSeq(const std::vector<int>& vec) {
    return static_cast<double>(getSumSeq(vec)) / static_cast<int>(vec.size());
}

double getAvgPar(const std::vector<int>& globVec, int glob_vec_size) {
    int comm_size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int defaultQuantity = glob_vec_size / comm_size;
    const int remains = glob_vec_size % comm_size;

    if (rank == 0) {
        for (int pRank = 1; pRank < comm_size; pRank++) {
            MPI_Send(globVec.data() + remains + pRank * defaultQuantity,
                     defaultQuantity, MPI_INT, pRank, 0, MPI_COMM_WORLD);
        }
    }

    std::vector<int> localVec(defaultQuantity);
    if (rank == 0) {
        localVec = std::vector<int>(
            globVec.begin(), globVec.begin() + defaultQuantity + remains);
    } else {
        MPI_Status stat;
        MPI_Recv(localVec.data(), defaultQuantity, MPI_INT, 0, 0,
                 MPI_COMM_WORLD, &stat);
    }

    int globalSum = 0;
    int localSum = getSumSeq(localVec);

    MPI_Reduce(&localSum, &globalSum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        return static_cast<double>(globalSum) / glob_vec_size;
    }
    return globalSum;
}

void printVecElements(const std::vector<int>& vec) {
    int size = static_cast<int>(vec.size());
    for (int i = 0; i < size - 1; i++) {
        std::cout << vec[i] << ", ";
    }
    std::cout << vec[size - 1];
}
