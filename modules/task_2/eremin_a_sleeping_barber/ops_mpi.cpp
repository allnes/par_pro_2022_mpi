// Copyright 2022 Eremin Aleksandr
#include <math.h>
#include <mpi.h>
#include <iostream>
#include <random>
#include <algorithm>
#include <queue>
#include "../../../modules/test_tasks/test_mpi/ops_mpi.h"

#define Get_in_line 1
#define Go_Barber 2
#define Get_un_line 3
#define Exit_Queue 4

double getRandomTime() {
    std::random_device rd;
    std::mt19937 gen(rd());
    return (10.0 + static_cast<double>(gen() % 20u)) / 100.0;
}

void wait(double time) {
    double start = MPI_Wtime();
    while (MPI_Wtime() - start < time) {
    }
}

void client(int rank) {
    wait(getRandomTime());
    MPI_Status status;
    int queue_index = 0;

    MPI_Send(&rank, 1, MPI_INT, 1, Get_in_line, MPI_COMM_WORLD);
    MPI_Recv(&queue_index, 1, MPI_INT, MPI_ANY_SOURCE, Get_un_line,
        MPI_COMM_WORLD, &status);

    if (queue_index != 0) {
        MPI_Send(&rank, 1, MPI_INT, 0, Go_Barber, MPI_COMM_WORLD);
    } else {
        int nun = -1;
        MPI_Send(&nun, 1, MPI_INT, 0, Go_Barber, MPI_COMM_WORLD);
    }
}

void queue(int numberOFseats, int numberOFclients) {
    int client_rank;
    int queue_index = 0, index = 0;
    MPI_Status status;
    std::queue<int> same_queue;
    int i = 0;
    while (i < numberOFclients) {
        MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        if (status.MPI_TAG != Exit_Queue && index < numberOFclients) {
            MPI_Recv(&client_rank, 1, MPI_INT, MPI_ANY_SOURCE, Get_in_line,
                MPI_COMM_WORLD, &status);
            if ((same_queue.size()) < numberOFseats) {
                same_queue.push(client_rank);
                queue_index = 1;
                index++;
                MPI_Send(&queue_index, 1, MPI_INT, client_rank, Get_un_line,
                    MPI_COMM_WORLD);
            } else {
                queue_index = 0;
                MPI_Send(&queue_index, 1, MPI_INT, client_rank, Get_un_line,
                    MPI_COMM_WORLD);
                i++;
                index++;
            }
        }
        if (status.MPI_TAG == Exit_Queue || index == numberOFclients) {
            MPI_Recv(&client_rank, 1, MPI_INT, MPI_ANY_SOURCE, Exit_Queue,
                MPI_COMM_WORLD, &status);
            same_queue.pop();
            i++;
        }
    }
}

void Barber(int numberOFseats, int numberOFclients) {
    int client_rank = 0;
    MPI_Status status;
    for (int i = 0; i < numberOFclients; i++) {
        MPI_Recv(&client_rank, 1, MPI_INT, MPI_ANY_SOURCE, Go_Barber,
            MPI_COMM_WORLD, &status);
        if (client_rank != -1) {
            MPI_Send(&client_rank, 1, MPI_INT, 1, Exit_Queue, MPI_COMM_WORLD);
            wait(getRandomTime());
        }
    }
}
