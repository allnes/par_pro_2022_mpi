// Copyright 2022 Yurin Evgeny
#ifndef MODULES_TASK_2_YURIN_E_TOR_TOR_H_
#define MODULES_TASK_2_YURIN_E_TOR_TOR_H_
#include <mpi.h>

const int N = 5;

struct Mesh {
  int rank, size;
  int row, column;
  MPI_Comm rowComm, colComm;
  int north, south, east, west;
};

Mesh createMesh();
void MeshSend(void *data, int length, MPI_Datatype datatype, int from, int dest,
              int tag, Mesh *mesh);
void meshSendInternal(void *data, int length, MPI_Datatype datatype, int from,
                      int dest, int final, int tag, Mesh *mesh);
int getNext(int from, int dest);
#endif  // MODULES_TASK_2_YURIN_E_TOR_TOR_H_

