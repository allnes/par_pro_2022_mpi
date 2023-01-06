// Copyright 2022 Tsvetkov
#include <mpi.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <ctime>
#include <random>
#include <queue>
#include <list>
#include <iostream>
#include "../../../modules/task_3/tsvetkov_a_moore_alg/moore_alg.h"

std::vector<int> sequenceMoore(std::vector<int> graph, int start, int n) {
    std::list <int> Q;
    int inf = 1000, curr;
    std::vector<int> result(n, inf);
    Q.push_front(start);
    result.at(start) = 0;
    while (Q.size() != 0) {
        int curr = Q.front();
        Q.pop_front();
        for (int i = 0; i < n; i++) {
            int currLen = graph[i + curr * n];
            if (currLen == inf) continue;
            if (currLen + result.at(curr) < result.at(i)) {
                result.at(i) = currLen + result.at(curr);
                for (int j : Q) {
                    if (j == i) {
                        Q.remove(j);
                        Q.push_front(j);
                    }
                }
                if (Q.front() != i) {
                Q.push_back(i);
                }
            }
        }
    }
return result;
}

std::vector<int> parallelMoore(std::vector<int> graph, int start, int n) {
    int ProcNum, ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    int r = n % ProcNum;
    int d = n / ProcNum;
    int inf = 1000;
    std::vector<int> result;
    std::vector<int> buffer(n * d);
    if (d > 0) {
        MPI_Scatter(&graph[r * n], d * n, MPI_INT, &buffer[0], d * n, MPI_INT, 0, MPI_COMM_WORLD);
    }

    if (ProcRank == 0) {
        buffer.insert(buffer.begin(), graph.begin(), graph.begin() + r * n);
    }

    std::vector<bool> visited(ProcRank == 0 ? d + r : d, false);
    std::vector<int> distance(ProcRank == 0 ? d + r : d, inf);
    std::list <int> Q;
    int inRoot;
    if (ProcRank == 0) {
        inRoot = r;
    } else {
        inRoot = 0;
    }
    for (int i = 0; i < d + inRoot; i++) {
        distance[i] = buffer[i * n + start];
        if (distance[i] == 0) {
            visited[i] = true;
        }
    }

    for (int i = 0; i < n - 1; i++) {
        int min_dist = inf;
        int min_index = -1;
        for (size_t j = 0; j < distance.size(); j++) {
            if (distance[j] <= min_dist && !visited[j]) {
                min_dist = distance[j];
                min_index = j;
                Q.push_front(i);
            }
        }
        int index = -1;
        if (min_index >= 0) {
            index = r - inRoot + min_index + ProcRank * d;
        }

        int recv_index = -1;
        int recv_dist = inf;
        int glob_index = index;
        int globmin_dist = min_dist;

        if (d > 0) {
            if (ProcRank == 0) {
                for (int i = 1; i < ProcNum; i++) {
                    MPI_Recv(&recv_index, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    MPI_Recv(&recv_dist, 1, MPI_INT, i, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

                    if (recv_dist < globmin_dist) {
                        globmin_dist = recv_dist;
                        glob_index = recv_index;
                    }
                }
                for (int i = 1; i < ProcNum; i++) {
                    MPI_Send(&glob_index, 1, MPI_INT, i, 2, MPI_COMM_WORLD);
                    MPI_Send(&globmin_dist, 1, MPI_INT, i, 3, MPI_COMM_WORLD);
                }
            } else {
                MPI_Send(&index, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
                MPI_Send(&min_dist, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
                MPI_Recv(&glob_index, 1, MPI_INT, 0, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Recv(&globmin_dist, 1, MPI_INT, 0, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
        }
        if (glob_index == -1) {
            break;
        }
        if (glob_index == index && min_index != -1) {
            visited[min_index] = true;
        }

        for (int i = 0; i < d + inRoot; i++) {
            if (buffer[glob_index + i * n] != inf && !visited[i]
                && (globmin_dist + buffer[glob_index + i * n] < distance[i])) {
                distance[i] = globmin_dist + buffer[glob_index + i * n];
                Q.push_front(i);
            }
        }
    }

    if (ProcRank == 0) {
        result.insert(result.begin(), distance.begin(), distance.end());
        std::vector<int> recvRes(d);
        for (int i = 1; i < ProcNum; i++) {
            if (d > 0) {
                MPI_Recv(&recvRes[0], d, MPI_INT, i, 4, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                result.insert(result.end(), recvRes.begin(), recvRes.end());
            }
        }
    } else {
        if (d > 0) {
            MPI_Send(&distance[0], d, MPI_INT, 0, 4, MPI_COMM_WORLD);
        }
    }
    return result;
}
