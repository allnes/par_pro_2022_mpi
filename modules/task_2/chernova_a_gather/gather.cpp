// Copyright 2022 Chernova Anna
#include "../../modules/task_2/chernova_a_gather/gather.h"

void getRandomVector(int* arr, int size) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> dist(MIN_INT, MAX_INT);
  for (int i = 0; i < size; i++) {
    arr[i] = dist(gen);
  }
}
void getRandomVector(double* arr, int size) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<double> dist(MIN_DOUBLE, MAX_DOUBLE);
  for (int i = 0; i < size; i++) {
    arr[i] = dist(gen);
  }
}
void getRandomVector(float* arr, int size) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<float> dist(MIN_FLOAT, MAX_FLOAT);
  for (int i = 0; i < size; i++) {
    arr[i] = dist(gen);
  }
}

int chernovaGather(void* sendbuf, int sendcount, MPI_Datatype sendtype,
                     void* recvbuf, int recvcount, MPI_Datatype recvtype,
                     int root, MPI_Comm comm) {
  if (sendtype != recvtype || sendcount != recvcount) return MPI_ERR_OTHER;

  int rank, numProc;
  MPI_Comm_rank(comm, &rank);
  MPI_Comm_size(comm, &numProc);

  char* recvBuffer = static_cast<char*>(recvbuf);
  char* sendBuffer = static_cast<char*>(sendbuf);
  int type_size = 0;
  if (sendtype == MPI_INT) type_size = sizeof(int);
  if (sendtype == MPI_DOUBLE) type_size = sizeof(double);
  if (sendtype == MPI_FLOAT) type_size = sizeof(float);

  if (rank == root) {
    for (int i = root * recvcount * type_size;
         i < (root + 1) * recvcount * type_size; i++) {
      recvBuffer[i] = sendBuffer[i - root * (recvcount * type_size)];
    }
    for (int i = 0; i < numProc; i++) {
      if (i == root) {
        continue;
      }
      MPI_Recv(recvBuffer + (i * recvcount * type_size), recvcount, recvtype, i,
               MPI_ANY_TAG, comm, MPI_STATUSES_IGNORE);
    }
  } else {
    MPI_Send(sendBuffer, sendcount, sendtype, root, 0, comm);
  }

  return MPI_SUCCESS;
}
