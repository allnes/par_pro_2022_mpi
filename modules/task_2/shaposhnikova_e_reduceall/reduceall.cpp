// Copyright 2022 Shaposhnikova Ekaterina
#include "../../../modules/task_2/shaposhnikova_e_reduceall/reduceall.h"
#include <cmath>
#include <cstring>
#include <stdexcept>

int reduceAll(void* sendbuf, void* recvbuf, int count, MPI_Datatype datatype,
  MPI_Op op, MPI_Comm comm) {
  int rank, size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (datatype == MPI_INT) std::memcpy(recvbuf, sendbuf, count * sizeof(int));
  if (datatype == MPI_FLOAT)
    std::memcpy(recvbuf, sendbuf, count * sizeof(float));
  if (datatype == MPI_DOUBLE)
    std::memcpy(recvbuf, sendbuf, count * sizeof(double));

  int s = size - 1;
  int z = 0;
  for (int i = s; i > 0; i--) {
    if (i == s / 2) {
      s = s / 2;
    }
    if (rank == i)
      MPI_Send(recvbuf, count, datatype, s-i, rank, comm);
    if (rank == s-i) {
      MPI_Status status;

      if (datatype == MPI_INT) {
        int* getbuf = new int[count];

        MPI_Recv(getbuf, count, datatype, i, i, comm, &status);
        for (int j = 0; j < count; j++)
          static_cast<int*>(recvbuf)[j] += getbuf[j];
      } else if (datatype == MPI_FLOAT) {
        float* getbuf = new float[count];
        MPI_Recv(getbuf, count, datatype, i, i, comm, &status);
        for (int j = 0; j < count; j++)
          static_cast<float*>(recvbuf)[j] += getbuf[j];
      } else if (datatype == MPI_DOUBLE) {
        double* getbuf = new double[count];
        MPI_Recv(getbuf, count, datatype, i, i, comm, &status);
        for (int j = 0; j < count; j++)
          static_cast<double*>(recvbuf)[j] += getbuf[j];
      }
    }
  }
  if (rank == 0) {
    for (int proc = 1; proc < size; ++proc)
      MPI_Send(recvbuf, count, datatype, proc, proc + size, comm);
  } else {
    MPI_Status status;
    MPI_Recv(recvbuf, count, datatype, 0, rank + size, comm, &status);
  }
  return MPI_SUCCESS;
}
