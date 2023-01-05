// Copyright 2022 Voronov Alexander
#include <gtest/gtest.h>
#include "./star_topology.h"
#include <gtest-mpi-listener.hpp>

TEST(star_topology_MPI, test1_can_create_StarTopology) {
  ASSERT_NO_THROW(createStarComm(MPI_COMM_WORLD));
}

TEST(star_topology_MPI, test2_check_StarComm_is_StarTopology) {
  MPI_Comm StarComm = createStarComm(MPI_COMM_WORLD);
  EXPECT_TRUE(isStarTopology(StarComm));
}

TEST(star_topology_MPI, test3_test_communications_StarTopology) {
  MPI_Comm StarComm = createStarComm(MPI_COMM_WORLD);
  EXPECT_TRUE(testStarTopology(StarComm));
}

TEST(star_topology_MPI, test4_check_MPI_COMM_WORLD_is_StarTopology) {
  EXPECT_FALSE(isStarTopology(MPI_COMM_WORLD));
}

TEST(star_topology_MPI, test5_test_communications_MPI_COMM_WORLD) {
  EXPECT_FALSE(testStarTopology(MPI_COMM_WORLD));
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  MPI_Init(&argc, &argv);

  ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
  ::testing::TestEventListeners& listeners =
    ::testing::UnitTest::GetInstance()->listeners();

  listeners.Release(listeners.default_result_printer());
  listeners.Release(listeners.default_xml_generator());

  listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
  return RUN_ALL_TESTS();
}
