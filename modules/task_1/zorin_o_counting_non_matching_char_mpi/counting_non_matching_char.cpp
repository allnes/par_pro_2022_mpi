// Copyright 2022 Zorin Oleg
#include "../../../modules/task_1/zorin_o_counting_non_matching_char_mpi/counting_non_matching_char.h"
#include <mpi.h>
#include <cstring>
#include <random>
#include <string>

const char *getRandomString(int len) {
    const char alphanum[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";
    char *random_str = new char[len + 1];
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<size_t> dist(0, sizeof(alphanum) - 1);

    for (int i = 0; i < len; i++) {
        random_str[i] = alphanum[dist(mt)];
    }
    random_str[len] = '\0';

    return random_str;
}

int countNonMatchingCharSequential(const char *str, int len,
                                   const char *compare_str, int compare_len) {
    int count = 0;
    for (int i = 0; i < len; i++) {
        const char &chr = str[i];
        int not_match = 1;
        for (int j = 0; j < compare_len; j++) {
            if (compare_str[j] == chr) {
                not_match = 0;
                break;
            }
        }
        count += not_match;
    }
    return count;
}

int countNonMatchingCharParallel(const char *global_str, int len, const char *global_compare_str, int compare_len) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int delta = len / size;
    int shift = rank == size - 1 && len % size != 0 ? len - rank * delta : delta;
    char *local_str = new char[shift];
    std::memcpy(local_str, global_str + delta * rank, sizeof(char) * shift);

    int global_count = 0;
    int local_count = countNonMatchingCharSequential(local_str, shift, global_compare_str, compare_len);
    delete[] local_str;
    MPI_Reduce(&local_count, &global_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    return global_count;
}
