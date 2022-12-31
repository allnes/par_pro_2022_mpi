// Copyright 2022 Eremin Aleksandr
#include <mpi.h>
#include <math.h>
#include <iostream>
#include <random>
#include <algorithm>
#include "../../../modules/test_tasks/test_mpi/ops_mpi.h"

std::vector<int> random(int size) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::vector<int> vec(size);
    for (int i = 0; i < size; i++) {
        vec[i] = gen() % 10;
    }
    return vec;
}

int sum(std::vector<int> V) {
    int size = V.size();
    int sum = 0;

    for (int i = 0; i < size; i++) {
        sum += V[i];
    }
    return sum;
}

int sumParallel(std::vector<int> Vector, int size) {
    int ProcRank, ProcNum;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    int delta = size / ProcNum;
    if (delta == 0) delta = 1;

    int tmp;
    if (ProcNum < size)
        tmp = ProcNum;
    else
        tmp = size;
    std::vector<int> local_vec(delta);
    if (ProcRank == 0) {
        local_vec = std::vector<int>(Vector.begin(), Vector.begin() + delta);
        for (size_t i = 1; i < tmp; i++) {
            MPI_Send(Vector.data() + i * delta, delta, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    } else if (ProcRank < size) {
        MPI_Status status;
        MPI_Recv(local_vec.data(), delta, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }
    if (ProcRank == 0) {
        if (size % tmp > 0) {
            size_t j = 0;
            local_vec.resize(local_vec.size() + size % ProcNum);
            for (int i = (size - size % ProcNum); i < size; i++) {
                local_vec[j + delta] = Vector[i];
                j++;
            }
        }
    }

    int globalSum = 0;
    int localSum = 0;
    if (ProcRank < size) localSum = sum(local_vec);
    MPI_Reduce(&localSum, &globalSum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    return globalSum;
}
