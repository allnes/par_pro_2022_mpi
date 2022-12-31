// Copyright Goncharova Anna

#include <gtest/gtest.h>
#include "../../../modules/task_2/goncharova_a_star/star.h"
#include <gtest-mpi-listener.hpp>

TEST(star_test, can_create_star) {
    ASSERT_NO_THROW(createStarComm(MPI_COMM_WORLD));
}

TEST(star_test, no_throw_test) {
    ASSERT_NO_THROW(isStarTopology(MPI_COMM_WORLD));
}

TEST(star_test, check_random_topology) {
    int rank, nnodes;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nnodes);

    int* index = new int[nnodes];
    int* edges = new int[2 * nnodes];
    for (int i = 0; i < nnodes; i++) {
        index[i] = 2 + i * 2;
        edges[i * 2] = (nnodes - 1 + i) % nnodes;
        edges[(i * 2) + 1] = (nnodes + 1 + i) % nnodes;
    }
    MPI_Comm comm;
    MPI_Graph_create(MPI_COMM_WORLD, nnodes, index, edges, 0, &comm);
    if (rank == 0 && nnodes != 1) {
        EXPECT_FALSE(isStarTopology(comm));
    }
}

TEST(star_test, check_is_star_topology) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm star = createStarComm(MPI_COMM_WORLD);
    if (rank == 0) {
        EXPECT_TRUE(isStarTopology(star));
    }
}

TEST(star_test, check_is_not_star_topology) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        EXPECT_FALSE(isStarTopology(MPI_COMM_WORLD));
    }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  MPI_Init(&argc, &argv);

  ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
  ::testing::TestEventListeners &listeners =
      ::testing::UnitTest::GetInstance()->listeners();

  listeners.Release(listeners.default_result_printer());
  listeners.Release(listeners.default_xml_generator());

  listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
  return RUN_ALL_TESTS();
}
