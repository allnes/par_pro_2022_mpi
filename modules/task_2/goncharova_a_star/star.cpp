// Copyright Anna Goncharova

#include "../../../modules/task_2/goncharova_a_star/star.h"

// Create new ///STAR/// topology
MPI_Comm createStarComm(const MPI_Comm oldComm) {
    // Create & get nnodes
    int nnodes;
    MPI_Comm_size(oldComm, &nnodes);

    // Create & add index
    int *index = new int[nnodes];
    index[0] = nnodes - 1;
    for (int i = 1; i < nnodes; i++) {
        index[i] = index[0] + i;
    }

    // Create & add edges
    int edgesSize = 2 * (nnodes - 1);
    int *edges = new int[edgesSize];
    int i = 0;
    for (; i < nnodes - 1; i++) {
        edges[i] = i + 1;
    }
    for (; i < edgesSize; i++) {
        edges[i] = 0;
    }

    // Create reorder
    int reorder = 0;

    // Create ///STAR///
    MPI_Comm starComm;
    MPI_Graph_create(oldComm, nnodes, index, edges, reorder, &starComm);

    return starComm;
}

bool isStarTopology(const MPI_Comm curComm) {
    // Is MPI_GRAPH???
    int status;
    MPI_Topo_test(curComm, &status);
    if (status != MPI_GRAPH) {
        return false;
    }

    // Check edgesSize info
    int nnodes;
    int nedges;
    MPI_Graphdims_get(curComm, &nnodes, &nedges);

    int edgesSize = 2 * (nnodes - 1);
    if (nedges != edgesSize) {
        return false;
    }

    // Check graph info
    int *index = new int[nnodes];
    int *edges = new int[edgesSize];
    MPI_Graph_get(curComm, nnodes, edgesSize, index, edges);
    for (int i = 0; i < nnodes; i++) {
        if (index[i] != i + nnodes - 1) {
            return false;
        }
    }
    int i = 0;
    for (; i < nnodes - 1; i++) {
        if (edges[i] != i + 1) {
            return false;
        }
    }
    for (; i < edgesSize; i++) {
        if (edges[i] != 0) {
            return false;
        }
    }

    return true;
}
