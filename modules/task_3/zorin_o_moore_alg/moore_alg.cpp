// Copyright 2022 Zorin Oleg
#include "../../modules/task_3/zorin_o_moore_alg/moore_alg.h"
#include <mpi.h>
#include <deque>

std::vector<int> update(std::vector<int> *dist, const int size, const std::vector<int> &range, const int d) {
    std::vector<int> update;
    for (int i = 0; i < size; i++) {
        if (d + range[i] < dist->at(i) && range[i] != INF) {
            dist->at(i) = d + range[i];
            update.push_back(i);
        }
    }
    return update;
}

void side_process_work(int prank) {
    while (true) {
        int size;
        MPI_Status status;
        MPI_Recv(&size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        if (size == EXIT_PROCESS) return;
        std::vector<int> dist(size);
        MPI_Recv(dist.data(), size, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        std::vector<int> range(size);
        MPI_Recv(range.data(), size, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        int d;
        MPI_Recv(&d, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        std::vector<int> updated = update(&dist, size, range, d);
        int up_size = updated.size();
        MPI_Send(&up_size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        if (up_size != 0)
            MPI_Send(updated.data(), up_size, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Send(dist.data(), size, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
}

int moore_algorithm(const std::vector<std::vector<int>> &adjacency_matrix, int start, int end, int size) {
    int prank, pcount;
    MPI_Comm_rank(MPI_COMM_WORLD, &prank);
    MPI_Comm_size(MPI_COMM_WORLD, &pcount);
    if (prank != 0) {
        side_process_work(prank);
        return 0;
    }
    std::deque<int> q;
    q.push_back(start);

    std::vector<int> dist(size, INF);
    dist[start] = 0;
    std::vector<int> was(size, 0);
    while (!q.empty()) {
        int current = q.front();
        q.pop_front();
        int block = size / pcount;
        for (int i = 1; i < pcount; i++) {
            int start = i * block + size % pcount;
            MPI_Send(&block, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(dist.data() + start, block, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(adjacency_matrix[current].data() + start, block, MPI_INT, i, 0,
                     MPI_COMM_WORLD);
            MPI_Send(&dist[current], 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
        std::vector<int> upd = update(&dist, block + size % pcount,
                                      adjacency_matrix[current], dist[current]);
        for (auto v : upd) {
            v = v + start;
            if (was[v] == 0) {
                q.push_back(v);
                was[v] = 1;
            } else {
                q.push_front(v);
            }
        }
        for (int i = 1; i < pcount; i++) {
            int start = i * block + size % pcount;
            int resv_size;
            MPI_Status status;
            MPI_Recv(&resv_size, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
            std::vector<int> update(resv_size);
            if (resv_size != 0)
                MPI_Recv(update.data(), resv_size, MPI_INT, i, 0, MPI_COMM_WORLD,
                         &status);
            MPI_Recv(dist.data() + start, block, MPI_INT, i, 0, MPI_COMM_WORLD,
                     &status);
            for (auto v : update) {
                v = v + start;
                if (was[v] == 0) {
                    q.push_back(v);
                    was[v] = 1;
                } else {
                    q.push_front(v);
                }
            }
        }
    }

    for (int i = 1; i < pcount; i++) {
        int ext = EXIT_PROCESS;
        MPI_Send(&ext, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
    }

    return dist[end];
}
