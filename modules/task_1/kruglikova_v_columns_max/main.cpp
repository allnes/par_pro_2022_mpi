// Copyright 2022 Kruglikova Valeriia
#include <gtest/gtest.h>
#include "./max_columns.h"
#include <gtest-mpi-listener.hpp>

TEST(Max_Columns, negativeSize_test) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        ASSERT_ANY_THROW(getMatrix(-15, 14));
    }
}

TEST(Max_Columns, sequental_and_paralles_have_same_answer_size20) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> a;
    std::vector<int> cmp2;
    if ( rank == 0 ) {
        a = getMatrix(20, 20);
        cmp2 = getSequentialMax(a, 20, 20);
    }
    std::vector<int> cmp1 = getParallelMax(a, 20, 20);
    if ( rank == 0 ) {
        ASSERT_EQ(cmp1, cmp2);
    }
}

TEST(Max_Columns, sequental_and_paralles_have_same_answer_size100) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> a;
    std::vector<int> cmp2;
    if ( rank == 0 ) {
        a = getMatrix(100, 100);
    }

    std::vector<int> cmp1 = getParallelMax(a, 100, 100);

    if ( rank == 0 ) {
        cmp2 = getSequentialMax(a, 100, 100);
        ASSERT_EQ(cmp1, cmp2);
    }
}

TEST(Max_Columns, zeroSize_test) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if ( rank == 0 ) {
        ASSERT_ANY_THROW(getMatrix(0, 0));
    }
}



TEST(Max_Columns, double_transpose_equals_first_matrix) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> matrix;
    if ( rank == 0 ) {
        matrix = getMatrix(3, 3);
        ASSERT_EQ(matrix, getTransposeMtx(getTransposeMtx(matrix, 3, 3), 3, 3));
    }
}

TEST(Max_Columns, throws_on_wrong_size) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> a;
    if ( rank == 0 ) {
        a = getMatrix(3, 4);
        ASSERT_ANY_THROW(getParallelMax(a, 5, 6));
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
