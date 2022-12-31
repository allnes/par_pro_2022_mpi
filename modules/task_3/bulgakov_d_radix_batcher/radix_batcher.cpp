// Copyright 2022 Bulgakov Daniil

#include "../../modules/task_3/bulgakov_d_radix_batcher/radix_batcher.h"

std::vector<double> genvec(int sz) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::vector<double> vec(sz);
    for (int  i = 0; i < sz; i++) {
        vec[i] = (gen() % 1000) / static_cast<double>((gen() % 1000) + 1);
        if (static_cast<double>(gen() % 4) == 2) vec[i] *= - 1;
    }

    return vec;
}

std::vector<double> parallel_radix_batcher_sort(std::vector<double> to_sort_vec) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int vsize = to_sort_vec.size() / size;
    std::vector<double> lvec(vsize);


    MPI_Scatter(to_sort_vec.data(), vsize, MPI_DOUBLE,
        lvec.data(), vsize, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    radix_sort(lvec.data(), lvec.size());

    if (rank != 0) {
        MPI_Send(lvec.data(), vsize, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    } else {
        std::vector<std::vector<double>> vbuff;
        vbuff.push_back(lvec);
        for (int i = 1; i < size; i++) {
            MPI_Recv(lvec.data(), vsize, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            vbuff.push_back(lvec);
        }
        return seq_batcher_merges(vbuff);
    }
    return lvec;
}
