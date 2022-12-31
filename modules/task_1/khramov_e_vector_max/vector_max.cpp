// Copyright 2022 Khramov Egor
#include "../../../modules/task_1/khramov_e_vector_max/vector_max.h"

#include <mpi.h>

#include <iostream>
#include <random>

void printVector(const std::vector<int>& vec) {
    for (int i = 0; i < vec.size(); ++i) std::cout << vec[i] << " ";
    std::cout << std::endl;
}

std::vector<int> getRandomVector(int size) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::vector<int> vec(size);
    for (int i = 0; i < size; i++) {
        vec[i] = gen() % 100;
    }
    return vec;
}

int getMax(std::vector<int> vec) {
    int max = vec[0];
    for (int i = 1; i < vec.size(); ++i)
        if (vec[i] > max) max = vec[i];
    return max;
}

int getMaxParallel(const std::vector<int>& vec, int vec_size) {
    int rank, comm_size;
    int total_max = 0, proc_max = 0;
    MPI_Status status;

    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int local_size = vec_size / comm_size;
    int remainder = vec_size % comm_size;

    std::vector<int> local_vector(local_size);

    if (rank == 0) {
        for (int i = 1; i < comm_size; ++i) {
            MPI_Send(vec.data() + remainder + local_size * i, local_size,
                     MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    }

    if (rank == 0) {
        local_vector =
            std::vector<int>(vec.begin(), vec.begin() + local_size + remainder);
    } else {
        MPI_Recv(local_vector.data(), local_size, MPI_INT, 0, 0, MPI_COMM_WORLD,
                 &status);
    }

    proc_max = getMax(local_vector);
    MPI_Reduce(&proc_max, &total_max, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

    return total_max;
}
