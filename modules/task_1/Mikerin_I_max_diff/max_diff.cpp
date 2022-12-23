// Copyright 2022 Mikerin Ilya
#include <mpi.h>
#include <vector>
#include <iostream>
#include <string>
#include <random>
#include <algorithm>
#include "./max_diff.h"


std::vector<int> getRandomVector(int  sz) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::vector<int> vec(sz);
    for (int  i = 0; i < sz; i++) { vec[i] = gen() % 100; }
    return vec;
}

int getSequentialOperations(std::vector<int> vec) {
    const int  sz = vec.size();
    int max_diff = 0;
    for ( int i = 0; i < sz-1; i++ ) {
        if ( abs(vec[i]-vec[i+1]) > max_diff)
            max_diff = abs(vec[i]-vec[i+1]);
    }

    return max_diff;
}

int getParallelOperations(std::vector<int> global_vec, int count_size_vector) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    const int delta = count_size_vector / size;
    const int remainder = count_size_vector % size;

    if (rank == 0) {
        for (int proc = 1; proc < size-1; proc++) {
            MPI_Send(global_vec.data() + proc * delta + remainder, delta+1, MPI_INT, proc, 0, MPI_COMM_WORLD);
        }
        MPI_Send(global_vec.data() + (size-1) * delta + remainder, delta, MPI_INT, (size-1), 0, MPI_COMM_WORLD);
    }

    int global_max_diff = 0;

    if (rank == size-1) {
        std::vector<int> local_vec(delta);
        MPI_Status status;
        MPI_Recv(local_vec.data(), delta, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        int local_max_diff = getSequentialOperations(local_vec);

        MPI_Reduce(&local_max_diff, &global_max_diff, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
    } else {
        if (rank == 0) {
            std::vector<int> local_vec(delta+1 + remainder);
            local_vec = std::vector<int>(global_vec.begin(),
                                            global_vec.begin() + delta+ 1 + remainder);
            int local_max_diff = getSequentialOperations(local_vec);
            MPI_Reduce(&local_max_diff, &global_max_diff, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

        } else {
            std::vector<int> local_vec(delta+1);
            MPI_Status status;
            MPI_Recv(local_vec.data(), delta + 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
            int local_max_diff = getSequentialOperations(local_vec);
            MPI_Reduce(&local_max_diff, &global_max_diff, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
        }
    }

    return global_max_diff;
}

