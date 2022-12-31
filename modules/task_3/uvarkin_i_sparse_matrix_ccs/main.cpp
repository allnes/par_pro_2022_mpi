// Copyright 2022 Uvarkin Ilya
#include <gtest/gtest.h>
#include <cmath>
#include "../../../modules/task_3/uvarkin_i_sparse_matrix_ccs/sparse_matrix.h"
#include <gtest-mpi-listener.hpp>

TEST(sparse_matrix_ccs, multiplication) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    std::vector<std::vector<double>> martixA = {{1, 0, 0},
                                                {0, 2, 0},
                                                {3, 0, 1},
                                                {0, 5, 0}};

    std::vector<std::vector<double>> martixB = {{1, 0, 0},
                                                {0, 0, 2},
                                                {0, 9, 0}};

    CCSMatrix A, B;
    if (ProcRank == 0) {
        A = getCCSMatrix(martixA);
        B = getCCSMatrix(martixB);

        std::vector<double> result = Multiply(A, B);
        std::vector<double> expect{ 1, 0, 0, 0, 0, 4, 3, 9, 0, 0, 0, 10};

        ASSERT_EQ(result, expect);
    }
}

TEST(sparse_matrix_ccs, parallelMultiplication_1) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    std::vector<std::vector<double>> martixA = {{1, 0, 0},
                                                {0, 2, 0},
                                                {3, 0, 1},
                                                {0, 5, 0}};

    std::vector<std::vector<double>> martixB = {{1, 0, 0},
                                                {0, 0, 2},
                                                {0, 9, 0}};

    CCSMatrix A, B;
    if (ProcRank == 0) {
        A = getCCSMatrix(martixA);
        B = getCCSMatrix(martixB);
    }

    std::vector<double> result = MultiplyParallel(A, B);

    if (ProcRank == 0) {
        std::vector<double> expect{ 1, 0, 0, 0, 0, 4, 3, 9, 0, 0, 0, 10};

        ASSERT_EQ(result, expect);
    }
}

TEST(sparse_matrix_ccs, parallelMultiplication_2) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    std::vector<std::vector<double>> martixA = {{0, 0, 1},
                                                {1, 0, 0},
                                                {0, 0, 3}};

    std::vector<std::vector<double>> martixB = {{1, 0, 0},
                                                {0, 0, 2},
                                                {0, 9, 0}};

    CCSMatrix A, B;
    if (ProcRank == 0) {
        A = getCCSMatrix(martixA);
        B = getCCSMatrix(martixB);
    }

    std::vector<double> result = MultiplyParallel(A, B);

    if (ProcRank == 0) {
        std::vector<double> expect{ 0, 9, 0, 1, 0, 0, 0, 27, 0};

        ASSERT_EQ(result, expect);
    }
}

TEST(sparse_matrix_ccs, parallelMultiplication_3) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    std::vector<std::vector<double>> martixA = {{3, 0, 1, 0},
                                                {1, 0, 0, 2},
                                                {0, 0, 3, 0},
                                                {5, 0, 0, 0}};

    std::vector<std::vector<double>> martixB = {{1, 0, 0, 0},
                                                {0, 0, 2, 0},
                                                {0, 9, 0, 0},
                                                {0, 0, 7, 0}};

    CCSMatrix A, B;
    if (ProcRank == 0) {
        A = getCCSMatrix(martixA);
        B = getCCSMatrix(martixB);
    }

    std::vector<double> result = MultiplyParallel(A, B);

    if (ProcRank == 0) {
        std::vector<double> expect{ 3, 9, 0, 0, 1, 0, 14, 0, 0, 27, 0, 0, 5, 0, 0, 0};

        ASSERT_EQ(result, expect);
    }
}

TEST(sparse_matrix_ccs, parallelMultiplication_4) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    std::vector<std::vector<double>> martixA = {{0, 8, 0, 0},
                                                {0, 0, 7, 0},
                                                {5, 0, 0, 3},
                                                {0, 2, 0, 0}};

    std::vector<std::vector<double>> martixB = {{0},
                                                {5},
                                                {0},
                                                {2}};

    CCSMatrix A, B;
    if (ProcRank == 0) {
        A = getCCSMatrix(martixA);
        B = getCCSMatrix(martixB);
    }

    std::vector<double> result = MultiplyParallel(A, B);

    if (ProcRank == 0) {
        std::vector<double> expect{ 40, 0, 6, 10};

        ASSERT_EQ(result, expect);
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
