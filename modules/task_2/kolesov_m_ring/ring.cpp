// Copyright 2022 Kolesov Maxim
#include <mpi.h>
#include <vector>
#include <algorithm>

#include "../../../modules/task_2/kolesov_m_ring/ring.h"

void GetNextPrev(int *next, int *prev, MPI_Comm comm, int from) {
  int size;

  MPI_Comm_size(comm, &size);

  *next = from + 1;
  *prev = from - 1;

  if (from == size - 1) {
    *next = 0;
  } else if (from == 0) {
    *prev = size - 1;
  }
}

void RingSend(void *data, int length, MPI_Datatype datatype, int from, int dest, int tag, MPI_Comm comm) {
  int rank, size;

  MPI_Comm_size(comm, &size);
  MPI_Comm_rank(comm, &rank);

  std::vector<int> ranks;

  int dir = ChooseDirection(from, dest, comm, &ranks);
  int next, previous;

  if (std::find(ranks.begin(), ranks.end(), rank) == ranks.end()) {
    return;
  }

  GetNextPrev(&next, &previous, comm, rank);

  if (dir < 0) {
    std::swap(next, previous);
  }

  if (rank == from) {
    MPI_Send(data, length, datatype, next, tag, comm);
    return;
  }

  MPI_Status status;
  MPI_Recv(data, length, datatype, previous, tag, comm, &status);

  if (rank != dest) {
    MPI_Send(data, length, datatype, next, tag, comm);
  }
}

int ChooseDirection(int from, int dest, MPI_Comm comm, std::vector<int> *ranks) {
  int count = 1;
  int next, prev;
  int size;

  std::vector<int> nextVector;
  std::vector<int> prevVector;

  nextVector.push_back(from);
  prevVector.push_back(from);

  MPI_Comm_size(comm, &size);
  GetNextPrev(&next, &prev, comm, from);

  if (from == dest) {
    return 0;
  }

  while (next != dest) {
    nextVector.push_back(next);
    count++;
    if (next == size - 1) {
      next = 0;
      continue;
    }

    next++;
  }

  nextVector.push_back(next);

  int buf = count;
  count = 1;

  GetNextPrev(&next, &prev, comm, from);


  while (prev != dest) {
    count++;
    prevVector.push_back(prev);
    if (prev == 0) {
      prev = size - 1;
      continue;
    }

    prev--;
  }

  prevVector.push_back(prev);

  if (buf <= count) {
    ranks->clear();
    ranks->insert(ranks->end(), nextVector.begin(), nextVector.end());
    return 1;
  }

  ranks->clear();
  ranks->insert(ranks->end(), prevVector.begin(), prevVector.end());
  return -1;
}
