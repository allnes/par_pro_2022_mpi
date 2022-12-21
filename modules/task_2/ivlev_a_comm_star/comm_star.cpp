// Copyright 2022 Ivlev A
#include <mpi.h>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include "../../../modules/task_2/ivlev_a_comm_star/comm_star.h"


void MPI_group_star_create(MPI_Comm oldcomm, int nnodes,
    int index[], MPI_Comm* newcomm) {

    int size, rank;

    MPI_Comm_size(oldcomm, &size);
    MPI_Comm_rank(oldcomm, &rank);

    MPI_Group old_group;
    MPI_Comm_group(oldcomm, &old_group);

    MPI_Group star_group;
    MPI_Group_incl(old_group, nnodes, index, &star_group);

    MPI_Comm star_comm;
    MPI_Comm_create(oldcomm, star_group, &star_comm);

    MPI_Group_free(&old_group);

    *newcomm = star_comm;
}

int Star_Send(const void *buf, int count, MPI_Datatype datatype,
    int from, int dest, int tag, MPI_Comm comm) {
    if (comm != MPI_COMM_NULL) {
        int size, rank;

        MPI_Comm_rank(comm, &rank);
        MPI_Comm_size(comm, &size);

        if (rank == from) {
            if (from == 0 || dest == 0) {
                MPI_Send(buf, count, datatype, dest, tag, comm);
                return 0;
            } else {
                MPI_Send(buf, count, datatype, 0, tag, comm);
                return 0;
            }
        } else {
            if (rank == 0 && dest != 0) {
                MPI_Status status;
                MPI_Datatype* temp = new MPI_Datatype[count];
                MPI_Recv(temp, count, datatype, from, tag, comm, &status);
                MPI_Send(temp, count, datatype, dest, tag, comm);
                delete [] temp;
            }
            return 0;
        }

    } else {
        return -1;
    }
}
