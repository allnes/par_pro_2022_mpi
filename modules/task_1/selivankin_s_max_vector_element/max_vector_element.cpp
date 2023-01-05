// Copyright 2022 Selivankin Sergey
#include <mpi.h>
#include <vector>
#include <random>

#include "../../../modules/task_1/selivankin_s_max_vector_element/max_vector_element.h"


std::vector<int> getRandomVector(int size) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::vector<int> vec(size);
    for (int  i = 0; i < size; i++) { vec[i] = gen() % 100; }
    return vec;
}

int getMaxVectorElemSequence(std::vector<int> vec) {
    const int  size = vec.size();
    int reduction_elem = vec[0];
    for (int  i = 1; i < size; i++) {
        reduction_elem = std::max(reduction_elem, vec[i]);
    }
    return reduction_elem;
}

int getMaxVectorElemParallel(std::vector<int> global_vec, int count_size_vector) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int delta = count_size_vector / size;
    const int additional_delta = count_size_vector % size;

    if (rank == 0) {
        for (int proc = 1; proc < size; proc++) {
            MPI_Send(global_vec.data() + proc * delta + additional_delta,
                delta, MPI_INT, proc, 0, MPI_COMM_WORLD);
        }
    }

    std::vector<int> local_vec(delta);
    if (rank == 0) {
        local_vec = std::vector<int>(global_vec.begin(), global_vec.begin() + delta + additional_delta);
    } else {
        MPI_Status status;
        MPI_Recv(local_vec.data(), delta, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }

    int global_max = 0;
    int local_max = getMaxVectorElemSequence(local_vec);

    MPI_Reduce(&local_max, &global_max, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
    return global_max;
}
