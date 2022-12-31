// Copyright 2022 Voronov Alexander

#include <gtest/gtest.h>
#include "./min_column_matrix.h"
#include <gtest-mpi-listener.hpp>

TEST(Min_Columns_MPI, Incorrect_Arguments_Create_Matrix) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> Matrix;

    if (rank == 0) {
        ASSERT_ANY_THROW(GetRandomMatrix(0, 1));
    }
}

TEST(Min_Columns_MPI, Incorrect_Arguments_Seq_Min) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> Matrix = GetRandomMatrix(2, 2);

    if (rank == 0) {
        ASSERT_ANY_THROW(GetSequentialMinValueColumn(Matrix, 3, 3));
    }
}

TEST(Min_Columns_MPI, Incorrect_Arguments_Parall_Min) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> Matrix;

    if (rank == 0) {
        Matrix = GetRandomMatrix(2, 2);
    }

    ASSERT_ANY_THROW(GetParallelMinValueColumn(Matrix, 1, 1));
}

TEST(Min_Columns_MPI, Correct_Seq_Min) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> Matrix(4);
    std::vector<int> res(2);
    std::vector<int> e(2);

    e[0] = 1;
    e[1] = 2;

    if (rank == 0) {
        for (int i = 0; i < 4; i++)
            Matrix[i] = i + 1;
    }

    if (rank == 0) {
        res = GetSequentialMinValueColumn(Matrix, 2, 2);
        ASSERT_EQ(e, res);
    }
}

TEST(Min_Columns_MPI, Correct_Transpose_Even) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector <int> v;
    std::vector <int> e(4);

    if (rank == 0) {
        for (int i = 0; i < 4; i++)
            v.push_back(i + 1);
    }

    if (rank == 0) {
        v = GetTransposeMatrix(v, 2, 2);
        e[0] = 1;
        e[1] = 3;
        e[2] = 2;
        e[3] = 4;

        ASSERT_EQ(e, v);
    }
}

TEST(Min_Columns_MPI, Correct_Transpose_Uneven) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector <int> v;
    std::vector <int> e(9);

    if (rank == 0) {
        for (int i = 0; i < 9; i++)
            v.push_back(i + 1);
    }

    if (rank == 0) {
        v = GetTransposeMatrix(v, 3, 3);
        e[0] = 1;
        e[1] = 4;
        e[2] = 7;
        e[3] = 2;
        e[4] = 5;
        e[5] = 8;
        e[6] = 3;
        e[7] = 6;
        e[8] = 9;

        ASSERT_EQ(e, v);
    }
}

TEST(Min_Columns_MPI, Test_On_Size_1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> Matrix;

    if (rank == 0) {
        Matrix = GetRandomMatrix(1, 1);
    }

    std::vector<int> result_parall = GetParallelMinValueColumn(Matrix, 1, 1);

    if (rank == 0) {
        std::vector <int> result = GetSequentialMinValueColumn(Matrix, 1, 1);
        ASSERT_EQ(result, result_parall);
    }
}

TEST(Min_Columns_MPI, Test_On_Size_2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> Matrix;

    if (rank == 0) {
        Matrix = GetRandomMatrix(2, 2);
    }

    std::vector<int> result_parall = GetParallelMinValueColumn(Matrix, 2, 2);

    if (rank == 0) {
        std::vector <int> result = GetSequentialMinValueColumn(Matrix, 2, 2);
        ASSERT_EQ(result, result_parall);
    }
}

TEST(Min_Columns_MPI, Test_On_Size_3) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> Matrix;

    if (rank == 0) {
        Matrix = GetRandomMatrix(3, 3);
    }

    std::vector<int> result_parall = GetParallelMinValueColumn(Matrix, 3, 3);

    if (rank == 0) {
        std::vector <int> result = GetSequentialMinValueColumn(Matrix, 3, 3);
        ASSERT_EQ(result, result_parall);
    }
}

TEST(Min_Columns_MPI, Test_On_Size_4) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> Matrix;

    if (rank == 0) {
        Matrix = GetRandomMatrix(4, 4);
    }

    std::vector<int> result_parall = GetParallelMinValueColumn(Matrix, 4, 4);

    if (rank == 0) {
        std::vector <int> result = GetSequentialMinValueColumn(Matrix, 4, 4);
        ASSERT_EQ(result, result_parall);
    }
}

TEST(Min_Columns_MPI, Test_On_Size_51) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> Matrix;

    if (rank == 0) {
        Matrix = GetRandomMatrix(51, 51);
    }

    std::vector<int> result_parall = GetParallelMinValueColumn(Matrix, 51, 51);

    if (rank == 0) {
        std::vector <int> result = GetSequentialMinValueColumn(Matrix, 51, 51);
        ASSERT_EQ(result, result_parall);
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



