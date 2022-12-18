// Copyright 2022 Churkin Alexander
#include <mpi.h>
#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include "../../../modules/task_1/churkin_a_vector_sum/vector_sum.h"

std::vector<int> getRandomVector(int size) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::vector<int> vec(size);
    for (int i = 0; i < size; i++) {
        vec[i] = gen() % 10;
    }
    return vec;
}

void printVector(const std::string& name, const std::vector<int>& vec) {
    std::cout << name << " = ";
    std::cout << '[';
    for (int i = 0; i < static_cast<int>(vec.size()) - 1; i++) {
        std::cout << vec[i] << ", ";
    }
    std::cout << vec[static_cast<int>(vec.size()) - 1];
    std::cout << ']';
}

int getSumSequential(std::vector<int> vec) {
    const int size = vec.size();

    int result = 0;

    for (int i = 0; i < size; i++) {
        result += vec[i];
    }

    return result;
}

int getSumParallel(const std::vector<int>& globalVector, int global_vector_size) {
    int comm_size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        // std::cout << "comm_size = " << comm_size << '\n';
        // printVector("globalVector", globalVector);
    }

    const int delta = global_vector_size / comm_size;

    const int diff = global_vector_size - comm_size * delta;

    if (rank == 0) {
        for (int procNum = 1; procNum < comm_size - 1; procNum++) {
            MPI_Send(globalVector.data() + procNum * delta, delta,
                        MPI_INT, procNum, 0, MPI_COMM_WORLD);
        }
        if (comm_size > 1) {
            if (diff > 0) {
                MPI_Send(globalVector.data() + (comm_size - 1) * delta, delta + diff,
                            MPI_INT, (comm_size - 1), 0, MPI_COMM_WORLD);
            } else {
                MPI_Send(globalVector.data() + (comm_size - 1) * delta, delta,
                            MPI_INT, (comm_size - 1), 0, MPI_COMM_WORLD);
            }
        }
    }

    std::vector<int> localVector(delta);
    if (rank == 0) {
        localVector = std::vector<int>(globalVector.begin(),
                                     globalVector.begin() + delta);
    } else if (rank != comm_size - 1) {
        MPI_Status status;
        MPI_Recv(localVector.data(), delta, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    } else {
        localVector.resize(delta + diff);

        MPI_Status status;
        MPI_Recv(localVector.data(), delta + diff, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }

    // std::cout << "rank = " << rank;
    // std::cout << ", ";
    // printVector("localVector", localVector);
    // std::cout << '\n';

    int globalSum = 0;
    int localSum = getSumSequential(localVector);

    MPI_Reduce(&localSum, &globalSum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    return globalSum;
}
