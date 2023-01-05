// Copyright 2022 Bulgakov Daniil

#include <mpi.h>
#include <vector>
#include <random>
#include <cstring>
#include <iostream>
#include <cmath>
#include "../../../modules/task_2/bulgakov_d_gather/gather_mpi.h"

int convert_back(int rank, int root, int size) {
    return (rank + root) % size;
}

int convert_rank(int rank, int root, int size) {
    return (rank >= root) ? rank - root : rank - root + size;
}

int MPI_Own_Gather(void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf,
               int recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm) {
    int MPI_GATHER_TAG = 4023;
    int comm_size, rank;
    int rel_rank;
    char * local_buff;
    int nbytes;
    int offset = 0;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &comm_size);

    // Return if gather can not be done
    if (((rank == root) && (recvbuf == 0)) ||
        ((rank != root) && (sendbuf == 0)))
        return MPI_ERR_NO_MEM;


    // Create indecies for binomial tree. root for tree is a root rank
    rel_rank = convert_rank(rank, root, comm_size);

    // Allocate space for all nodes that is note leaves
    // And copy their parts at the start of buffer
    if (((rel_rank % 2 == 0) && (rel_rank != 0)) || (rel_rank == 1)) {
        int mpi_type;
        MPI_Type_size(sendtype, &mpi_type);
        nbytes = (mpi_type * comm_size * sendcount) / 2;
        local_buff = new char[nbytes];
        offset = mpi_type * sendcount;
        memcpy(local_buff, sendbuf, offset);
    }

    // Allocate space for root local_buf depends if its rank = 0
    if (rel_rank == 0) {
        int mpi_type;
        MPI_Type_size(sendtype, &mpi_type);
        nbytes = mpi_type * comm_size * recvcount;
        offset = mpi_type * sendcount;
        if (root == 0) {
            local_buff = reinterpret_cast<char*>(recvbuf);
        } else {
            local_buff = new char[nbytes];
        }
        memcpy(local_buff, sendbuf, offset);
    }

    // Make send and recieves depends on bit in a rel_rank
    // Example : rel_rank = 4 (0x100)
    // Looping by bits in a value from right to left
    // bit = [1, 0, 0]
    // If bit[2] == 0 => wait for recieve from a proc with rel_rank 0x101
    // If bit[1] == 0 => wait for recieve from a proc with rel_rank 0x110
    // If bit[0] == 1 => make Send to proc with rel_rank 0x100 ^ 0x100 = 0x000
    // Example : rel rank = 6 (0x110)
    // If bit[1] == 1 => make Send to proc with rel_rank 0x100 ^ 0x010 = 0x100
    if (rel_rank % 2 == 1 && rel_rank != 1) {
        MPI_Send(sendbuf, sendcount, sendtype, convert_back(rel_rank ^ 1, root, comm_size), MPI_GATHER_TAG, comm);
    } else if (rel_rank != 0) {
        int iter = 0x1;
        MPI_Status status;
        for (iter; iter < comm_size; iter = iter << 1) {
            if ((iter & rel_rank) == 0 && ((rel_rank | iter) < comm_size)) {
                MPI_Recv(local_buff + offset, (nbytes * (iter)), MPI_BYTE,
                    convert_back(rel_rank | iter, root, comm_size), MPI_GATHER_TAG, comm, &status);
                int add_off;
                MPI_Get_count(&status, MPI_BYTE, &add_off);
                offset += add_off;
            } else if ((rel_rank ^ iter) < comm_size) {
                MPI_Send(local_buff, offset, MPI_BYTE,
                    convert_back(rel_rank ^ iter, root, comm_size), MPI_GATHER_TAG, comm);
                delete[] local_buff;
                break;
            }
        }

    } else if (rel_rank == 0) {
        int iter = 0x1;
        MPI_Status status;
        int mpi_type;
        MPI_Type_size(sendtype, &mpi_type);
        for (iter; iter < comm_size; iter = iter << 1) {
                MPI_Recv(local_buff + offset, ((mpi_type * recvcount) * (iter)), MPI_BYTE,
                    convert_back(iter, root, comm_size), MPI_GATHER_TAG, comm, &status);
                int add_off;
                MPI_Get_count(&status, MPI_BYTE, &add_off);
                offset += add_off;
        }
        if (root != 0) {
            char * send_buf = reinterpret_cast<char *>(recvbuf);
            int split = root;
            memcpy(send_buf + recvcount * mpi_type * (split), local_buff, recvcount * mpi_type * (comm_size - split) );
            memcpy(send_buf , local_buff + recvcount * mpi_type * (comm_size - split), recvcount * mpi_type * (split) );
            delete[] local_buff;
        }
    }

    return MPI_SUCCESS;
}
