// Copyright 2022 Popov Andrey
#ifndef MODULES_TASK_2_POPOV_A_FROM_ONE_TO_ALL_BROADCATS_FROM_ONE_TO_ALL_BROADCATS_H_
#define MODULES_TASK_2_POPOV_A_FROM_ONE_TO_ALL_BROADCATS_FROM_ONE_TO_ALL_BROADCATS_H_

#include<mpi.h>

int* getRandArrInt(int Arrsize);
double* getRandArrDouble(int Arrsize);
float* getRandArrFloat(int Arrsize);

int getMaxArrInt(int* buf, int Arrsize);
double getMaxArrDouble(double* buf, int Arrsize);
float getMaxArrFloat(float* buf, int Arrsize);

void Broadcast(void* buffer, int cnt, MPI_Datatype type, int root,
                 MPI_Comm comm);

#endif  // MODULES_TASK_2_POPOV_A_FROM_ONE_TO_ALL_BROADCATS_FROM_ONE_TO_ALL_BROADCATS_H_
