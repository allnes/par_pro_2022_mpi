// Copyright 2022 Elistratov Vladimir
#include "../../../modules/task_2/elistratov_v_producers_consumers/elistratov_v_producers_consumers.h"

#include <mpi.h>

#include <ctime>
#include <iostream>
#include <queue>
#include <random>
#include <vector>

double sleep_time = 0.01;

void sleep_seconds(double seconds) {
  double t1 = MPI_Wtime();
  while (MPI_Wtime() - t1 < seconds) {
  }
}

int master(int Leap) {
  MPI_Status status;
  MPI_Request requestSend;
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int result;
  int last_consumer = -1;

  std::queue<int> queue;
  while (Leap != 0) {
    if (last_consumer == -1) {
      MPI_Recv(&result, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD,
               &status);
    } else {
      MPI_Recv(&result, 1, MPI_INT, MPI_ANY_SOURCE, 3, MPI_COMM_WORLD, &status);
    }
    --Leap;
    if (status.MPI_TAG == 3) {
      queue.push(result);
      std::cout << "PUT: " << result << " from " << status.MPI_SOURCE
                << std::endl;
      if (last_consumer != -1) {
        int elem = queue.front();
        queue.pop();
        MPI_Isend(&elem, 1, MPI_INT, last_consumer, 4, MPI_COMM_WORLD,
                  &requestSend);
        MPI_Request_free(&requestSend);
        last_consumer = -1;
      }
    } else {
      if (!queue.empty()) {
        int el = queue.front();
        queue.pop();
        MPI_Isend(&el, 1, MPI_INT, status.MPI_SOURCE, 4, MPI_COMM_WORLD,
                  &requestSend);
        MPI_Request_free(&requestSend);
      } else {
        last_consumer = status.MPI_SOURCE;
      }
    }
  }
  return queue.size();
}

void produce() {
  int rank;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  sleep_seconds(sleep_time * rank);
  MPI_Send(&rank, 1, MPI_INT, 0, 3, MPI_COMM_WORLD);
}

void consume() {
  int rank, elem;
  MPI_Status statusGet;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  sleep_seconds(sleep_time * rank);
  MPI_Send(&rank, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
  MPI_Recv(&elem, 1, MPI_INT, 0, 4, MPI_COMM_WORLD, &statusGet);
  // std::cout << "GET: " << elem << " by " << rank << std::endl;
}
