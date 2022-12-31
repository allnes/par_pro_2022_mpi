// Copyright 2018 Nesterov Alexander
#include <mpi.h>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <ctime>
#include <iostream>
#include "../../../modules/task_1/polyackov_l_vector_sum/vector_sum.h"

std::vector<int> getRandVector(int size) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::vector<int> tmp_vec;
    for (int i = 0; i < size; i++) {
        tmp_vec.push_back(gen() % 100);
    }
    return tmp_vec;
}

int getVecSum(std::vector<int> vec) {
    int sum = 0;
    for (int i = 0; i < vec.size(); i++) {
        sum += vec[i];
    }
    return sum;
}

int getParallelVecSum(std::vector<int> main_vec, int vec_size) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (vec_size < size || size == 1) {
        return getVecSum(main_vec);
    }

    const int fixed_space = vec_size / size;
    const int tail_space = vec_size - fixed_space * size + fixed_space;

    if (rank == 0) {
        auto data_ptr = main_vec.data();
        int procNum = 1;
        int delta = procNum * fixed_space;
        for (; procNum < size - 1; procNum++) {
            MPI_Send(data_ptr + delta, fixed_space, MPI_INT, procNum, 0, MPI_COMM_WORLD);
            delta += fixed_space;
        }
        if (procNum == size - 1) {
            MPI_Send(data_ptr + procNum * fixed_space, tail_space, MPI_INT, procNum, 0, MPI_COMM_WORLD);
        }
    }

    std::vector<int> sub_vec;
    if (rank == 0) {
        sub_vec.resize(fixed_space);
        auto it_begin = main_vec.begin();
        sub_vec = std::vector<int>(it_begin, it_begin + fixed_space);
    }
    if (rank > 0 && rank < size - 1) {
        sub_vec.resize(fixed_space);
        auto sub_data_ptr = sub_vec.data();
        MPI_Status status;
        MPI_Recv(sub_data_ptr, fixed_space, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    } else if (rank == size - 1) {
        sub_vec.resize(tail_space);
        auto sub_data_ptr = sub_vec.data();
        MPI_Status status;
        MPI_Recv(sub_data_ptr, tail_space, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    }

    int recv_buf = 0;
    int send_buf = getVecSum(sub_vec);

    MPI_Reduce(&send_buf, &recv_buf, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    return recv_buf;
}
