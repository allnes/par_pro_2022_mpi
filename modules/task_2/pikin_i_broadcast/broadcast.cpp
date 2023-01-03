// Copyright 2022 Pikin Ilya

#include <mpi.h>
#include "../../../modules/task_2/pikin_i_broadcast/broadcast.h"

int getProcName(int root, int procRank, int procNum) {
    return ((procRank - root) + procNum) % procNum;
}

int getProcRank(int root, int procName, int procNum) {
    return (procName + root) % procNum;
}

void myBroadcast(void* buf, int count, MPI_Datatype type, int root,
    MPI_Comm comm) {
    int procNum, procRank, procName;
    MPI_Comm_size(MPI_COMM_WORLD, &procNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);

    procName = getProcName(root, procRank, procNum);

    MPI_Status status;
    int twosDegree = 1;

    if (procName != 0) {
        MPI_Recv(buf, count, type, MPI_ANY_SOURCE, MPI_ANY_TAG, comm, &status);
        while (twosDegree <= procName) {
            twosDegree = twosDegree << 1;
        }
    }

    while (procName + twosDegree <= procNum - 1) {
        int dest = getProcRank(root, procName + twosDegree, procNum);
        MPI_Send(buf, count, type, dest, 0, comm);
        twosDegree = twosDegree << 1;
    }
}
