// Copyright 2022 Nikitin Alexander
#include "../../../modules/task_2/nikitin_a_topology_ring/nikitin_a_topology_ring.h"

#include <mpi.h>

MPI_Comm ring(const MPI_Comm comm) {
  int size;
  MPI_Comm_size(comm, &size);
  int ndims = 1;
  int dims[1] = {size};
  int periods[1] = {1};
  int reorder = 0;
  MPI_Comm ring;

  MPI_Cart_create(comm, ndims, dims, periods, reorder, &ring);

  return ring;
}
