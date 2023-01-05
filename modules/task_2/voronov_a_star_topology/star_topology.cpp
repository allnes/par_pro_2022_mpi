// Copyright 2022 Voronov Alexander
#include <mpi.h>
#include "../../../modules/task_2/voronov_a_star_topology/star_topology.h"

MPI_Comm createStarComm(const MPI_Comm oldcomm) {
  int nnodes;
  MPI_Comm_size(oldcomm, &nnodes);
  int* index = new int[nnodes];
  for (int i = 0; i < nnodes; i++)
    index[i] = nnodes - 1 + i;
  int* edges = new int[2*(nnodes - 1)];
  for (int i = 0; i < nnodes - 1; i++)
    edges[i] = i + 1;
  for (int i = nnodes - 1; i < 2*(nnodes - 1) ; i++)
    edges[i] = 0;
  MPI_Comm StarComm;
  MPI_Graph_create(oldcomm, nnodes, index, edges, 0, &StarComm);
  return StarComm;
}

bool isStarTopology(const MPI_Comm StarComm) {
  int status;
  MPI_Topo_test(StarComm, &status);
  if (status != MPI_GRAPH)
    return false;
  int nnodes, nedges;
  MPI_Graphdims_get(StarComm, &nnodes, &nedges);
  int* index = new int[nnodes];
  if (nedges != 2*(nnodes - 1))
    return false;
  int* edges = new int[nedges];
  MPI_Graph_get(StarComm, nnodes, nedges, index, edges);
  for (int i = 0; i < nnodes; i++)
    if (index[i] != nnodes - 1 + i)
      return false;
  for (int i = 0; i < nnodes - 1; i++)
    if (edges[i] != i + 1)
      return false;
  for (int i = nnodes - 1; i < nedges; i++)
    if (edges[i] != 0)
      return false;
  return true;
}

bool testStarTopology(const MPI_Comm StarComm) {
  if (!isStarTopology(StarComm))
    return false;
  int rank;
  MPI_Comm_rank(StarComm, &rank);
  int a = 0;
  int res = 0;
  if (rank == 0)
    a = 1;
  MPI_Bcast(&a, 1, MPI_INT, 0, StarComm);
  MPI_Reduce(&a, &res, 1, MPI_INT, MPI_LAND, 0, StarComm);
  if ((rank == 0) && (res != 1))
    return false;
  return true;
}
