// Copyright 2022 Semenova Veronika
#include <mpi.h>

#include <random>

#include "../../../modules/task_1/semenova_alter_sign/alter_sign.h"

void RandVec(int * V, int n) {
  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_int_distribution < std::mt19937::result_type > dist(1, 0xFFFFFFFF);
  if (V == nullptr)
    throw "vector is not allocated";
  for (int i = 0; i < n; i++) {
    do {
      V[i] = static_cast < int > (dist(rng)) * ((static_cast < int > (dist(rng)) & 0x01) ? -1 : 1);
    } while (!V[i]);
  }
}

int SerialSum(const int * V, int n) {
  int sum = 0;
  for (int i = 0; i < n - 1; i++) {
    if (V[i] * V[i + 1] < 0)
      sum++;
  }
  return sum;
}

int ParallelSum(const int * V, int n) {
  int ProcNum = 0, rank = 0, i = 0, j = 0;
  MPI_Status status;

  MPI_Comm_size(MPI_COMM_WORLD, & ProcNum);
  MPI_Comm_rank(MPI_COMM_WORLD, & rank);

  if (n < ProcNum * 2) {
    if (rank == 0)
      return SerialSum(V, n);
    else
      return 0;
  }

  int nP = n / ProcNum;
  if (rank < n % ProcNum)
    nP++;
  if (rank < ProcNum - 1)
    nP++;
  int * v2 = new int[nP];

  if (rank == 0) {
    int nD = n / ProcNum;
    for (i = 0; i < nP; i++)
      v2[i] = V[i];
    for (i = 1; i < n % ProcNum; i++)
      MPI_Send(& V[(nD + 1) * i], nD + 2, MPI_INT, i, 0, MPI_COMM_WORLD);
    int x = (nD + 1) * (i - 1) + (nP - 1);
    int j = 0;
    for (i; i < ProcNum - 1; i++) {
      MPI_Send(& V[x + j * (nD)], (nD + 1), MPI_INT, i, 0, MPI_COMM_WORLD);
      j++;
    }
    if (i == ProcNum - 1)
      MPI_Send(& V[x + j * (nD)], (nD), MPI_INT, i, 0, MPI_COMM_WORLD);

  } else {
    MPI_Recv(v2, nP, MPI_INT, 0, 0, MPI_COMM_WORLD, & status);
  }

  int SumG = 0;
  int SumL = SerialSum(v2, nP);
  MPI_Reduce(& SumL, & SumG, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  delete[] v2;
  return SumG;
}
