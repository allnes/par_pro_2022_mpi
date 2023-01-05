// Copyright 2022 Shaposhnikova Ekaterina
#ifndef MODULES_TASK_2_SHAPOSHNIKOVA_E_REDUCEALL_REDUCEALL_H_
#define MODULES_TASK_2_SHAPOSHNIKOVA_E_REDUCEALL_REDUCEALL_H_

#include <mpi.h>

int reduceAll(void* sendbuf, void* recvbuf, int count, MPI_Datatype datatype,
  MPI_Op op, MPI_Comm comm);

#endif  // MODULES_TASK_2_SHAPOSHNIKOVA_E_REDUCEALL_REDUCEALL_H_
