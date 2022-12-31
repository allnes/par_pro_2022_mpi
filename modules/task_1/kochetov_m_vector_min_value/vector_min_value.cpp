// Copyright 2022 Kochetov Maksim
#include "./vector_min_value.h"
#include <mpi.h>
#include <algorithm>
#include <iostream>
#include <random>
#include <string>
#include <vector>

std::vector<int> genRandomVector(int vecSize) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::vector<int> vec(vecSize);
    for (int i = 0; i < vecSize; i++) vec[i] = gen() % 100;
    return vec;
}

double getVectorMinSequential(const std::vector<int>& vec) {
    if (vec.size() == 0) throw "Vector is empty!";

    int result = vec[0];

    for (int k = 1; k < static_cast<int>(vec.size()); k++) {
        if (vec[k] < result) {
            result = vec[k];
        }
    }

    return result;
}

double getVectorMinParralel(const std::vector<int>& globVec, const int vecSize) {
    int commSize, procRank;
    MPI_Comm_size(MPI_COMM_WORLD, &commSize);
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);

    const int standardCount = vecSize / commSize;
    const int remainder = vecSize % commSize;

    // Let's send standardCount of items from process 0 to other processes
    if (procRank == 0 && standardCount > 0) {
        for (int pRank = 1; pRank <= commSize - 1; pRank++) {
            MPI_Send(globVec.data() + remainder + pRank * standardCount,
                     standardCount, MPI_INT, pRank, 0, MPI_COMM_WORLD);
        }
    }

    std::vector<int> vecLocal(standardCount);
    if (procRank == 0) {
        // If process is 0, we get our required numbers directly
        // ((standardCount + remainder) of them)
        vecLocal = std::vector<int>(
            globVec.begin(), globVec.begin() + (standardCount + remainder));
    } else if (standardCount > 0) {
        // In other case we just need to receive standardCount of numbers from
        // process 0
        MPI_Status mpiStatus;
        MPI_Recv(vecLocal.data(), standardCount, MPI_INT, 0, 0, MPI_COMM_WORLD,
                 &mpiStatus);
    }

    int globalMin = 0;

    if (standardCount > 0) {
        int localMin = getVectorMinSequential(vecLocal);
        // And then reduce (get the minimum from all of the local minumums)
        MPI_Reduce(&localMin, &globalMin, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);
    } else if (standardCount == 0 && procRank == 0) {
        globalMin = getVectorMinSequential(vecLocal);
    }

    return globalMin;
}
