// Copyright 2022 Bochkarev Vladimir
#include <gtest/gtest.h>
#include <vector>
#include "./matrix_max_columns.h"
#include <gtest-mpi-listener.hpp>

TEST(Parallel_Operations_MPI, Test_Max_1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> matrix;
    const int count_row = 1, count_col = 1;

    if (rank == 0) {
        matrix = getRandomMatrix(count_row, count_col);
    }

    std::vector<int> p_res = getParallelOperations(matrix, count_row, count_col);
    std::vector<int> s_res(count_col);
    if (rank == 0) {
        for (int i = 0; i < count_col; i++)
            s_res[i] = getSequentialOperations(std::vector<int>(
                matrix.begin()+i*count_row, matrix.begin()+count_row+i*count_row));
        ASSERT_EQ(p_res, s_res);
    }
}

TEST(Parallel_Operations_MPI, Test_Max_2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> matrix;
    const int count_row = 5, count_col = 1;

    if (rank == 0) {
        matrix = getRandomMatrix(count_row, count_col);
    }

    std::vector<int> p_res = getParallelOperations(matrix, count_row, count_col);
    std::vector<int> s_res(count_col);
    if (rank == 0) {
        for (int i = 0; i < count_col; i++)
            s_res[i] = getSequentialOperations(std::vector<int>(
                matrix.begin()+i*count_row, matrix.begin()+count_row+i*count_row));
        ASSERT_EQ(p_res, s_res);
    }
}

TEST(Parallel_Operations_MPI, Test_Max_3) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> matrix;
    const int count_row = 1, count_col = 5;

    if (rank == 0) {
        matrix = getRandomMatrix(count_row, count_col);
    }

    std::vector<int> p_res = getParallelOperations(matrix, count_row, count_col);
    std::vector<int> s_res(count_col);
    if (rank == 0) {
        for (int i = 0; i < count_col; i++)
            s_res[i] = getSequentialOperations(std::vector<int>(
                matrix.begin()+i*count_row, matrix.begin()+count_row+i*count_row));
        ASSERT_EQ(p_res, s_res);
    }
}

TEST(Parallel_Operations_MPI, Test_Max_4) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> matrix;
    const int count_row = 143, count_col = 143;

    if (rank == 0) {
        matrix = getRandomMatrix(count_row, count_col);
    }

    std::vector<int> p_res = getParallelOperations(matrix, count_row, count_col);
    std::vector<int> s_res(count_col);
    if (rank == 0) {
        for (int i = 0; i < count_col; i++)
            s_res[i] = getSequentialOperations(std::vector<int>(
                matrix.begin()+i*count_row, matrix.begin()+count_row+i*count_row));
        ASSERT_EQ(p_res, s_res);
    }
}

TEST(Parallel_Operations_MPI, Test_Max_5) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> matrix;
    const int count_row = 4, count_col = 4;

    if (rank == 0) {
        matrix = getRandomMatrix(count_row, count_col);
    }

    std::vector<int> p_res = getParallelOperations(matrix, count_row, count_col);
    std::vector<int> s_res(count_col);
    if (rank == 0) {
        for (int i = 0; i < count_col; i++)
            s_res[i] = getSequentialOperations(std::vector<int>(
                matrix.begin()+i*count_row, matrix.begin()+count_row+i*count_row));
        ASSERT_EQ(p_res, s_res);
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
