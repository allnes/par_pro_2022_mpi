// Copyright 2022 Shipilov Artem
#include <mpi.h>
#include <vector>
#include <utility>
#include <algorithm>
#include <stdexcept>
#include <ctime>
#include <random>
#include <iostream>
#include "../../../modules/task_1/shipilov_a_find_most_diff/find_most_diff.h"

std::vector<int> Random(int size) {
    std::mt19937 random;
    random.seed(static_cast<unsigned int>(time(0)));
    std::vector<int> vec(size);
    for (int i = 0; i < size; i++) { vec[i] = random() % 100; }
    return vec;
}

pair<int, int> ParallelOperation(const vector<int>& v) {
    int size, rank;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if ((static_cast<int>(v.size()) <= size) || (size == 1)) {
        if (rank == 0) {
            return SequenOperation(v);
        } else {
            return std::make_pair(0, 0);
        }
    }
    vector<int> vec_loc;

    int size_new = v.size() / size + 1;

    if (rank == 0) {
        for (int num_proc = 1; num_proc < size; num_proc++) {
            int start_index = size_new * num_proc - 1 * num_proc;

            if (num_proc == size - 1) {
                MPI_Send(&v[0] + start_index, v.size() - start_index,
                    MPI_INT, num_proc, 0, MPI_COMM_WORLD);
            } else {
                MPI_Send(&v[0] + start_index, size_new, MPI_INT, num_proc, 0, MPI_COMM_WORLD);
            }
        }
        vec_loc = vector<int>(size_new);
        vec_loc = vector<int>(v.begin(), v.begin() + size_new);
    } else {
        MPI_Status status;

        if (rank == size - 1) {
            size_new = v.size() - (size_new * (size - 1) - 1 * (size - 1));
        }
        vec_loc = vector<int>(size_new);
        MPI_Recv(&vec_loc[0], size_new, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }

    auto res = SequenOperation(vec_loc);
    int arr[3] = { res.first, res.second, rank };
    int max_elements[3] = { 0, 0, 0 };

    MPI_Op op;
    MPI_Op_create(reinterpret_cast<MPI_User_function*>(pairCompare), true, &op);
    MPI_Reduce(arr, max_elements, 3, MPI_INT, op, 0, MPI_COMM_WORLD);
    return std::make_pair(max_elements[0], max_elements[1]);
}

pair<int, int> SequenOperation(const vector<int>& v) {
    if (v.size() < 2) {
        throw std::runtime_error("Wrong size!");
    }
    int diff = abs(v[0] - v[1]);
    pair<int, int> res = std::make_pair(v[0], v[1]);

    for (int i = 1; i < static_cast<int>(v.size()) - 1; i++) {
        if (abs(v[i] - v[i + 1]) > diff) {
            diff = abs(v[i] - v[i + 1]);
            res.first = v[i];
            res.second = v[i + 1];
        }
    }

    return res;
}

void pairCompare(int* in, int* out, int* lenght, MPI_Datatype* type) {
    if (abs(out[0] - out[1]) < abs(in[0] - in[1])) {
        out[1] = in[1];
        out[0] = in[0];
    } else {
        if ((in[2] < out[2]) && abs(in[0] - in[1]) == abs(out[0] - out[1])) {
            out[1] = in[1];
            out[0] = in[0];
        }
    }
}
