// Copyright 2022 Uvarkin Ilya
#include "../../../modules/task_2/uvarkin_i_hypercube/hypercube.h"

int getDimension(int procNum) {
    int dimension = 0;

    if (procNum < 0) {
        return -1;
    }

    while (procNum > 1) {
        if (procNum % 2 == 1) {
            return -1;
        }

        procNum /= 2;
        dimension++;
    }

    return dimension;
}

std::map<int, int> CreateHypercube(int procNum) {
    std::map<int, int> hypercube{};
    int dimension = getDimension(procNum);

    if (dimension < 0) {
        return hypercube;
    }

    int count = static_cast<int>(std::pow(2, dimension));
    for (int i = 0; i < count; ++i) {
        hypercube[i] = i;
    }

    return hypercube;
}

int getNext(int from, int dest) {
    if (from == dest) {
        return from;
    }

    int mask = 1;
    while (true) {
        if ((from & mask) == (dest & mask)) {
            mask = mask << 1;
            continue;
        }

        if ((from & mask) == 0) {
            return from + mask;
        } else {
            return from - mask;
        }
    }
}

void HypercubeSend(void *data, int length, MPI_Datatype datatype, int from, int dest, int tag, MPI_Comm comm) {
    int next = getNext(from, dest);
    hypercubeSendInternal(data, length, datatype, from, next, dest, tag, comm);
}

void hypercubeSendInternal(void *data, int length, MPI_Datatype datatype, int from,
                           int dest, int final, int tag, MPI_Comm comm) {
    int procRank;
    MPI_Comm_rank(comm, &procRank);

    if (procRank == from) {
        MPI_Send(data, length, datatype, dest , tag, comm);
    }

    if (procRank == dest) {
        MPI_Status status;

        MPI_Recv(data, length, datatype, from, tag, comm, &status);
    }

    if (dest == final) {
        return;
    }

    int next = getNext(dest, final);
    hypercubeSendInternal(data, length, datatype, dest, next, final, tag, comm);
}
