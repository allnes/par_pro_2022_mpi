// Copyright 2022 Mikerin Ilya
#include <mpi.h>
#include <vector>
#include <iostream>
#include <string>
#include <random>
#include <algorithm>
#include "../../../modules/task_1/Mikerin_I_max_diff/max_diff.h"


int* getRandomVector(int  sz) {
    std::random_device dev;
    std::mt19937 gen(dev());
    int* vec = new int[sz];
    for (int  i = 0; i < sz; i++) { vec[i] = gen() % 100; }
    return vec;
}

int getSequentialOperations(int* vec, int sz) {
    int max_diff = 0;
    int tmp;
    for ( int i = 0; i < sz-1; i++ ) {
        tmp = abs(vec[i]-vec[i+1]);
        if ( tmp > max_diff)
            max_diff = tmp;
    }

    return max_diff;
}

int getParallelOperations(int* global_vec, int count_size_vector) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int delta = count_size_vector / size;
    const int remainder = count_size_vector % size;
    int* sendcount = new int[size];
    int* displs = new int[size];

    sendcount[0] = delta+1 + static_cast<int>(remainder > 0);
    displs[0] = 0;

        if (remainder > 0) {
            for (int proc =1; proc < remainder -1; proc++) {
                sendcount[proc] = delta + 2;
                displs[proc] = proc * delta + proc;
            }
            for (int proc = remainder - 1; proc < size-1; proc++) {
                if (proc > 0) {
                    sendcount[proc] = delta + 1;
                    displs[proc] = proc * delta + remainder;
                }
            }
            sendcount[size-1] = delta;
            displs[size-1] = (size-1) * delta + remainder;
        } else {
            for (int proc = 1; proc < size-1; proc++) {
                sendcount[proc] = delta + 1;
                displs[proc] = proc * delta;
            }
            sendcount[size-1] = delta;
            displs[size-1] = (size-1) * delta + remainder;
        }
    //}

    int global_max_diff = 0;

    if (rank == size-1) {
        int* local_vec = new int[delta];
        MPI_Status status;
        MPI_Scatterv(global_vec , sendcount , displs , MPI_INT ,
                    local_vec , sendcount[rank] , MPI_INT , 0 , MPI_COMM_WORLD);
        int local_max_diff = getSequentialOperations(local_vec, delta);

        MPI_Reduce(&local_max_diff, &global_max_diff, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
    } else {
        if (rank == 0) {
            int* local_vec = new int[delta+1 + static_cast<int>(remainder > 0)];
            // for (int i = 0; i < delta+ 1 + static_cast<int>(remainder > 0); i++) {
            //     local_vec[i] = global_vec[i];
            // }
            MPI_Scatterv(global_vec , sendcount , displs , MPI_INT ,
                    local_vec , sendcount[rank] , MPI_INT , 0 , MPI_COMM_WORLD);
            int local_max_diff = getSequentialOperations(local_vec, delta+1 + static_cast<int>(remainder > 0));
            MPI_Reduce(&local_max_diff, &global_max_diff, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

        } else {
            if (remainder > 0) {
                if (rank < remainder - 1) {
                    int* local_vec = new int[delta+2];
                    MPI_Status status;
                    MPI_Scatterv(global_vec , sendcount , displs , MPI_INT ,
                    local_vec , sendcount[rank] , MPI_INT , 0 , MPI_COMM_WORLD);
                    int local_max_diff = getSequentialOperations(local_vec, delta+2);
                    MPI_Reduce(&local_max_diff, &global_max_diff, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

                } else {
                    int* local_vec = new int[delta+1];
                    MPI_Status status;

                    MPI_Scatterv(global_vec , sendcount , displs , MPI_INT ,
                    local_vec , sendcount[rank] , MPI_INT , 0 , MPI_COMM_WORLD);
                    int local_max_diff = getSequentialOperations(local_vec, delta+1);
                    MPI_Reduce(&local_max_diff, &global_max_diff, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
                }
            } else {
                int* local_vec = new int[delta+1];
                MPI_Status status;

                MPI_Scatterv(global_vec , sendcount , displs , MPI_INT ,
                    local_vec , sendcount[rank] , MPI_INT , 0 , MPI_COMM_WORLD);
                int local_max_diff = getSequentialOperations(local_vec, delta+1);
                MPI_Reduce(&local_max_diff, &global_max_diff, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
            }
        }
    }

    return global_max_diff;
}

