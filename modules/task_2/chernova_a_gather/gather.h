// Copyright 2022 Chernova Anna
#pragma once
#include <mpi.h>

#include <random>

const int MAX_RANDOM_INT = 128;
const int MIN_RANDOM_INT = -128;

const double MAX_RANDOM_DOUBLE = 16.0;
const double MIN_RANDOM_DOUBLE = -16.0;

const float MAX_RANDOM_FLOAT = 16.0;
const float MIN_RANDOM_FLOAT = -16.0;

void random(int* arr, int size);
void random(double* arr, int size);
void random(float* arr, int size);

int my_gather(void* sendbuf, int sendcount, MPI_Datatype sendtype,
              void* recvbuf, int recvcount, MPI_Datatype recvtype, int root,
              MPI_Comm comm);
