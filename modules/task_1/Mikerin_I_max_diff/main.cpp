// Copyright 2022 Mikerin Ilya
#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include "./max_diff.h"
#include <gtest-mpi-listener.hpp>

TEST(Parallel_Operations_MPI, CONCRET) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int* global_vec;
    const int count_size_vector = 24;

    if (rank == 0) {
        global_vec = new int[count_size_vector]
        {0 , 100, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23};
    } else {
        global_vec = new int[count_size_vector];
    }

    int global_max_diff = getParallelOperations(global_vec, count_size_vector);

    if (rank == 0) {
        ASSERT_EQ(100, global_max_diff);
    }
}

TEST(Parallel_Operations_MPI, Gran) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int* global_vec;
    const int count_size_vector = 24;

    if (rank == 0) {
        global_vec = new int[count_size_vector]
        {0 , 1, 2, 3, 4, 5, 105, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23};
    } else {
        global_vec = new int[count_size_vector];
    }

    int global_max_diff = getParallelOperations(global_vec, count_size_vector);

    if (rank == 0) {
        ASSERT_EQ(100, global_max_diff);
    }
}

TEST(Parallel_Operations_MPI, Vosrast) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int* global_vec;
    const int count_size_vector = 24;

    if (rank == 0) {
        global_vec = new int[count_size_vector]
        {0 , 1, -2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23};
    } else {
        global_vec = new int[count_size_vector];
    }

    int global_max_diff = getParallelOperations(global_vec, count_size_vector);

    if (rank == 0) {
        ASSERT_EQ(5, global_max_diff);
    }
}

TEST(Parallel_Operations_MPI, RANDOM) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int* global_vec;
    const int count_size_vector = 120;

    if (rank == 0) {
        global_vec = getRandomVector(count_size_vector);
    } else {
        global_vec = new int[count_size_vector];
    }

    int global_max_diff = getParallelOperations(global_vec, count_size_vector);

    if (rank == 0) {
        int reference_max_diff = getSequentialOperations(global_vec, count_size_vector);
        ASSERT_EQ(reference_max_diff, global_max_diff);
    }
}

TEST(Parallel_Operations_MPI, SeampleSize) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int* global_vec;
    const int count_size_vector = 5003;

    if (rank == 0) {
        global_vec = getRandomVector(count_size_vector);
    } else {
        global_vec = new int[count_size_vector];
    }

    int global_max_diff = getParallelOperations(global_vec, count_size_vector);

    if (rank == 0) {
        int reference_max_diff = getSequentialOperations(global_vec, count_size_vector);
        ASSERT_EQ(reference_max_diff, global_max_diff);
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
