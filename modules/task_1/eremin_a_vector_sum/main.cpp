// Copyright 2022 Eremin Aleksandr
#include <gtest/gtest.h>
#include "./ops_mpi.h"
#include <gtest-mpi-listener.hpp>

TEST(Parallel_Operations_MPI, Test_Sum_1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> global_vec;
    const int count_size_vector = 14;
    if (rank == 0) {
        global_vec = { 1, 2, 3, 4, 5, 10, 7, 8, 90, 6, 11, 12, 59, 18 };
    }
    int global_sum = sumParallel(global_vec, count_size_vector);
    if (rank == 0) {
        int reference_sum = 236;
        ASSERT_EQ(reference_sum, global_sum);
    }
}

TEST(Parallel_Operations_MPI, Test_Sum_2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec;
    const int count_size_vector = 26;

    if (rank == 0) {
        global_vec = random(count_size_vector);
    }
    int global_sum = sumParallel(global_vec, count_size_vector);
    if (rank == 0) {
        int reference_sum = sum(global_vec);
        ASSERT_EQ(reference_sum, global_sum);
    }
}

TEST(Parallel_Operations_MPI, Test_Sum_3) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec;
    const int count_size_vector = 151;

    if (rank == 0) {
        global_vec = random(count_size_vector);
    }
    int global_sum = sumParallel(global_vec, count_size_vector);
    if (rank == 0) {
        int reference_sum = sum(global_vec);
        ASSERT_EQ(reference_sum, global_sum);
    }
}

TEST(Parallel_Operations_MPI, Test_Sum_4) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec;
    const int count_size_vector = 543;

    if (rank == 0) {
        global_vec = random(count_size_vector);
    }
    int global_sum = sumParallel(global_vec, count_size_vector);
    if (rank == 0) {
        int reference_sum = sum(global_vec);
        ASSERT_EQ(reference_sum, global_sum);
    }
}

TEST(Parallel_Operations_MPI, Test_Sum_5) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec;
    const int count_size_vector = 1001;

    if (rank == 0) {
        global_vec = random(count_size_vector);
    }
    int global_sum = sumParallel(global_vec, count_size_vector);
    if (rank == 0) {
        int reference_sum = sum(global_vec);
        ASSERT_EQ(reference_sum, global_sum);
    }
}

TEST(Parallel_Operations_MPI, Test_Sum_6) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec;
    const int count_size_vector = 1500;

    if (rank == 0) {
        global_vec = random(count_size_vector);
    }
    int global_sum = sumParallel(global_vec, count_size_vector);
    if (rank == 0) {
        int reference_sum = sum(global_vec);
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
