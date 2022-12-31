// Copyright 2022 Uvarkin Ilya
#pragma once
#include <mpi.h>
#include <iostream>
#include <cmath>
#include <map>

void HypercubeSend(void *data, int length, MPI_Datatype datatype, int from, int dest, int tag, MPI_Comm comm);
void hypercubeSendInternal(void *data, int length, MPI_Datatype datatype, int from,
                           int dest, int final, int tag, MPI_Comm comm);
int getNext(int from, int dest);
std::map<int, int> CreateHypercube(int procNum);
int getDimension(int procNum);
