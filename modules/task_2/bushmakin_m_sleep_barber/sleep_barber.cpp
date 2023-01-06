// Copyright 2022 Bushmakin Maksim
#include "../../../modules/task_2/bushmakin_m_sleep_barber/sleep_barber.h"

#include <mpi.h>

#include <iostream>
#include <vector>

void wait(double wtime) {
  double time1 = MPI_Wtime();
  while (MPI_Wtime() - time1 < wtime) {
  }
}

void queue(int seat_count, int number_of_clients) {
  MPI_Status status;

  std::vector<int> vec;
  int queue_response, customer_id, client_count = 0;
  double start_busy_time = 0.;

  while ((client_count < number_of_clients) || (vec.size() != 0)) {
    wait(0.005);

    if (client_count < number_of_clients) {
      MPI_Recv(&customer_id, 1, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD,
               &status);
      // std::cout << "vec.size() is " << vec.size() << std::endl;

      if (vec.size() != seat_count && vec.size() >= 0) {
        queue_response = 0;
        MPI_Send(&queue_response, 1, MPI_INT, customer_id, 2, MPI_COMM_WORLD);
        vec.insert(vec.begin(), customer_id);
        // std::cout << "vec.front() is " << vec.front() << std::endl;
        client_count++;
        // std::cout << "Customer " << customer_id << " insert to queue." <<
        // std::endl;
      } else {
        queue_response = 1;
        MPI_Send(&queue_response, 1, MPI_INT, customer_id, 2, MPI_COMM_WORLD);
      }
    }

    if (MPI_Wtime() - start_busy_time > busy_time && vec.size() != 0) {
      // std::cout << "vec.back() is " << vec.back() << std::endl;
      MPI_Send(&vec.back(), 1, MPI_INT, 0, 3, MPI_COMM_WORLD);
      start_busy_time = MPI_Wtime();
      vec.pop_back();
    }
  }

  MPI_Barrier(MPI_COMM_WORLD);
}

void customer() {
  int ProcRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  MPI_Status status;

  int queue_response = 1;

  while (queue_response == 1) {
    MPI_Send(&ProcRank, 1, MPI_INT, 1, 1, MPI_COMM_WORLD);
    MPI_Recv(&queue_response, 1, MPI_INT, 1, 2, MPI_COMM_WORLD, &status);

    if (queue_response == 1) {
      // std::cout << "Customer " << ProcRank << " goes on cooldown." <<
      // std::endl;
      wait(client_cooldown);
    }
  }

  MPI_Barrier(MPI_COMM_WORLD);
}

void barber(int number_of_clients) {
  MPI_Status status;
  int customer_id;

  for (int i = 0; i < number_of_clients; i++) {
    MPI_Recv(&customer_id, 1, MPI_INT, 1, 3, MPI_COMM_WORLD, &status);
    wait(busy_time);
    // std::cout << "Barber finished with " << customer_id << " customer." <<
    // std::endl;
  }

  MPI_Barrier(MPI_COMM_WORLD);
}
