  // Copyright 2022 Ermolaev Danil
#include <gtest/gtest.h>
#include <vector>
#include "./val_rows_matrix_sum.h"
#include <gtest-mpi-listener.hpp>

TEST(Parallel_Operations_MPI, Test_1) {
    int x = 50;
    int y = 50;
    int* mymatrix = nullptr;
    int* result1 = new int[y];
    for (int j = 0; j < y; j++) {
        result1[j] = 0;
    }
    mymatrix = getRandomMatrix(x, y);
    getSequentialOperation(mymatrix, result1, x, y);
    for (int i = 0; i < y; ++i) {
        int RES = 0;
        RES = std::accumulate(mymatrix + x * i, mymatrix + x * (i + 1), RES);
        ASSERT_EQ(result1[i], RES);
    }

    delete[] result1;
    delete[] mymatrix;
}
TEST(Parallel_Operations_MPI, Test_2) {
    int x = 500;
    int y = 2;
    int* mymatrix = nullptr;
    int* result1 = new int[y];
    for (int j = 0; j < y; j++) {
        result1[j] = 0;
    }
    mymatrix = getRandomMatrix(x, y);
    getSequentialOperation(mymatrix, result1, x, y);
    for (int i = 0; i < y; ++i) {
        int RES = 0;
        RES = std::accumulate(mymatrix + x * i, mymatrix + x * (i + 1), RES);
        ASSERT_EQ(result1[i], RES);
    }

    delete[] result1;
    delete[] mymatrix;
}
TEST(Parallel_Operations_MPI, Test_3) {
    int x = 2;
    int y = 500;
    int* mymatrix = nullptr;
    int* result1 = new int[y];
    for (int j = 0; j < y; j++) {
        result1[j] = 0;
    }
    mymatrix = getRandomMatrix(x, y);
    getSequentialOperation(mymatrix, result1, x, y);
    for (int i = 0; i < y; ++i) {
        int RES = 0;
        RES = std::accumulate(mymatrix + x * i, mymatrix + x * (i + 1), RES);
        ASSERT_EQ(result1[i], RES);
    }

    delete[] result1;
    delete[] mymatrix;
}
TEST(Parallel_Operations_MPI, Test_4) {
    int rank;
    int x = 500;
    int y = 400;
    int* mymatrix = nullptr;
    int* result1 = new int[y];
    for (int j = 0; j < y; j++) {
        result1[j] = 0;
    }
    int* result2 = new int[y];
    for (int j = 0; j < y; j++) {
        result2[j] = 0;
    }

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        mymatrix = getRandomMatrix(x, y);
        getSequentialOperation(mymatrix, result2, x, y);
    }

    getParallelOperation(mymatrix, result1, x, y);

    if (rank == 0) {
        for (int i = 0; i < y; ++i) {
            int RES = 0;
            RES = std::accumulate(mymatrix + x * i, mymatrix + x * (i + 1), RES);
            ASSERT_EQ(result1[i], result2[i]);
        }
    }

    delete[] result1;
    delete[] result2;
    delete[] mymatrix;
}
TEST(Parallel_Operations_MPI, Test_5) {
    int rank;
    int x = 100;
    int y = 1;
    int* mymatrix = nullptr;
    int* result1 = new int[y];
    for (int j = 0; j < y; j++) {
        result1[j] = 0;
    }
    int* result2 = new int[y];
    for (int j = 0; j < y; j++) {
        result2[j] = 0;
    }

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        mymatrix = getRandomMatrix(x, y);
        getSequentialOperation(mymatrix, result2, x, y);
    }

    getParallelOperation(mymatrix, result1, x, y);

    if (rank == 0) {
        for (int i = 0; i < y; ++i) {
            int RES = 0;
            RES = std::accumulate(mymatrix + x * i, mymatrix + x * (i + 1), RES);
            ASSERT_EQ(result1[i], result2[i]);
        }
    }

    delete[] result1;
    delete[] result2;
    delete[] mymatrix;
}
TEST(Parallel_Operations_MPI, Test_6) {
    int rank;
    int x = 1;
    int y = 100;
    int* mymatrix = nullptr;
    int* result1 = new int[y];
    for (int j = 0; j < y; j++) {
        result1[j] = 0;
    }
    int* result2 = new int[y];
    for (int j = 0; j < y; j++) {
        result2[j] = 0;
    }

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        mymatrix = getRandomMatrix(x, y);
        getSequentialOperation(mymatrix, result2, x, y);
    }

    getParallelOperation(mymatrix, result1, x, y);

    if (rank == 0) {
        for (int i = 0; i < y; ++i) {
            int RES = 0;
            RES = std::accumulate(mymatrix + x * i, mymatrix + x * (i + 1), RES);
            ASSERT_EQ(result1[i], result2[i]);
        }
    }

    delete[] result1;
    delete[] result2;
    delete[] mymatrix;
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
