// Copyright 2022 Selivankin Sergey
#include <gtest/gtest.h>
#include <vector>
#include "./max_vector_element.h"
#include <gtest-mpi-listener.hpp>

TEST(Parallel_Operations_MPI, Test_1) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec;
    const int count_size_vector = size + 10;

    if (rank == 0) {
        global_vec = getRandomVector(count_size_vector);
    }

    int parallel_max = getMaxVectorElemParallel(global_vec, count_size_vector);

    if (rank == 0) {
        int sequence_max = getMaxVectorElemSequence(global_vec);
        ASSERT_EQ(parallel_max, sequence_max);
    }
}

TEST(Parallel_Operations_MPI, Test_2) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec;
    const int count_size_vector = size;

    if (rank == 0) {
        global_vec = getRandomVector(count_size_vector);
    }

    int parallel_max = getMaxVectorElemParallel(global_vec, count_size_vector);

    if (rank == 0) {
        int sequence_max = getMaxVectorElemSequence(global_vec);
        ASSERT_EQ(parallel_max, sequence_max);
    }
}

TEST(Parallel_Operations_MPI, Test_3) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec;
    const int count_size_vector = size * 2;

    if (rank == 0) {
        global_vec = getRandomVector(count_size_vector);
    }

    int parallel_max = getMaxVectorElemParallel(global_vec, count_size_vector);

    if (rank == 0) {
        int sequence_max = getMaxVectorElemSequence(global_vec);
        ASSERT_EQ(parallel_max, sequence_max);
    }
}

TEST(Parallel_Operations_MPI, Test_4) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec;
    const int count_size_vector = size + 109;

    if (rank == 0) {
        global_vec = getRandomVector(count_size_vector);
    }

    int parallel_max = getMaxVectorElemParallel(global_vec, count_size_vector);

    if (rank == 0) {
        int sequence_max = getMaxVectorElemSequence(global_vec);
        ASSERT_EQ(parallel_max, sequence_max);
    }
}

TEST(Parallel_Operations_MPI, Test_5) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec;
    const int count_size_vector = size + 13;

    if (rank == 0) {
        global_vec = getRandomVector(count_size_vector);
    }

    int parallel_max = getMaxVectorElemParallel(global_vec, count_size_vector);

    if (rank == 0) {
        int sequence_max = getMaxVectorElemSequence(global_vec);
        ASSERT_EQ(parallel_max, sequence_max);
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
