// Copyright 2022 Nikiforova Lada
#include "../../../modules/task_1/nikiforova_l_unright_vec_element/unright_vec_element.h"

#include <mpi.h>

#include <iostream>
#include <random>
#include <vector>

std::vector<int> getRandomVector(int size) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::vector<int> vec(size);
    for (int i = 0; i < size; i++) {
        vec[i] = gen() % 100;
    }
    return vec;
}

int unrightElements(std::vector<int> vec) {
    int res = 0;
    for (std::vector<int>::iterator i = vec.begin() + 1; i != vec.end(); ++i) {
        std::vector<int>::iterator j = i - 1;
        if (*j > *i) {
            res++;
        }
    }
    return res;
}

int parallelFindUnrightElements(const std::vector<int> &globalVec,
                                int vecSize) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> localVec;
    const int delta = vecSize / size;
    int localVecSize = delta;
    int r = 0;

    if (rank == 0) {
        int ost = vecSize % size;
        localVecSize = delta + ost;
        localVec = std::vector<int>(globalVec.begin(),
                                    globalVec.begin() + localVecSize);

        for (int i = 0; i < size - 1; i++) {
            MPI_Send(globalVec.data() + localVecSize + (i * delta) - 1,
                     delta + 1, MPI_INT, i + 1, 0, MPI_COMM_WORLD);
        }
    } else {
        MPI_Status status;
        localVec = std::vector<int>(delta + 1);
        MPI_Recv(localVec.data(), delta + 1, MPI_INT, 0, 0, MPI_COMM_WORLD,
                 &status);
    }
    int localRes = unrightElements(localVec);
    int globalRes = 0;
    MPI_Reduce(&localRes, &globalRes, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    return globalRes + r;
}
