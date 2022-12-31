  // Copyright 2022 Gosteeva Ekaterina

#include <gtest/gtest.h>
#include <vector>
#include "./matrix_max.h"
#include <gtest-mpi-listener.hpp>

void MatrixMaxTest(int row_size, int col_size) {
    int rank, parallel, sequential;
    std::vector<int> matrix;
    int matrix_size = row_size * col_size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        matrix = GetRandomMatrix(matrix_size);
    }
    parallel = GetMatrixMaxParralel(matrix, matrix_size);
    if (rank == 0) {
        sequential = GetMatrixMaxSequential(matrix);
        ASSERT_EQ(parallel, sequential);
    }
}

TEST(Matrix_Max, Test1) {
    MatrixMaxTest(5, 5);
}

TEST(Matrix_Max, Test2) {
    MatrixMaxTest(10, 10);
}

TEST(Matrix_Max, Test3) {
    MatrixMaxTest(100, 100);
}

TEST(Matrix_Max, Test4) {
    MatrixMaxTest(500, 500);
}

TEST(Matrix_Max, Test5) {
    MatrixMaxTest(1000, 1000);
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
