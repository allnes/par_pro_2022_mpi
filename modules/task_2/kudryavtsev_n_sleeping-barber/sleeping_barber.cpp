
#include "../../../modules/task_2/kudryavtsev_n_sleeping-barber/sleeping_barber.h"

#include <mpi.h>

#include <chrono> 
#include <iostream>
#include <queue>
#include <thread> 

void client() {
  MPI_Status status;
  int ProcRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  int res = -1;

  while (res == -1) {
    MPI_Send(&ProcRank, 1, MPI_INT, 1, WROOM_REQUEST, MPI_COMM_WORLD);
    MPI_Recv(&res, 1, MPI_INT, 1, WROOM_RESPONSE, MPI_COMM_WORLD, &status);
    if (res == -1) std::this_thread::sleep_for(std::chrono::milliseconds(15));
 
  }
}

void wait(int seat_count, int client_count) {
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

        i++;
      } else {
        res = -1;
        MPI_Send(&res, 1, MPI_INT, req, WROOM_RESPONSE, MPI_COMM_WORLD);
      }
    }

  
    if (q.size() != 0 && (MPI_Wtime() - t1 > 0.06)) {
      reqBarber = q.front();
      MPI_Send(&reqBarber, 1, MPI_INT, 0, HAIRCUT, MPI_COMM_WORLD);
      MPI_Recv(&resBarber, 1, MPI_INT, 0, HAIRCUT, MPI_COMM_WORLD, &status);
      t1 = MPI_Wtime();
      q.pop();
    }
  }
}

void barb(int client_count) {
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
   
  }
 
}
