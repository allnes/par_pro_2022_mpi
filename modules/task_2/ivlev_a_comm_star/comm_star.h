// Copyright 2022 Ivlev A
#ifndef  MODULES_TASK_2_IVLEV_A_COMM_STAR_COMM_STAR_H_
#define  MODULES_TASK_2_IVLEV_A_COMM_STAR_COMM_STAR_H_

#include <mpi.h>
#include <vector>
#include <string>


void MPI_group_star_create(MPI_Comm oldcomm, int nnodes,
    int index[], MPI_Comm* newcomm);
int Star_Send(const void *buf, int count, MPI_Datatype datatype,
    int from, int dest, int tag, MPI_Comm comm);


#endif  // MODULES_TASK_2_IVLEV_A_COMM_STAR_COMM_STAR_H_
