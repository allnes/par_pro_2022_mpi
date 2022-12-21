// Copyright 2022 Chernova Anna
#pragma once
#include <mpi.h>

#include <random>

const int MAX_INT = 100;
const int MIN_INT = 50;

const double MAX_DOUBLE = 32.0;
const double MIN_DOUBLE = -32.0;

const float MAX_FLOAT = -33.0;
const float MIN_FLOAT = -60.0;

void getRandomVector(int* arr, int size);
void getRandomVector(double* arr, int size);
void getRandomVector(float* arr, int size);

int chernovaGather(void* sendbuf, int sendcount, MPI_Datatype sendtype,
                     void* recvbuf, int recvcount, MPI_Datatype recvtype,
                     int root, MPI_Comm comm);
