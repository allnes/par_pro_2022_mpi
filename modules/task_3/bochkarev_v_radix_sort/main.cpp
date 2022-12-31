// Copyright 2022 Bochkarev Vladimir
#include <gtest/gtest.h>
#include <vector>
#include "./radix_sort.h"
#include <gtest-mpi-listener.hpp>

TEST(Parallel, Test1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec;
    const int count_size_vector = 120;

    if (rank == 0) {
        global_vec = getRandomVector(count_size_vector);
    }

    std::vector<int> res = getParallelOperations(global_vec, count_size_vector);
    std::vector<int> s;

    if (rank == 0) {
        s = getSequentialOperations(global_vec);
        ASSERT_EQ(s, res);
    }
}

TEST(Parallel, Test2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec;
    const int count_size_vector = 10;

    if (rank == 0) {
        global_vec = getRandomVector(count_size_vector);
    }

    std::vector<int> res = getParallelOperations(global_vec, count_size_vector);
    std::vector<int> s;

    if (rank == 0) {
        s = getSequentialOperations(global_vec);
        ASSERT_EQ(s, res);
    }
}

TEST(Parallel, Test3) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec;
    const int count_size_vector = 1;

    if (rank == 0) {
        global_vec = getRandomVector(count_size_vector);
    }

    std::vector<int> res = getParallelOperations(global_vec, count_size_vector);
    std::vector<int> s;

    if (rank == 0) {
        s = getSequentialOperations(global_vec);
        ASSERT_EQ(s, res);
    }
}

TEST(Parallel, Test4) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec;
    const int count_size_vector = 17;

    if (rank == 0) {
        global_vec = getRandomVector(count_size_vector);
    }

    std::vector<int> res = getParallelOperations(global_vec, count_size_vector);
    std::vector<int> s;

    if (rank == 0) {
        s = getSequentialOperations(global_vec);
        ASSERT_EQ(s, res);
    }
}

TEST(Parallel, Test5) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec;
    const int count_size_vector = 5;

    if (rank == 0) {
        global_vec = getRandomVector(count_size_vector);
    }

    std::vector<int> res = getParallelOperations(global_vec, count_size_vector);
    std::vector<int> s;

    if (rank == 0) {
        s = getSequentialOperations(global_vec);
        ASSERT_EQ(s, res);
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
