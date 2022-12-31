// Copyright 2022 Korobeynikova Alisa
#include "../../../modules/task_2/korobeynikova_a_sleeping_barber/sleeping_barber.h"

#include <mpi.h>

#include <string>
#include <vector>

static void sendClientToBarber(int *message) {  // message = procNum
  MPI_Send(message, 1, MPI_INT, barberProc, 0, MPI_COMM_WORLD);
}
static void sendResponseToClient(int *response, int procNum) {
  MPI_Send(response, 1, MPI_INT, procNum, 0, MPI_COMM_WORLD);
}

void waitingRoom(int waitingRoomSize) {
  int servedCust = 0;
  CustomerQueue queue(waitingRoomSize);

  int procCount;
  MPI_Comm_size(MPI_COMM_WORLD, &procCount);

  bool barberIsFree = false;
  while (true) {
    int procNum;
    MPI_Status status;
    MPI_Recv(&procNum, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);

    if (procNum == barberProc) {
      if (queue.empty() == false) {
        int clientToBarber = queue.front();
        sendClientToBarber(&clientToBarber);
        queue.pop();
      } else if (servedCust == procCount - 2) {
        int message = no_more_clients;
        sendClientToBarber(&message);
        return;
      } else {
        barberIsFree = true;
      }
    } else {
      int response = take_your_sit;
      try {
        queue.push(procNum);
      } catch (const std::exception &) {
        response = no_place;
      }
      ++servedCust;
      sendResponseToClient(&response, procNum);
    }
    if (barberIsFree && queue.empty() == false) {
      int clientToBarber = queue.front();
      sendClientToBarber(&clientToBarber);
      queue.pop();
      barberIsFree = false;
    }
  }
}
void customer() {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  MPI_Send(&rank, 1, MPI_INT, waitingRoomProc, 0, MPI_COMM_WORLD);

  int response;
  MPI_Status status;
  MPI_Recv(&response, 1, MPI_INT, waitingRoomProc, 0, MPI_COMM_WORLD, &status);

  if (response == take_your_sit) {
    int res;
    MPI_Recv(&res, 1, MPI_INT, barberProc, 0, MPI_COMM_WORLD, &status);
  }
}
int barber() {
  int servedCust = 0;
  MPI_Status status;
  int clientNum = 0;
  while (clientNum != no_more_clients) {
    MPI_Send(&clientNum, 1, MPI_INT, waitingRoomProc, 0, MPI_COMM_WORLD);
    MPI_Recv(&clientNum, 1, MPI_INT, waitingRoomProc, 0, MPI_COMM_WORLD,
             &status);
    if (clientNum != no_more_clients) {
      int res = you_got_a_beautiful_haircut;
      MPI_Send(&res, 1, MPI_INT, clientNum, 0, MPI_COMM_WORLD);
      ++servedCust;
      clientNum = barberProc;
    }
  }
  return servedCust;
}
