// Copyright 2022 Selivankin Sergey
#include <gtest/gtest.h>
#include <vector>
#include "./median_filter.h"
#include <gtest-mpi-listener.hpp>

TEST(Parallel_Operations_MPI, Test_1) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int m = size;
    int n = size + 5;

    std::vector<int> global_mat(m*n);
    std::vector<int> parallel_result(m*n);

    if (rank == 0) {
        global_mat = getRandomMatrix(m, n);
    }

    parallel_result = getMedianFilterParallel(global_mat, m, n);

    if (rank == 0) {
        std::vector<int> sequence_result(m*n);
        sequence_result = getMedianFilterSequence(global_mat, m, n);
        ASSERT_EQ(sequence_result, parallel_result);
    }
}

TEST(Parallel_Operations_MPI, Test_2) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int m = size + 3;

    std::vector<int> global_mat(m * m);
    std::vector<int> parallel_result(m * m);

    if (rank == 0) {
        global_mat = getRandomMatrix(m, m);
    }

    parallel_result = getMedianFilterParallel(global_mat, m, m);

    if (rank == 0) {
        std::vector<int> sequence_result(m * m);
        sequence_result = getMedianFilterSequence(global_mat, m, m);
        ASSERT_EQ(sequence_result, parallel_result);
    }
}

TEST(Parallel_Operations_MPI, Test_3) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int m = size;

    std::vector<int> global_mat(m * m);
    std::vector<int> parallel_result(m * m);

    if (rank == 0) {
        global_mat = getRandomMatrix(m, m);
    }

    parallel_result = getMedianFilterParallel(global_mat, m, m);

    if (rank == 0) {
        std::vector<int> sequence_result(m * m);
        sequence_result = getMedianFilterSequence(global_mat, m, m);
        ASSERT_EQ(sequence_result, parallel_result);
    }
}

TEST(Parallel_Operations_MPI, Test_4) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int m = size + 7;
    int n = size;

    std::vector<int> global_mat(m * n);
    std::vector<int> parallel_result(m * n);

    if (rank == 0) {
        global_mat = getRandomMatrix(m, n);
    }

    parallel_result = getMedianFilterParallel(global_mat, m, n);

    if (rank == 0) {
        std::vector<int> sequence_result(m * n);
        sequence_result = getMedianFilterSequence(global_mat, m, n);
        ASSERT_EQ(sequence_result, parallel_result);
    }
}

TEST(Parallel_Operations_MPI, Test_5) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int m = size + 2;
    int n = size + 11;

    std::vector<int> global_mat(m * n);
    std::vector<int> parallel_result(m * n);

    if (rank == 0) {
        global_mat = getRandomMatrix(m, n);
    }

    parallel_result = getMedianFilterParallel(global_mat, m, n);

    if (rank == 0) {
        std::vector<int> sequence_result(m * n);
        sequence_result = getMedianFilterSequence(global_mat, m, n);
        ASSERT_EQ(sequence_result, parallel_result);
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
