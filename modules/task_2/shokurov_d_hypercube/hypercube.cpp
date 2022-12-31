// Copyright 2022 Shokurov Daniil
#include <mpi.h>
#include <stdlib.h>

#include <random>
#include <vector>
#include <algorithm>

#include "../../modules/task_2/shokurov_d_hypercube/hypercube.h"

int inv(int x, int i) {
    int v = (x & (1 << i));
    if (v) {
        x &= ~(1 << i);
    } else {
        x |= (1 << i);
    }
    return x;
}

std::vector<int> right_to_left(int i, int j) {
    std::vector<int> v;
    int y = j;
    int temp = i;
    int index = 0;
    v.push_back(temp);
    while (y > 0) {
        if ((i & (1 << index)) != (j & (1 << index))) {
            temp = inv(temp, index);
            v.push_back(temp);
        }
        index++;
        y >>= 1;
    }
    return v;
}

std::vector<int> find_path(int i, int j) {
    std::vector<int> path;
    if (i <= j) {
        path = right_to_left(i, j);
    } else {
        path = right_to_left(j, i);
        reverse(path.begin(), path.end());
    }
    return path;
}

void send(int i, int j, char** mes, int* n) {
    int rank = 0;
    int ProcNum = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == i) {
        std::vector<int> path;
        if (i != j) {
            path = find_path(i, j);
            int* members = new int[ProcNum];
            for (int k = 0; k < ProcNum; ++k)
                members[k] = 0;
            for (auto it : path) {
                members[it] = 1;
            }
            for (int k = 0; k < ProcNum; ++k) {
                if (k == rank)
                    continue;
                MPI_Send(&members[k], 1, MPI_INT, k, 101, MPI_COMM_WORLD);
            }
            for (int k = 1; k + 1 < path.size(); ++k) {
                MPI_Send(&path[k + 1], 1, MPI_INT, path[k], 102, MPI_COMM_WORLD);
            }
            MPI_Send(&j, 1, MPI_INT, j, 102, MPI_COMM_WORLD);
            MPI_Send(*mes, *n, MPI_CHAR, path[1], 103, MPI_COMM_WORLD);
        } else {
            int busy = 0;
            for (int k = 0; k < ProcNum; ++k) {
                if (k == rank)
                    continue;
                MPI_Send(&busy, 1, MPI_INT, k, 101, MPI_COMM_WORLD);
            }
        }
    } else {
        MPI_Status status;
        int busy;
        MPI_Recv(&busy, 1, MPI_INT, MPI_ANY_SOURCE, 101, MPI_COMM_WORLD, &status);
        if (busy) {
            int j;
            MPI_Recv(&j, 1, MPI_INT, MPI_ANY_SOURCE, 102, MPI_COMM_WORLD, &status);
            if (rank == j) {
                int count;
                MPI_Probe(MPI_ANY_SOURCE, 103, MPI_COMM_WORLD, &status);
                MPI_Get_count(&status, MPI_CHAR, &count);
                *n = count;
                *mes = new char[count];
                MPI_Recv(*mes, count, MPI_CHAR, MPI_ANY_SOURCE, 103, MPI_COMM_WORLD, &status);
            } else {
                int count;
                MPI_Probe(MPI_ANY_SOURCE, 103, MPI_COMM_WORLD, &status);
                MPI_Get_count(&status, MPI_CHAR, &count);
                char* ch = new char[count];
                MPI_Recv(ch, count, MPI_CHAR, MPI_ANY_SOURCE, 103, MPI_COMM_WORLD, &status);
                MPI_Send(ch, count, MPI_CHAR, j, 103, MPI_COMM_WORLD);
                delete[] ch;
            }
        } else {
            return;
        }
    }
}
