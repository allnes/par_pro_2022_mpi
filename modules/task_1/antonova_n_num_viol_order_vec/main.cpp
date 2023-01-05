  //  Copyright 2022 Antonova Nastya
#include <gtest/gtest.h>
#include <vector>
#include <gtest-mpi-listener.hpp>
#include "./num_violation_order_vector.h"

TEST(Num_Violation_Order_Vector, Test_Num) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);  //
  std::vector<int> global_vec;
  const int size_vector = 100;
  if (rank == 0) {
    global_vec = getRandomVector(size_vector);
  }

  int  parellel_num = getNumViolationOrderVectorParallel(global_vec, size_vector);

  if (rank == 0) {
    int seqential_num = getNumViolationOrderVector(global_vec);
    ASSERT_EQ(seqential_num, parellel_num);
  }
}

TEST(Num_Violation_Order_Vector, Test_Ordered_Vector) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> vec(20);
  if (rank == 0) {
    for (size_t i = 0; i < vec.size(); ++i) {
      vec[i] = i;
    }
  }

  int  parellel_num = getNumViolationOrderVectorParallel(vec, vec.size());

  if (rank == 0) {
    ASSERT_EQ(parellel_num, 0);
  }
}

TEST(Num_Violation_Order_Vector, Test_Const_Unordered_Vector) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<int> vec = {9, 5, 2, 57, 2, 47, 1, 6, 3, 5, 2, 67, 2, 4, 73, 2, 5};

  int  parellel_num = getNumViolationOrderVectorParallel(vec, vec.size());

  if (rank == 0) {
    ASSERT_EQ(parellel_num, 8);
  }
}

TEST(Num_Violation_Order_Vector, Test_Vector_Identical_Elements) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<int> vec(20, 5);

  int  parellel_num = getNumViolationOrderVectorParallel(vec, vec.size());

  if (rank == 0) {
    ASSERT_EQ(parellel_num, 0);
  }
}

TEST(Num_Violation_Order_Vector, Test_Sorted_In_Descending_Order_Vector) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const int size_vector = 20;
  std::vector<int> vec(size_vector);

  if (rank == 0) {
    for (size_t i = 0; i < vec.size(); ++i) {
      vec[i] = vec.size() - i - 1;
    }
  }

  int  parellel_num = getNumViolationOrderVectorParallel(vec, vec.size());

  if (rank == 0) {
    ASSERT_EQ(parellel_num, size_vector - 1);
  }
}

TEST(Num_Violation_Order_Vector, Test_Odd_Size_Vector) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> global_vec;
  const int size_vector = 125;
  if (rank == 0) {
    global_vec = getRandomVector(size_vector);
  }

  int  parellel_num = getNumViolationOrderVectorParallel(global_vec, size_vector);

  if (rank == 0) {
    int seqential_num = getNumViolationOrderVector(global_vec);
    ASSERT_EQ(seqential_num, parellel_num);
  }
}

TEST(Num_Violation_Order_Vector, Test_Big_Vector) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> global_vec;
  const int size_vector = 100000;
  if (rank == 0) {
    global_vec = getRandomVector(size_vector);
  }

  int  parellel_num = getNumViolationOrderVectorParallel(global_vec, size_vector);

  if (rank == 0) {
    int seqential_num = getNumViolationOrderVector(global_vec);
    ASSERT_EQ(seqential_num, parellel_num);
  }
}

TEST(Num_Violation_Order_Vector, Test_Zero_Elements_Vector) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> global_vec;

  int  parellel_num = getNumViolationOrderVectorParallel(global_vec, global_vec.size());

  if (rank == 0) {
    ASSERT_EQ(parellel_num, 0);
  }
}

TEST(Num_Violation_Order_Vector, Test_One_Elements_Vector) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> global_vec;
  const int size_vector = 1;
  if (rank == 0) {
    global_vec = getRandomVector(size_vector);
  }

  int  parellel_num = getNumViolationOrderVectorParallel(global_vec, size_vector);

  if (rank == 0) {
    int seqential_num = getNumViolationOrderVector(global_vec);
    ASSERT_EQ(seqential_num, parellel_num);
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
