// Copyright 2022 Popov Andrey
#include "../../../modules/task_2/Popov_A_from_one_to_all_broadcats/from_one_to_all_broadcats.h"
#include <mpi.h>
#include<stdexcept>
#include<ctime>
#include<iostream>
#include <random>
#include <algorithm>



int* getRandArrInt(int Arrsize) {
  if (Arrsize <= 0) throw std::runtime_error("Error Array size");
  std::mt19937 gen;
  gen.seed(static_cast<unsigned int>(time(0)));
  int* lclBf = reinterpret_cast<int*>(malloc(sizeof(int) * Arrsize));
  for (int j = 0; j < Arrsize; j++) {
    lclBf[j] = gen() % 100;
  }
  return lclBf;
}

double* getRandArrDouble(int Arrsize) {
  if (Arrsize <= 0) throw std::runtime_error("Error Array size");
  std::mt19937 gen;
  gen.seed(static_cast<unsigned int>(time(0)));
  double* lclBf = reinterpret_cast<double*>(malloc(sizeof(double) * Arrsize));
  for (int j = 0; j < Arrsize; j++) {
    lclBf[j] = gen() % 100;
  }
  return lclBf;
}

float* getRandArrFloat(int Arrsize) {
  if (Arrsize <= 0) throw std::runtime_error("Error Array size");
  std::mt19937 gen;
  gen.seed(static_cast<unsigned int>(time(0)));
  float* lclBf = reinterpret_cast<float*>(malloc(sizeof(float) * Arrsize));
  for (int j = 0; j < Arrsize; j++) {
    lclBf[j] = gen() % 100;
  }
  return lclBf;
}

int getMaxArrInt(int* buf, int Arrsize) {
  int max = buf[0];
  for (int j = 0; j < Arrsize; j++) {
    if (buf[j] > max) {
      max = buf[j];
    }
  }
  return max;
}

double getMaxArrDouble(double* buf, int Arrsize) {
  double max = buf[0];
  for (int j = 0; j < Arrsize; j++) {
    if (buf[j] > max) {
      max = buf[j];
    }
  }
  return max;
}

float getMaxArrFloat(float* buf, int Arrsize) {
  float max = buf[0];
  for (int j = 0; j < Arrsize; j++) {
    if (buf[j] > max) {
      max = buf[j];
    }
  }
  return max;
}

void Broadcast(void* buffer, int cnt, MPI_Datatype type, int root,
                 MPI_Comm comm) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (cnt <= 0) {
    throw std::runtime_error("Error Array size");
  }

  if (rank == root) {
    for (int i = 0; i < size; i++) {
      if (i != rank) {
        MPI_Send(buffer, cnt, type, i, 1, comm);
      }
    }
  } else {
    MPI_Status Status;
    MPI_Recv(buffer, cnt, type, root, 1, comm, &Status);
  }
}
