// Copyright 2022 Semenova Veronika
#include <mpi.h>

#include <random>

#include "../../modules/task_2/semenova_a_gather/gather.h"


int Gather(void * sbuf, int scount, MPI_Datatype stype, void * rbuf,
  int rcount, MPI_Datatype rtype, int root, MPI_Comm comm) {
  if (stype != rtype || scount != rcount) return MPI_ERR_OTHER;
  if (sbuf == nullptr) return MPI_ERR_BUFFER;
  if (rcount < 0 || scount < 0) return MPI_ERR_COUNT;

  int type_size = 0;
  if (stype == MPI_INT) {
      type_size = sizeof(int);
  } else {
      if (stype == MPI_FLOAT) {
          type_size = sizeof(float);
      } else {
          if (stype == MPI_DOUBLE) {
              type_size = sizeof(double);
          } else {
              return -1;
           }
      }
  }
  int rank, ProcNum;
  MPI_Status status;
  MPI_Comm_rank(comm, & rank);
  MPI_Comm_size(comm, & ProcNum);

  char * res = static_cast < char * > (rbuf);
  char * given = static_cast < char * > (sbuf);

  char * given2 = new char[scount * ProcNum * type_size];
  for (int i = 0; i < scount * type_size; i++)
    given2[i] = given[i];
  for (int i = scount * type_size; i < scount * type_size * ProcNum; i++)
    given2[i] = 0;

  int n = ProcNum, i = 1, flag = 0;
  while (n > 1) {
    if (n % 2 == 1)
        flag = 1;
    if (rank % (i * 2) == i)
        MPI_Send(given2, scount * i, stype, rank - i, i, comm);
    if (rank % (i * 2) == 0 && rank < (n - 1) * i)
      MPI_Recv(given2 + (i) * rcount * type_size, i * rcount, rtype, rank + i, i, comm, & status);

    i = i * 2;
    n = (n + 1) / 2;
    flag = 0;
  }

  if (root != 0) {
    if (rank == 0)
      MPI_Send(given2, scount * ProcNum, stype, root, 1, comm);
    if (rank == root)
      MPI_Recv(res, rcount * ProcNum, rtype, 0, 1, comm, & status);
  } else {
    if (rank == root) {
      for (i = 0; i < scount * ProcNum * type_size; i++)
        res[i] = given2[i];
    }
  }

  return MPI_SUCCESS;
}
