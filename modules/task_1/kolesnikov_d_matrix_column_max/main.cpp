// Copyright 2022 Kolesnikov Denis
#include <gtest/gtest.h>

#include <gtest-mpi-listener.hpp>

#include "./matrix_column_max.h"





TEST(MAX_BY_COLUMN_TEST, find_max_in_zero_matrix) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int size_x = 0;
    int size_y = 0;
    vector<int> matrix;
    vector<int> max = MaxByColumnSeq(matrix, size_x, size_y);
    if (rank == 0) {
        ASSERT_EQ(vector<int>(), max);
    }
}



TEST(MAX_BY_COLUMN_TEST, find_max_in_1x1_matrix) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int size_x = 1;
    int size_y = 1;
    vector<int> matrix;
    matrix.push_back(5);
    vector<int> max = MaxByColumnSeq(matrix, size_x, size_y);
    if (rank == 0) {
        ASSERT_EQ(5, max[0]);
    }
}


TEST(MAX_BY_COLUMN_TEST, find_max_in_large_matrix) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int size_x = 40;
    int size_y = 40;
    vector<int> matrix;
    if (rank == 0) {
        matrix = GenRndMtrx(size_x, size_y);
    }
    vector<int> max = MaxByColumnPrl(matrix, size_x, size_y);
    if (rank == 0) {
        vector<int> seq_max = MaxByColumnSeq(matrix, size_x, size_y);
        ASSERT_EQ(seq_max, max);
    }
}



TEST(MAX_BY_COLUMN_TEST, find_max_in_non_square_matrix) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int size_x = 13;
    int size_y = 40;
    vector<int> matrix;
    if (rank == 0) {
        matrix = GenRndMtrx(size_x, size_y);
    }
    vector<int> max = MaxByColumnPrl(matrix, size_x, size_y);
    if (rank == 0) {
        vector<int> seq_max = MaxByColumnSeq(matrix, size_x, size_y);
        ASSERT_EQ(seq_max, max);
    }
}


TEST(MAX_BY_COLUMN_TEST, find_max_in_concrete_matrix) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int size_x = 3;
    int size_y = 3;
    vector<int> matrix;
    matrix = vector<int> {
        0, 1, 2,
        1, 2, 3,
        2, 3, 4
    };
    vector<int> max = MaxByColumnPrl(matrix, size_x, size_y);
    if (rank == 0) {
        vector<int> concrete_max{2, 3, 4};
        vector<int> seq_max = MaxByColumnSeq(matrix, size_x, size_y);
        ASSERT_EQ(seq_max, max);
        ASSERT_EQ(concrete_max, max);
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
