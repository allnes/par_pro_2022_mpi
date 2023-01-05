// Copyright 2022 Andrey Chubenko
#include <mpi.h>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include "../../../modules/task_1/chubenko_a_vector_disorder/chubenko_a_vector_disorder.h"

std::vector<int> getRandomVector(int sz) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::vector<int> vec(sz);
    for (int  i = 0; i < sz; i++) { vec[i] = gen() % 100; }
    return vec;
}

// Suppose that the vector is sorted in ascending order
// Get number of elements in invalid order
int getSequentialNDisorder(const std::vector<int> &vec) {
    const int  sz = vec.size();
    int n = 0;
    for (int i = 0; i < sz - 1; i++) {
        if (vec[i + 1] < vec[i]) { n++; }
    }
    return n;
}

int getParallelNDisorder(const std::vector<int> &global_vec,
                          int count_size_vector) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int delta = count_size_vector / size;

    if (rank == 0) {
        for (int proc = 1; proc < size; proc++) {
            MPI_Send(global_vec.data() + (proc - 1) * delta, delta + 1,
                        MPI_INT, proc, 0, MPI_COMM_WORLD);
        }
    }

    std::vector<int> local_vec(delta);
    if (rank == 0) {
        local_vec = std::vector<int>(global_vec.begin() + (size - 1) * delta,
            global_vec.end());
    } else {
        local_vec.resize(delta + 1);
        MPI_Status status;
        MPI_Recv(local_vec.data(), delta + 1, MPI_INT, 0, 0,
                 MPI_COMM_WORLD, &status);
    }

    int global_n = 0;
    int local_n = getSequentialNDisorder(local_vec);

    MPI_Reduce(&local_n, &global_n, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    return global_n;
}
