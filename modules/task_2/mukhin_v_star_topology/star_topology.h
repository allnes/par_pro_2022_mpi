// Copyright 2022 MUKHIN VADIM
#ifndef MODULES_TASK_2_MUKHIN_V_STAR_TOPOLOGY_STAR_TOPOLOGY_H_
#define MODULES_TASK_2_MUKHIN_V_STAR_TOPOLOGY_STAR_TOPOLOGY_H_

#include <mpi.h>
#include <vector>

MPI_Comm Star(const MPI_Comm comm);
bool IsStar(const MPI_Comm star);

#endif  //  MODULES_TASK_2_MUKHIN_V_STAR_TOPOLOGY_STAR_TOPOLOGY_H_
