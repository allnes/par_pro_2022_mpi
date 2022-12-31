// Copyright 2022 Khairetdinov Timur
#include <gtest/gtest.h>
#include <vector>
#include <iostream>
#include "../../../modules/task_1/khairetdinov_t_vector_mid_value/vector_mid_value.h"
#include <gtest-mpi-listener.hpp>

TEST(Mid_Value_Vector_MPI, Test_Random_Vector_Whith_2_Elements) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector <int> global_vector;

  const int vector_size = 2;

  if (rank == 0) {
    global_vector = getRandomVector(vector_size);
  }

  double mid_parallel = midValueOfVectorParallel(global_vector, vector_size);

  if (rank == 0) {
    double mid_sequential = sumOfVectorSequential(global_vector) / vector_size;
    ASSERT_EQ(mid_parallel, mid_sequential);
  }
}

TEST(Mid_Value_Vector_MPI, Test_Random_Vector_With_10_Elements) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector <int> global_vector;
  const int vector_size = 10;

  if (rank == 0) {
    global_vector = getRandomVector(vector_size);
  }
  double mid_parallel = midValueOfVectorParallel(global_vector, vector_size);

  if (rank == 0) {
    double mid_sequential = sumOfVectorSequential(global_vector) / vector_size;
    ASSERT_EQ(mid_parallel, mid_sequential);
  }
}

TEST(Mid_Value_Vector_MPI, Test_Random_Vector_With_30_Elements) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector <int> global_vector;

  const int vector_size = 30;
  if (rank == 0) {
    global_vector = getRandomVector(vector_size);
  }

  double mid_parallel = midValueOfVectorParallel(global_vector, vector_size);

  if (rank == 0) {
    double mid_sequential = sumOfVectorSequential(global_vector) / vector_size;
    ASSERT_EQ(mid_parallel, mid_sequential);
  }
}

TEST(Mid_Value_Vector_MPI, Test_Const_Vector) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector <int> global_vector = {1, 2, 3, 4, 5, 6, 7, 8};

  const int vector_size = 8;
  double mid_parallel = midValueOfVectorParallel(global_vector, vector_size);

  if (rank == 0) {
    double mid_sequential = sumOfVectorSequential(global_vector) / vector_size;
    ASSERT_EQ(mid_parallel, mid_sequential);
  }
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  MPI_Init(&argc, &argv);

  ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
  ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();

  listeners.Release(listeners.default_result_printer());
  listeners.Release(listeners.default_xml_generator());

  listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
  return RUN_ALL_TESTS();
}
