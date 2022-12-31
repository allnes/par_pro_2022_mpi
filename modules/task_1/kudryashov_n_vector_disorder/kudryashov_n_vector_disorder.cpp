  // Copyright 2022 Kudryashov Nikita
#include <mpi.h>
#include <random>
#include <ctime>

#include "../../../modules/task_1/kudryashov_n_vector_disorder/kudryashov_n_vector_disorder.h"

std::vector<int> generateRandomVector(int size) {
    std::mt19937 rnd;
    rnd.seed(std::time(nullptr));
    std::vector<int> vec(size);
    for (int i = 0; i < size; i++) {
        vec[i] = rnd() % 100;
    }

    return vec;
}

int countOfDisruptionInVector(std::vector<int> vec) {
    int size = vec.size();
    int count = 0;
    for (int i = 0; i < size - 1; i++) {
        if (vec[i] > vec[i + 1]) {
            count++;
        }
    }

    return count;
}

int countOfDisruptionInVectorParallel(std::vector<int> vec, int vec_size) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int shift = vec_size / size;

    if (rank == 0) {
        for (int proc = 1; proc < size; proc++) {
        MPI_Send(vec.data() + (proc - 1) * shift, shift + 1, MPI_INT, proc, 0, MPI_COMM_WORLD);
        }
    }

    std::vector<int> local_vec(shift);

    if (rank == 0) {
        local_vec = std::vector<int>(vec.begin() + shift * (size - 1), vec.end());
    } else {
        local_vec.resize(shift + 1);
        MPI_Status status;
        MPI_Recv(local_vec.data(), shift + 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }

    int local_count = countOfDisruptionInVector(local_vec);
    int global_count = 0;
    MPI_Reduce(&local_count, &global_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    return global_count;
}
