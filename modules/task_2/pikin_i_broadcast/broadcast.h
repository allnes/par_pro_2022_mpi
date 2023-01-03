// Copyright 2022 Pikin Ilya
#ifndef MODULES_TASK_2_PIKIN_I_BROADCAST_BROADCAST_H_
#define MODULES_TASK_2_PIKIN_I_BROADCAST_BROADCAST_H_

#include <mpi.h>

int getProcName(int root, int procRank, int procNum);
int getProcRank(int root, int procName, int procNum);
void myBroadcast(void* buf, int count, MPI_Datatype type,
    int root, MPI_Comm comm);

#endif  // MODULES_TASK_2_PIKIN_I_BROADCAST_BROADCAST_H_
