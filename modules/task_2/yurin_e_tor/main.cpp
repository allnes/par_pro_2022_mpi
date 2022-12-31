// Copyright 2022 Yurin Evgeny
#include <gtest/gtest.h>

#include <gtest-mpi-listener.hpp>

#include "./tor.h"

TEST(MeshMpi, test1) {
  Mesh mesh = createMesh();

  int ProcRank;
  int Size;

  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  MPI_Comm_size(MPI_COMM_WORLD, &Size);

  int test_data = 0;
  int next = getNext(0, 3);
  ASSERT_EQ(next, 1);
}

TEST(MeshMpi, test2) {
  Mesh mesh = createMesh();
  int ProcRank;
  int Size;

  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  MPI_Comm_size(MPI_COMM_WORLD, &Size);
  int test_data = 0;

  if (mesh.size >= 4) {
    if (mesh.rank == 0) {
      test_data = 2;
    }

    MeshSend(&test_data, 1, MPI_INT, 0, 3, 0, &mesh);

    if (mesh.rank == 1) {
      ASSERT_EQ(test_data, 2);
    }
  }
}

TEST(MeshMpi, test3) {
  Mesh mesh = createMesh();
  int ProcRank;
  int Size;

  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  MPI_Comm_size(MPI_COMM_WORLD, &Size);
  int test_data = 0;

  if (mesh.size >= 2) {
    if (mesh.rank == 0) {
      test_data = 3;
    }

    MeshSend(&test_data, 1, MPI_INT, 0, 1, 0, &mesh);

    if (mesh.rank == 1) {
      ASSERT_EQ(test_data, 3);
    }
  }
}

TEST(MeshMpi, test4) {
  Mesh mesh = createMesh();
  int ProcRank;
  int Size;

  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  MPI_Comm_size(MPI_COMM_WORLD, &Size);
  int test_data = 0;

  if (mesh.size >= 4) {
    if (mesh.rank == 0) {
      test_data = 4;
    }

    MeshSend(&test_data, 1, MPI_INT, 0, 3, 0, &mesh);

    if (mesh.rank == 4) {
      ASSERT_EQ(test_data, 4);
    }
  }
}

TEST(MeshMpi, test5) {
  Mesh mesh = createMesh();
  int ProcRank;
  int Size;

  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  MPI_Comm_size(MPI_COMM_WORLD, &Size);

  int test_data = 0;

  if (mesh.size >= 4) {
    if (mesh.rank == 0) {
      test_data = 1234;
    }

    MeshSend(&test_data, 1, MPI_INT, 0, 2, 0, &mesh);

    if (mesh.rank == 2) {
      ASSERT_EQ(test_data, 1234);
    }
  }
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
