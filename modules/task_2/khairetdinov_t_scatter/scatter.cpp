// Copyright 2022 Khairetdinov Timur
#include <mpi.h>
#include <algorithm>
#include <vector>
#include "../../../modules/task_2/khairetdinov_t_scatter/scatter.h"

int Scatter(void* send_data, int send_count, MPI_Datatype send_datatype, void* recv_data,
                      int recv_count, MPI_Datatype recv_datatype, int root, MPI_Comm communicator) {
  if (send_count <= 0 || recv_count <= 0 || root < 0)
    throw - 1;
  int size, rank;
  int elemSizer, elemSizes;
  MPI_Comm_size(communicator, &size);
  MPI_Comm_rank(communicator, &rank);

  MPI_Type_size(send_datatype, &elemSizes);
  MPI_Type_size(recv_datatype, &elemSizer);

  if (rank == root) {
    memcpy(recv_data, reinterpret_cast<char*>(send_data) + root * send_count * elemSizer, send_count * elemSizes);
    for (int i = 0; i < size; i++) {
      if (i == root)
        continue;
      MPI_Send(reinterpret_cast<char*>(send_data) + i * send_count * elemSizes,
        send_count, send_datatype, i, 0, communicator);
    }
  } else {
    MPI_Status status;
    MPI_Recv(recv_data, recv_count, recv_datatype, root, 0, communicator, &status);
  }
  return MPI_SUCCESS;
}

int FasterScatter(void* send_data, int send_count, MPI_Datatype send_datatype, void* recv_data,
  int recv_count, MPI_Datatype recv_datatype, int root, MPI_Comm communicator) {
  if (send_count <= 0 || recv_count <= 0 || root < 0)
    throw - 1;
  int size, rank;
  int elemSizes;
  MPI_Comm_size(communicator, &size);
  MPI_Comm_rank(communicator, &rank);

  MPI_Type_size(send_datatype, &elemSizes);

  int locsize = 1;
  int i = 1;
  int patner;
  if (locsize == size) {
    memcpy(recv_data, reinterpret_cast<char*>(send_data), send_count * elemSizes);
  } else {
    while (i <= size) {
      if (i == 1 && rank == 0) {
        memcpy(recv_data, reinterpret_cast<char*>(send_data), send_count * elemSizes);
      }
      if (rank >= i - 1 && rank < locsize && rank != 0) {
        patner = rank - static_cast<int>(pow(2.0, i - 2));
        MPI_Status status;
        MPI_Recv(recv_data, send_count, send_datatype, patner, 0, communicator, &status);
      }
      if (rank < locsize) {
        patner = rank + static_cast<int>(pow(2.0, i - 1));
        if (patner >= size)
          break;
        MPI_Send(reinterpret_cast<char*>(send_data) + patner * send_count * elemSizes,
          send_count, send_datatype, patner, 0, communicator);
      }
      locsize += static_cast<int>(pow(2.0, i - 1));
      i++;
    }
  }
  return MPI_SUCCESS;
}

int MyTest(std::vector<int> mas, std::function<int(void*, int, MPI_Datatype, void*,
  int, MPI_Datatype, int, MPI_Comm)> func) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int n = mas.size();
  int maxrez = 0;
  std::vector<int> rez(n / size);
  func(mas.data(), n / size, MPI_INT, rez.data(), n / size, MPI_INT, 0, MPI_COMM_WORLD);

  int max = *std::max_element(rez.begin(), rez.end());
  MPI_Reduce(&max, &maxrez, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
  return maxrez;
}
