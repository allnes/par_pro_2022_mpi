// Copyright 2021 Musin Alexandr

#include "../../../modules/task_1/musin_a_chars_on_str/chars_on_str.h"

#include <mpi.h>
#include <algorithm>
#include <random>
#include <string>
#include <vector>

char *getRandomString(const int size) {
    char *res = new char[size];
    std::random_device dev;
    std::mt19937 gen(dev());

    for (int i = 0; i < size; i++) {
        res[i] = (gen() % 78) + 49;
    }
    return res;
}

int sym_on_str(const char *str, const int size, const char sym) {
    int res = 0;
    for (int i = 0; i < size; i++) {
        if (str[i] == sym) {
            res++;
        }
    }
    return res;
}

int par_sym_on_str(const char *global_str, const int global_str_len,
                   const char sym) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int delta = global_str_len / size;
    const int ost = global_str_len % size;
    const int local_size = delta + ost;

    if (rank == 0) {
        for (int proc = 1; proc < size; proc++) {
            MPI_Send(ost + global_str + proc * delta, delta, MPI_CHAR, proc, 0,
                     MPI_COMM_WORLD);
        }
    }

    char *local_str = new char[local_size];

    if (rank == 0) {
        for (int i = 0; i < local_size; i++) {
            local_str[i] = global_str[i];
        }
    } else {
        MPI_Status status;
        MPI_Recv(local_str, delta, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
    }

    int global_sum = 0;
    int local_sum;
    if (rank == 0) {
        local_sum = sym_on_str(local_str, local_size, sym);
    } else {
        local_sum = sym_on_str(local_str, delta, sym);
    }

    MPI_Op op_code = MPI_SUM;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_INT, op_code, 0, MPI_COMM_WORLD);
    return global_sum;
}
