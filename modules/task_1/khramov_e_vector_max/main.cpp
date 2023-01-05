// Copyright 2022 Khramov Egor
#include <gtest/gtest.h>
#include <vector>
#include "./vector_max.h"
#include <gtest-mpi-listener.hpp>

TEST(Parallel_Operations_MPI, Test_Vector_Max_10_elements) {
    int rank, vec_size = 10;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> random_vector;

    if (rank == 0) random_vector = getRandomVector(vec_size);

    int max_sequential;
    int max_parallel = getMaxParallel(random_vector, vec_size);

    if (rank == 0) {
        max_sequential = getMax(random_vector);
        ASSERT_EQ(max_sequential, max_parallel);
    }
}

TEST(Parallel_Operations_MPI, Test_Vector_Max_10000_elements) {
    int rank, vec_size = 10000;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> random_vector;

    if (rank == 0) random_vector = getRandomVector(vec_size);

    int max_sequential;
    int max_parallel = getMaxParallel(random_vector, vec_size);

    if (rank == 0) {
        max_sequential = getMax(random_vector);
        ASSERT_EQ(max_sequential, max_parallel);
    }
}

TEST(Parallel_Operations_MPI, Test_Vector_Max_100000_elements) {
    int rank, vec_size = 100000;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> random_vector;

    if (rank == 0) random_vector = getRandomVector(vec_size);

    int max_sequential;
    int max_parallel = getMaxParallel(random_vector, vec_size);

    if (rank == 0) {
        max_sequential = getMax(random_vector);
        ASSERT_EQ(max_sequential, max_parallel);
    }
}

TEST(Parallel_Operations_MPI, Test_Vector_Max_1_million_elements) {
    int rank, vec_size = 1000000;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> random_vector;

    if (rank == 0) random_vector = getRandomVector(vec_size);

    int max_sequential;
    int max_parallel = getMaxParallel(random_vector, vec_size);

    if (rank == 0) {
        max_sequential = getMax(random_vector);
        ASSERT_EQ(max_sequential, max_parallel);
    }
}

TEST(Parallel_Operations_MPI, Test_Vector_Max_3_million_elements) {
    int rank, vec_size = 3000000;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> random_vector;

    if (rank == 0) random_vector = getRandomVector(vec_size);

    int max_sequential;
    int max_parallel = getMaxParallel(random_vector, vec_size);

    if (rank == 0) {
        max_sequential = getMax(random_vector);
        ASSERT_EQ(max_sequential, max_parallel);
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
