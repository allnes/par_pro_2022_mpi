// Copyright 2022 Shipilov Artem
#include <mpi.h>
#include <iostream>
#include <queue>
#include <ctime>
#include <random>
#include <vector>
#include "../../../modules/task_2/shipilov_a_producers_consumers/producers_consumers.h"

double time_expectation = 0.002;

void Seconds(double sec) {
    double time = MPI_Wtime();
    while (MPI_Wtime() - time < sec) {
    }
}

int ParallelOperation(int noose) {
    MPI_Request Reques;
    MPI_Status Status;
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int res;
    int consumer_finite = -1;

    std::queue<int> sequence;
    while (noose != 0) {
        if (consumer_finite == -1) {
            MPI_Recv(&res, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &Status);
        } else {
            MPI_Recv(&res, 1, MPI_INT, MPI_ANY_SOURCE, 3, MPI_COMM_WORLD, &Status);
        }
        --noose;
        if (Status.MPI_TAG == 3) {
            sequence.push(res);
            if (consumer_finite != -1) {
                int element = sequence.front();
                sequence.pop();
                MPI_Isend(&element, 1, MPI_INT, consumer_finite, 4, MPI_COMM_WORLD, &Reques);
                MPI_Request_free(&Reques);
                consumer_finite = -1;
            }
        } else {
            if (!sequence.empty()) {
                int el = sequence.front();
                sequence.pop();
                MPI_Isend(&el, 1, MPI_INT, Status.MPI_SOURCE, 4, MPI_COMM_WORLD, &Reques);
                MPI_Request_free(&Reques);
            } else {
                consumer_finite = Status.MPI_SOURCE;
            }
        }
    }
    return sequence.size();
}

void Consumers() {
    int element;
    int rank;
    MPI_Status status;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    Seconds(rank * time_expectation);
    MPI_Send(&rank, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
    MPI_Recv(&element, 1, MPI_INT, 0, 4, MPI_COMM_WORLD, &status);
}

void Producers() {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    Seconds(rank * time_expectation);
    MPI_Send(&rank, 1, MPI_INT, 0, 3, MPI_COMM_WORLD);
}
