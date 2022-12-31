  // Copyright 2022 Kudryashov Nikita
#include <gtest/gtest.h>
#include <mpi.h>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include "./kudryashov_n_vector_disorder.h"
#include <gtest-mpi-listener.hpp>

TEST(count_order_disruptions_in_vector, test_ordered_vector) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> vec(30);
    const int vec_size = vec.size();

    if (rank == 0) {
        vec = generateRandomVector(vec_size);
        std::sort(vec.begin(), vec.end());
    }

    int  global_count = countOfDisruptionInVectorParallel(vec, vec_size);

    if (rank == 0) {
        int reference_count = countOfDisruptionInVector(vec);
        ASSERT_EQ(global_count, reference_count);
    }
}

TEST(count_order_disruptions_in_vector, test_vector_with_equal_values) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> vec(30, 7);
    const int vec_size = vec.size();

    int  global_count = countOfDisruptionInVectorParallel(vec, vec_size);

    if (rank == 0) {
        int reference_count = countOfDisruptionInVector(vec);
        ASSERT_EQ(global_count, reference_count);
    }
}

TEST(count_order_disruptions_in_vector, test_random_vector) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> vec(30);
    const int vec_size = vec.size();

    if (rank == 0) {
        vec = generateRandomVector(vec_size);
        std::sort(vec.begin(), vec.end());
    }

    int  global_count = countOfDisruptionInVectorParallel(vec, vec_size);

    if (rank == 0) {
        int reference_count = countOfDisruptionInVector(vec);
        ASSERT_EQ(global_count, reference_count);
    }
}

TEST(count_order_disruptions_in_vector, test_unordered_vector) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> vec = {85, 13 , 26, 28, 38, 22, 53, 43, 94, 68, 76, 40, 69, 27, 73};
    const int vec_size = vec.size();

    int  global_count = countOfDisruptionInVectorParallel(vec, vec_size);

    if (rank == 0) {
        int reference_count = countOfDisruptionInVector(vec);
        ASSERT_EQ(global_count, reference_count);
    }
}

TEST(count_order_disruptions_in_vector, test_totally_unordered_vector) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> vec(30);
    const int vec_size = vec.size();

    if (rank == 0) {
        vec = generateRandomVector(vec_size);
        std::sort(vec.begin(), vec.end());
        std::reverse(vec.begin(), vec.end());
    }

    int  global_count = countOfDisruptionInVectorParallel(vec, vec_size);

    if (rank == 0) {
        int reference_count = countOfDisruptionInVector(vec);
        ASSERT_EQ(global_count, reference_count);
    }
}

TEST(count_order_disruptions_in_vector, test_unit_vector) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> vec(1);
    const int vec_size = vec.size();

    if (rank == 0) {
        vec = generateRandomVector(vec_size);;
    }

    int  global_count = countOfDisruptionInVectorParallel(vec, vec_size);

    if (rank == 0) {
        int reference_count = countOfDisruptionInVector(vec);
        ASSERT_EQ(global_count, reference_count);
    }
}

TEST(count_order_disruptions_in_vector, test_big_vector) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> vec(10000);
    const int vec_size = vec.size();

    if (rank == 0) {
        vec = generateRandomVector(vec.size());;
    }

    int  global_count = countOfDisruptionInVectorParallel(vec, vec_size);

    if (rank == 0) {
        int reference_count = countOfDisruptionInVector(vec);
        ASSERT_EQ(global_count, reference_count);
    }
}

TEST(count_order_disruptions_in_vector, test_vector_with_odd_size) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> vec(123);
    const int vec_size = vec.size();

    if (rank == 0) {
        vec = generateRandomVector(vec_size);;
    }

    int  global_count = countOfDisruptionInVectorParallel(vec, vec_size);

    if (rank == 0) {
        int reference_count = countOfDisruptionInVector(vec);
        ASSERT_EQ(global_count, reference_count);
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
