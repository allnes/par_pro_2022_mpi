// Copyright 2022 Bochkarev Vladimir
#include <mpi.h>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include "../../../modules/task_2/bochkarev_v_linear/linear.h"

void getNext(int* next, MPI_Comm comm, int source, bool route) {
    int size;
    MPI_Comm_size(comm, &size);
    if (route) {
        *next = source + 1;
    } else {
        *next = source - 1;
    }
}

void getPrev(int* prev, MPI_Comm comm, int source, bool route) {
    int size;
    MPI_Comm_size(comm, &size);
    if (route) {
        *prev = source - 1;
    } else {
        *prev = source + 1;
    }
}

void send(void* mes, int count, MPI_Datatype type, int source, int dest, int tag, MPI_Comm comm) {
    int rank, size;
    int next, prev;
    bool route;

    if (dest - source > 0) {
        route = true;
    } else {
        route = false;
    }

    MPI_Comm_size(comm, &size);
    MPI_Comm_rank(comm, &rank);

    if (((rank <= dest) && (source <= rank) && route) || ((rank >= dest) && (source >= rank) && !route)) {
        if (((rank != 0) && route) || ((rank != size - 1) && !route)) {
            getPrev(&prev, comm, rank, route);
        }

        if (((rank != size - 1) && route) || ((rank != 0) && !route)) {
            getNext(&next, comm, rank, route);
        }

        if (rank == source) {
            MPI_Send(mes, count, type, next, tag, comm);
            return;
        }

        MPI_Status status;
        MPI_Recv(mes, count, type, prev, tag, comm, &status);

        if (rank != dest) {
            MPI_Send(mes, count, type, next, tag, comm);
        }
    }
}
