// Copyright 2022 Bochkarev Vladimir
#ifndef MODULES_TASK_2_BOCHKAREV_V_LINEAR_LINEAR_H_
#define MODULES_TASK_2_BOCHKAREV_V_LINEAR_LINEAR_H_

#include <mpi.h>
#include <algorithm>

void getNext(int* next, MPI_Comm comm, int source);
void getPrev(int* prev, MPI_Comm comm, int source);
void send(void* mes, int count, MPI_Datatype type, int source, int dest, int tag, MPI_Comm comm);

#endif  // MODULES_TASK_2_BOCHKAREV_V_LINEAR_LINEAR_H_
