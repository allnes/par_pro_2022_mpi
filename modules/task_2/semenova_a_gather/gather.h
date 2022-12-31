// Copyright 2022 Semenova Veronika
#ifndef MODULES_TASK_2_SEMENOVA_A_GATHER_GATHER_H_
#define MODULES_TASK_2_SEMENOVA_A_GATHER_GATHER_H_
#pragma once

#include <mpi.h>

#include<random>

template < typename Type >
  void randVec(Type * A, int n) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution < int > dist(0, 10);
    for (int i = 0; i < n; i++) {
      A[i] = dist(gen);
    }
  }

int Gather(void * sbuf, int scount, MPI_Datatype stype, void * rbuf,
  int rcount, MPI_Datatype rtype, int root, MPI_Comm comm);

#endif  // MODULES_TASK_2_SEMENOVA_A_GATHER_GATHER_H_
