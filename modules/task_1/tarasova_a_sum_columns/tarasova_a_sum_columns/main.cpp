// Copyright 2022 Tarasova Anastasia

#include <gtest/gtest.h>
#include "./sum_columns.h"
#include <gtest-mpi-listener.hpp>

void SumOfColumnsTest(const int rows, const int cols) {
    int ProcId;
    std::vector<int> Matrix;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcId);
    if (ProcId == 0) {
        Matrix = CreateMatrix(rows, cols);
    }
    std::vector<int> SumPar = GetSumColsParallel(Matrix, rows, cols);
    if (ProcId == 0) {
        std::vector<int> SumLin = GetSumCols(Matrix, rows, cols);

        for (int i = 0; i < cols; i++) {
            ASSERT_EQ(SumPar.at(i), SumLin.at(i));
        }
    }
}

TEST(Matrix_Sum_Cols, Test1) {
    SumOfColumnsTest(5, 5);
}

TEST(Matrix_Sum_Cols, Test2) {
    SumOfColumnsTest(10, 10);
}

TEST(Matrix_Sum_Cols, Test3) {
    SumOfColumnsTest(100, 100);
}

TEST(Matrix_Sum_Cols, Test4) {
    SumOfColumnsTest(500, 500);
}

TEST(Matrix_Sum_Cols, Test5) {
    SumOfColumnsTest(1000, 1000);
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
