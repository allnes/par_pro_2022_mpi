// Copyright 2022 Nikolaev Alexander
#include <mpi.h>
#include <vector>
#include <random>
#include <algorithm>
#include "../../../modules/task_1/nikolaev_a_vector_average/vector_average.h"

float getAverageVectorSequential(std::vector<int> vec, const int GlobalVecSize) {
    int sum = 0;
    const int LocalVecSize = vec.size();
    for (int i = 0; i < LocalVecSize; i++) {
        sum += vec[i];
    }
    return sum/static_cast<float>(GlobalVecSize);
}

// int SumVector(std::vector<int> vec) {
//     const int size = vec.size();
//     int sum = 0;
//     for (int i = 0; i < size; i++) {
//         sum += vec[i];
//     }
//     return sum;
// }

std::vector<int> getRandomVector(int size) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::vector<int> vec(size);
    for (int  i = 0; i < size; i++) {
        vec[i] = gen() % 100;
    }
    return vec;
}

// void PrintVector(std::vector<int> vec) {
//     const int size = vec.size();
//     for (int i = 0; i < size; i++) {
//        std::cout << vec[i] << " ";
//     }
//     std::cout << std::endl;
// }

float getAverageVectorParallel(std::vector<int> vec, int count_size_vector) {
    int size, rank;
    float global_average = 0.0;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int delta = count_size_vector / size;
    const int remainder = count_size_vector % size;

    if (rank == 0) {
        for (int proc = 1; proc < size; proc++) {
            MPI_Send(vec.data() + proc * delta + remainder, delta,
                        MPI_FLOAT, proc, 0, MPI_COMM_WORLD);
        }
    }

    std::vector<int> local_vec(delta);
    if (rank == 0) {
        local_vec = std::vector<int>(vec.begin(), vec.begin() + delta + remainder);
    } else {
        MPI_Status status;
        MPI_Recv(local_vec.data(), delta, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, &status);
    }

    float local_average = getAverageVectorSequential(local_vec, count_size_vector);
    MPI_Reduce(&local_average, &global_average, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);

    return global_average;
}
