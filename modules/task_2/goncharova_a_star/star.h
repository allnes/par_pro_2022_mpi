// Copyright Anna Goncharova

#ifndef MODULES_TASK_2_GONCHAROVA_A_STAR_STAR_H_
#define MODULES_TASK_2_GONCHAROVA_A_STAR_STAR_H_
#include <mpi.h>
#include <iostream>
MPI_Comm createStarComm(const MPI_Comm oldComm);
bool isStarTopology(const MPI_Comm curComm);

#endif  // MODULES_TASK_2_GONCHAROVA_A_STAR_STAR_H_
