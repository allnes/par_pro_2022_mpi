// Copyright 2022 Kruglikova Valeriia
#include "../../../modules/task_2/kruglikova_v_allreducee/allreducee.h"
#include <cmath>
#include <cstring>
#include <stdexcept>

int Allreduce(void* sendbuf, void* recvbuf, int count, MPI_Datatype datatype,
  MPI_Op op, MPI_Comm comm) {
  if ((sendbuf == nullptr) || (recvbuf == nullptr))
  throw std::runtime_error("Send or receive null pointer.");
  // The memory buffer with the message being sent and the memory buffer for the resulting message null pointer

  if (count == 0) throw std::runtime_error("Is empty.");
  // Message buffer is empty

  if (datatype != MPI_INT && datatype != MPI_FLOAT && datatype != MPI_DOUBLE)
  throw std::runtime_error("Error");
  // Invalid data type

  if (op != MPI_SUM) throw std::runtime_error("Error");
  // Invalid operation
  if (op != MPI_SUM) throw std::runtime_error("Incorrect operation.");

  int rank, size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (datatype == MPI_INT) std::memcpy(recvbuf, sendbuf, count * sizeof(int));
  if (datatype == MPI_FLOAT)
    std::memcpy(recvbuf, sendbuf, count * sizeof(float));
  if (datatype == MPI_DOUBLE)
    std::memcpy(recvbuf, sendbuf, count * sizeof(double));

  int delta;
  int exp = static_cast<int>(std::log2(size));
  for (int i = 0; i <= exp; ++i) {
    delta = static_cast<int>(std::pow(2, i));
    if (size > delta) {
      for (int proc = delta; proc < size;
        proc += static_cast<int>(std::pow(2, i + 1))) {
        if (rank == proc)
          MPI_Send(recvbuf, count, datatype, proc - delta, rank, comm);

        if (rank == proc - delta) {
          MPI_Status status;
          if (datatype == MPI_INT) {
            int* getbuf = new int[count];
            MPI_Recv(getbuf, count, datatype, proc, proc, comm, &status);
            for (int j = 0; j < count; ++j)
              static_cast<int*>(recvbuf)[j] += getbuf[j];
          } else if (datatype == MPI_FLOAT) {
            float* getbuf = new float[count];
            MPI_Recv(getbuf, count, datatype, proc, proc, comm, &status);
            for (int j = 0; j < count; ++j)
              static_cast<float*>(recvbuf)[j] += getbuf[j];
          } else if (datatype == MPI_DOUBLE) {
            double* getbuf = new double[count];
            MPI_Recv(getbuf, count, datatype, proc, proc, comm, &status);
            for (int j = 0; j < count; ++j)
              static_cast<double*>(recvbuf)[j] += getbuf[j];
          }
        }
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
