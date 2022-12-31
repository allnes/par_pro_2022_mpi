// Copyright 2022 Alexandra Bikova
#ifndef MODULES_TASK_2_BIKOVA_A_REDUCE_REDUCE_H_
#define MODULES_TASK_2_BIKOVA_A_REDUCE_REDUCE_H_
#include<mpi.h>
#include<vector>
void DoWork(MPI_Op op, std::vector<int>* a, std::vector<int>* b);
void Reduce(int* sendbuf, int* recvbuf, int count, MPI_Datatype type, MPI_Op op, int root, MPI_Comm comm);
#endif  // MODULES_TASK_2_BIKOVA_A_REDUCE_REDUCE_H_
