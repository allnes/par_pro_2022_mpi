// Copyright 2018 Nesterov Alexander
#include <mpi.h>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <numeric>
#include <iostream>
#include "../../../modules/task_1/chernova_a_counting_alphabetic_char/counting_alphabetic_char.h"


std::string getRandomString(size_t  size) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::string str(size, 'c');
    for (size_t  i = 0; i < size; i++) {
        str[i] = static_cast<char>((gen()% 113)+9);
    }
    return str;
}

int CountingAlphabeticCharSequential(const std::string& str) {
    auto l = [](int b, char c) { if (isalpha(c))b++; return b; };
    return std::accumulate(str.begin(), str.end(), 0, l);
}

int CountingAlphabeticCharParallel(const std::string& str) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int delta = str.length() / size;
    if (rank == 0) {
        for (int proc = 1; proc < size; proc++) {
            MPI_Send(str.c_str()+(proc * delta), delta,
                        MPI_CHAR, proc, 0, MPI_COMM_WORLD);
        }
    }

    std::string local_string;
    if (rank == 0) {
        local_string = std::string(str.begin(),
                                     str.begin() + delta);
    } else {
        MPI_Status status;
        MPI_Probe(0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_CHAR, &delta);
        local_string.resize(delta);
        MPI_Recv(&(local_string[0]), delta, MPI_CHAR, 0,
            0, MPI_COMM_WORLD, &status);
    }

    int global_count = 0;
    int local_count = CountingAlphabeticCharSequential(local_string);
    MPI_Reduce(&local_count, &global_count, 1,
        MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    return global_count;
}
