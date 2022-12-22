// Copyright 2022 Bulgakov Daniil

#ifndef MODULES_TASK_2_BULGAKOV_D_GATHER_GATHER_MPI_H_
#define MODULES_TASK_2_BULGAKOV_D_GATHER_GATHER_MPI_H_

#include <mpi.h>
#include <vector>
#include <string>

int MPI_Own_Gather(void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf,
               int recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm);



#endif  // MODULES_TASK_2_BULGAKOV_D_GATHER_GATHER_MPI_H_
