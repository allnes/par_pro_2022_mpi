// Copyright 2022 Kolesov Maxim
#pragma once
#include <mpi.h>
#include <vector>

void RingSend(void *data, int length, MPI_Datatype datatype, int from, int dest, int tag, MPI_Comm comm);
int ChooseDirection(int from, int dest, MPI_Comm comm, std::vector<int> *ranks);
