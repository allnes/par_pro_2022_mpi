// Copyright 2022 Chernova Anna
#include "../../modules/task_2/chernova_a_gather/gather.h"

void random(int* arr, int size) {
  std::random_device rd;
  std::mt19937 RNG(rd());
  std::uniform_int_distribution<int> dist(MIN_RANDOM_INT, MAX_RANDOM_INT);
  for (int i = 0; i < size; i++) {
    arr[i] = dist(RNG);
  }
}

void random(double* arr, int size) {
  std::random_device rd;
  std::mt19937 RNG(rd());
  std::uniform_real_distribution<double> dist(MIN_RANDOM_DOUBLE,
                                              MAX_RANDOM_DOUBLE);
  for (int i = 0; i < size; i++) {
    arr[i] = dist(RNG);
  }
}

void random(float* arr, int size) {
  std::random_device rd;
  std::mt19937 RNG(rd());
  std::uniform_real_distribution<float> dist(MIN_RANDOM_FLOAT,
                                             MAX_RANDOM_FLOAT);
  for (int i = 0; i < size; i++) {
    arr[i] = dist(RNG);
  }
}

int my_gather(void* sendbuf, int sendcount, MPI_Datatype sendtype,
              void* recvbuf, int recvcount, MPI_Datatype recvtype, int root,
              MPI_Comm comm) {
  if (sendtype != recvtype || sendcount != recvcount) return MPI_ERR_OTHER;

  int rank, numProc;
  MPI_Comm_rank(comm, &rank);
  MPI_Comm_size(comm, &numProc);

  if (recvtype == MPI_INT) {
    int* buffer = static_cast<int*>(recvbuf);
    int* sBuffer = static_cast<int*>(sendbuf);
    if (rank == root) {
      for (int i = root * recvcount; i < (root + 1) * recvcount; i++) {
        buffer[i] = sBuffer[i - root * recvcount];
      }
      for (int i = 0; i < numProc; i++) {
        if (i == root) {
          continue;
        }
        MPI_Recv(buffer + i * recvcount, recvcount, MPI_INT, i, MPI_ANY_TAG,
                 comm, MPI_STATUSES_IGNORE);
      }
    } else {
      MPI_Send(sBuffer, sendcount, MPI_INT, root, 0, comm);
    }
  }

  if (recvtype == MPI_DOUBLE) {
    double* buffer = static_cast<double*>(recvbuf);
    double* sBuffer = static_cast<double*>(sendbuf);
    if (rank == root) {
      for (int i = root * recvcount; i < (root + 1) * recvcount; i++) {
        buffer[i] = sBuffer[i - root * recvcount];
      }
      for (int i = 0; i < numProc; i++) {
        if (i == root) {
          continue;
        }
        MPI_Recv(buffer + i * recvcount, recvcount, MPI_DOUBLE, i, MPI_ANY_TAG,
                 comm, MPI_STATUSES_IGNORE);
      }
    } else {
      MPI_Send(sBuffer, sendcount, MPI_DOUBLE, root, 0, comm);
    }
  }

  if (recvtype == MPI_FLOAT) {
    float* buffer = static_cast<float*>(recvbuf);
    float* sBuffer = static_cast<float*>(sendbuf);
    if (rank == root) {
      for (int i = root * recvcount; i < (root + 1) * recvcount; i++) {
        buffer[i] = sBuffer[i - root * recvcount];
      }
      for (int i = 0; i < numProc; i++) {
        if (i == root) {
          continue;
        }
        MPI_Recv(buffer + i * recvcount, recvcount, MPI_FLOAT, i, MPI_ANY_TAG,
                 comm, MPI_STATUSES_IGNORE);
      }
    } else {
      MPI_Send(sBuffer, sendcount, MPI_FLOAT, root, 0, comm);
    }
  }

  return MPI_SUCCESS;
}
