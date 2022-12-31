// Copyright 2022 Yurin Evgeny
#include "../../../modules/task_2/yurin_e_tor/tor.h"

#include <mpi.h>

#include <map>

Mesh createMesh() {
  Mesh mesh;
  MPI_Comm_rank(MPI_COMM_WORLD, &mesh.rank);
  MPI_Comm_size(MPI_COMM_WORLD, &mesh.size);

  mesh.row = mesh.rank / N;
  mesh.column = mesh.rank % N;

  MPI_Comm_split(MPI_COMM_WORLD, mesh.row, mesh.rank, &mesh.rowComm);
  MPI_Comm_split(MPI_COMM_WORLD, mesh.column, mesh.rank, &mesh.colComm);

  mesh.north = (mesh.row > 0) ? mesh.rank - N : MPI_PROC_NULL;
  mesh.south = (mesh.row < N - 1) ? mesh.rank + N : MPI_PROC_NULL;
  mesh.west = (mesh.column > 0) ? mesh.rank - 1 : MPI_PROC_NULL;
  mesh.east = (mesh.column < N - 1) ? mesh.rank + 1 : MPI_PROC_NULL;
  return mesh;
}

int getNext(int from, int dest) {
  if (from == dest) {
    return from;
  }
  int rowDiff = abs(from / N - dest / N);
  int columnDiff = abs(from % N - dest % N);
  if (rowDiff > 0) {
    return (from < dest) ? from + N : from - N;
  } else if (columnDiff > 0) {
    return (from < dest) ? from + 1 : from - 1;
  } else {
    return -1;
  }
}

void MeshSend(void *data, int length, MPI_Datatype datatype, int from, int dest,
              int tag, Mesh *mesh) {
  int next = getNext(from, dest);
  meshSendInternal(data, length, datatype, from, next, dest, tag, mesh);
}

void meshSendInternal(void *data, int length, MPI_Datatype datatype, int sender,
                      int receiver, int final, int tag, Mesh *mesh) {
  if ((*mesh).rank == sender) {
    MPI_Send(data, length, datatype, receiver, tag, MPI_COMM_WORLD);
  }
  if ((*mesh).rank == receiver) {
    MPI_Status status;
    MPI_Recv(data, length, datatype, sender, tag, MPI_COMM_WORLD, &status);
  }
  if (receiver == final) {
    return;
  }
  int next = getNext(receiver, final);
  meshSendInternal(data, length, datatype, receiver, next, final, tag, mesh);
}
