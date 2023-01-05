// Copyright 2022 Makarov Danila
#include "../../../modules/task_2/makarov_d_sleeping_barber/sleeping_barber.h"

#include <mpi.h>

#include <chrono> // NOLINT [build/c++11]
#include <iostream>
#include <queue>
#include <thread> // NOLINT [build/c++11]

void client() {
  MPI_Status status;
  int ProcRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  int res = -1;

  while (res == -1) {
    MPI_Send(&ProcRank, 1, MPI_INT, 1, WROOM_REQUEST, MPI_COMM_WORLD);
    MPI_Recv(&res, 1, MPI_INT, 1, WROOM_RESPONSE, MPI_COMM_WORLD, &status);
    if (res == -1) std::this_thread::sleep_for(std::chrono::milliseconds(15));
    // if (res == -1) std::cout << "Client go away " << ProcRank << std::endl;
  }
}

void waitingRoom(int seat_count, int client_count) {
  MPI_Status status;

  std::queue<int> q;
  int req, res;
  int reqBarber;
  int resBarber = -1;

  double t1 = 0.;
  int i = 0;

  std::this_thread::sleep_for(std::chrono::milliseconds(30));

  while ((i < client_count) || (q.size() != 0)) {
    std::this_thread::sleep_for(std::chrono::milliseconds(5));

    if (i < client_count) {
      MPI_Recv(&req, 1, MPI_INT, MPI_ANY_SOURCE, WROOM_REQUEST, MPI_COMM_WORLD,
               &status);
      if (q.size() >= 0 && q.size() != seat_count) {
        res = 0;
        MPI_Send(&res, 1, MPI_INT, req, WROOM_RESPONSE, MPI_COMM_WORLD);
        q.push(req);
        // std::cout << "Process added to queue " << req << std::endl;
        i++;
      } else {
        res = -1;
        MPI_Send(&res, 1, MPI_INT, req, WROOM_RESPONSE, MPI_COMM_WORLD);
      }
    }

    // std::cout << "Wtime - t1 is " << MPI_Wtime() - t1 << std::endl;
    if (q.size() != 0 && (MPI_Wtime() - t1 > 0.06)) {
      reqBarber = q.front();
      MPI_Send(&reqBarber, 1, MPI_INT, 0, HAIRCUT, MPI_COMM_WORLD);
      MPI_Recv(&resBarber, 1, MPI_INT, 0, HAIRCUT, MPI_COMM_WORLD, &status);
      t1 = MPI_Wtime();
      q.pop();
    }
  }
}

void barber(int client_count) {
  int ProcNum;
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
  if (ProcNum == 1) return;

  int req;
  int res = 1;
  MPI_Status status;

  for (int i = 0; i < client_count; i++) {
    MPI_Recv(&req, 1, MPI_INT, 1, HAIRCUT, MPI_COMM_WORLD, &status);
    MPI_Ssend(&res, 1, MPI_INT, 1, HAIRCUT, MPI_COMM_WORLD);

    std::this_thread::sleep_for(std::chrono::milliseconds(60));
    // std::cout << "I'm barber and i finish with client " << req << std::endl;
  }
  // std::cout << "I'm barber and my work ended" << std::endl;
}
