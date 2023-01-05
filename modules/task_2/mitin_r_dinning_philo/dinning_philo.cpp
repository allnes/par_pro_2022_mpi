// Copyright 2022 Mitin Roman
#include "../../../modules/task_2/mitin_r_dinning_philo/dinning_philo.h"
#include <cstring>
#include <iostream>


enum tags {
    add = 1, disconect_proc
};

void waiter(int64_t* data, int mpi_size) {
    int up_process = mpi_size - 1;
    MPI_Status status;
    memset(&status, 0, sizeof(status));

    // printf("Hello %d\n", mpi_size);


    while ( up_process != 0 ) {
        int delta = 0;

        MPI_Recv(&delta, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        if ( status.MPI_TAG == disconect_proc ) {
            // std::cout << "process : " << status.MPI_SOURCE << " | " << delta << " disconected\n";
            up_process--;
        } else {
            if ( status.MPI_TAG == add ) {
                *data += delta;
                MPI_Send(&delta, 1, MPI_INT, status.MPI_SOURCE, 0, MPI_COMM_WORLD);
            } else {
                std::cerr << "Wrong tag\n";
            }
        }
    }
}

void philo(int mpi_rank) {
    int diff = 0;

    for (int i = 0; i < 0; i++) {
        if ( mpi_rank % 2 ) {
            diff++;
        } else {
            diff--;
        }
        MPI_Send(&diff, 1, MPI_INT, 0, add, MPI_COMM_WORLD);
        MPI_Recv(&diff, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, NULL);
    }

    diff = -10000000;
    MPI_Send(&mpi_rank, 1, MPI_INT, 0, disconect_proc, MPI_COMM_WORLD);
}

void dinning(int64_t* data) {
    int size;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if ( size % 2 == 0 )
        size--;

    if (rank == 0) {
        waiter(data, size);
    } else {
        if ( rank != size ) {
            philo(rank);
        }
    }
}



