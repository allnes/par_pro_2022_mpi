// Copyright 2023 Kiselev Vladislav
#include <gtest/gtest.h>
#include <vector>
#include <stdexcept>
#include "./matrmult_onlya.h"
#include <gtest-mpi-listener.hpp>

TEST(MatrMultOnlyA, MatrMultOnlyA_T1) {
    int ma = 3, na = 3, mb = 3, nb = 3;

    int Rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &Rank);

    std::vector<int> MatrixA;
    std::vector<int> MatrixB;

    if (Rank == 0) {
        MatrixA = GenRandMatrix(ma, na);
        MatrixB = GenRandMatrix(mb, nb);
    }

    std::vector<int> ParResult = MultParallel(MatrixA, MatrixB, ma, na, mb, nb);

    if (Rank == 0) {
        std::vector<int> SeqResult =
            MultSequential(MatrixA, MatrixB, ma, na, mb, nb);
        ASSERT_EQ(SeqResult, ParResult);
    }
}

TEST(MatrMultOnlyA, MatrMultOnlyA_T2) {
    int ma = 4, na = 3, mb = 3, nb = 5;

    int Rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &Rank);

    std::vector<int> MatrixA;
    std::vector<int> MatrixB;

    if (Rank == 0) {
        MatrixA = GenRandMatrix(ma, na);
        MatrixB = GenRandMatrix(mb, nb);
    }

    std::vector<int> ParResult = MultParallel(MatrixA, MatrixB, ma, na, mb, nb);

    if (Rank == 0) {
        std::vector<int> SeqResult =
            MultSequential(MatrixA, MatrixB, ma, na, mb, nb);
        ASSERT_EQ(SeqResult, ParResult);
    }
}

TEST(MatrMultOnlyA, MatrMultOnlyA_T3) {
    int ma = 5, na = 3, mb = 3, nb = 4;

    int Rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &Rank);

    std::vector<int> MatrixA;
    std::vector<int> MatrixB;

    if (Rank == 0) {
        MatrixA = GenRandMatrix(ma, na);
        MatrixB = GenRandMatrix(mb, nb);
    }

    std::vector<int> ParResult = MultParallel(MatrixA, MatrixB, ma, na, mb, nb);

    if (Rank == 0) {
        std::vector<int> SeqResult =
            MultSequential(MatrixA, MatrixB, ma, na, mb, nb);
        ASSERT_EQ(SeqResult, ParResult);
    }
}

TEST(MatrMultOnlyA, MatrMultOnlyA_T4) {
    int ma = 20, na = 21, mb = 21, nb = 20;

    int Rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &Rank);

    std::vector<int> MatrixA;
    std::vector<int> MatrixB;

    if (Rank == 0) {
        MatrixA = GenRandMatrix(ma, na);
        MatrixB = GenRandMatrix(mb, nb);
    }

    std::vector<int> ParResult = MultParallel(MatrixA, MatrixB, ma, na, mb, nb);

    if (Rank == 0) {
        std::vector<int> SeqResult =
            MultSequential(MatrixA, MatrixB, ma, na, mb, nb);
        ASSERT_EQ(SeqResult, ParResult);
    }
}

TEST(MatrMultOnlyA, MatrMultOnlyA_T5) {
    int ma = 50, na = 50, mb = 50, nb = 50;

    int Rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &Rank);

    std::vector<int> MatrixA;
    std::vector<int> MatrixB;

    if (Rank == 0) {
        MatrixA = GenRandMatrix(ma, na);
        MatrixB = GenRandMatrix(mb, nb);
    }

    std::vector<int> ParResult = MultParallel(MatrixA, MatrixB, ma, na, mb, nb);

    if (Rank == 0) {
        std::vector<int> SeqResult =
            MultSequential(MatrixA, MatrixB, ma, na, mb, nb);
        ASSERT_EQ(SeqResult, ParResult);
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
