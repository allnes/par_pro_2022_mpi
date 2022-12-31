// Copyright 2022 Andrey Chubenko
#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include "./chubenko_a_vector_disorder.h"
#include <gtest-mpi-listener.hpp>

TEST(Parallel_NDisorder_MPI, Test_random) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec;
    const int count_size_vector = 120;

    if (rank == 0) {
        global_vec = getRandomVector(count_size_vector);
    }

    int global_sum = getParallelNDisorder(global_vec, count_size_vector);

    if (rank == 0) {
        int reference_sum = getSequentialNDisorder(global_vec);
        ASSERT_EQ(reference_sum, global_sum);
    }
}

TEST(Parallel_NDisorder_MPI, Test_ordered) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec;
    const int count_size_vector = 120;

    if (rank == 0) {
        global_vec = getRandomVector(count_size_vector);
        std::sort(global_vec.begin(), global_vec.end());
    }

    int global_sum = getParallelNDisorder(global_vec, count_size_vector);

    if (rank == 0) {
        int reference_sum = getSequentialNDisorder(global_vec);
        ASSERT_EQ(reference_sum, global_sum);
    }
}


TEST(Parallel_NDisorder_MPI, Test_reverse_ordered) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec;
    const int count_size_vector = 120;

    if (rank == 0) {
        global_vec = getRandomVector(count_size_vector);
        std::sort(global_vec.begin(), global_vec.end());
        std::reverse(global_vec.begin(), global_vec.end());
    }

    int global_sum = getParallelNDisorder(global_vec, count_size_vector);

    if (rank == 0) {
        int reference_sum = getSequentialNDisorder(global_vec);
        ASSERT_EQ(reference_sum, global_sum);
    }
}

TEST(Parallel_NDisorder_MPI, Test_one_element) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec;
    const int count_size_vector = 1;

    if (rank == 0) {
        global_vec = getRandomVector(count_size_vector);
    }

    int global_sum = getParallelNDisorder(global_vec, count_size_vector);

    if (rank == 0) {
        int reference_sum = getSequentialNDisorder(global_vec);
        ASSERT_EQ(reference_sum, global_sum);
    }
}

TEST(Parallel_NDisorder_MPI, Test_right_result) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec;
    const int count_size_vector = 8;
    const int expected_n = 3;

    if (rank == 0) {
        global_vec = { 1, 6, 3, 2, 99, 100, 75, 75 };
    }

    int global_sum = getParallelNDisorder(global_vec, count_size_vector);

    if (rank == 0) {
        int reference_sum = getSequentialNDisorder(global_vec);
        ASSERT_EQ(reference_sum, global_sum);
        ASSERT_EQ(reference_sum, expected_n);
    }
}

TEST(Parallel_NDisorder_MPI, Test_long) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec;
    const int count_size_vector = 100000;

    if (rank == 0) {
        global_vec = getRandomVector(count_size_vector);
    }

    int global_sum = getParallelNDisorder(global_vec, count_size_vector);

    if (rank == 0) {
        int reference_sum = getSequentialNDisorder(global_vec);
        ASSERT_EQ(reference_sum, global_sum);
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
