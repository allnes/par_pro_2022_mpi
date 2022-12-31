// Copyright 2018 Nesterov Alexander
#include <gtest/gtest.h>
#include <vector>
#include <iostream>
#include "./vector_sum.h"
#include <gtest-mpi-listener.hpp>

TEST(Parallel_Vector_Sum_MPI, Can_Find_Sum_Of_An_Empty_Vector) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> vec;
    const int vec_size = 0;

    if (rank == 0) {
        vec = getRandVector(vec_size);
    }

    ASSERT_NO_THROW(getParallelVecSum(vec, vec_size));
}

TEST(Parallel_Vector_Sum_MPI, Sum_Of_An_Empty_Vector_Is_Zero) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> vec;
    const int vec_size = 0;

    if (rank == 0) {
        vec = getRandVector(vec_size);
    }

    int global_sum = getParallelVecSum(vec, vec_size);

    if (rank == 0) {
        ASSERT_EQ(0, global_sum);
    }
}

TEST(Parallel_Vector_Sum_MPI, Can_Find_Sum_With_Num_Of_Elems_A_Multiple_Of_The_Number_Of_Processes) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> vec;
    const int vec_size = 42 * size;

    if (rank == 0) {
        vec = getRandVector(vec_size);
    }

    ASSERT_NO_THROW(getParallelVecSum(vec, vec_size));
}

TEST(Parallel_Vector_Sum_MPI, Sum_With_Num_Of_Elems_A_Multiple_Of_The_Number_Of_Processes_Is_Correct) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> vec;
    const int vec_size = 42 * size;

    if (rank == 0) {
        vec = getRandVector(vec_size);
    }

    int global_sum = getParallelVecSum(vec, vec_size);

    if (rank == 0) {
        int reference_sum = getVecSum(vec);
        ASSERT_EQ(reference_sum, global_sum);
    }
}

TEST(Parallel_Vector_Sum_MPI, Can_Find_Sum_With_Num_Of_Elems_Not_Multiple_Of_The_Number_Of_Processes) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> vec;
    const int vec_size = 42 * size + 1;

    if (rank == 0) {
        vec = getRandVector(vec_size);
    }

    ASSERT_NO_THROW(getParallelVecSum(vec, vec_size));
}

TEST(Parallel_Vector_Sum_MPI, Sum_With_Num_Of_Elems_Not_Multiple_Of_The_Number_Of_Processes_Is_Correct) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> vec;
    const int vec_size = 42 * size + 1;

    if (rank == 0) {
        vec = getRandVector(vec_size);
    }

    int global_sum = getParallelVecSum(vec, vec_size);

    if (rank == 0) {
        int reference_sum = getVecSum(vec);
        ASSERT_EQ(reference_sum, global_sum);
    }
}

TEST(Parallel_Vector_Sum_MPI, Can_Find_Sum_Of_An_Unit_Vector) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> vec;
    const int vec_size = 1;
    const int unit_elem = 42;

    if (rank == 0) {
        vec.push_back(unit_elem);
    }

    ASSERT_NO_THROW(getParallelVecSum(vec, vec_size));
}

TEST(Parallel_Vector_Sum_MPI, Sum_Of_An_Unit_Vector_Is_Correct) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> vec;
    const int vec_size = 1;
    const int unit_elem = 42;

    if (rank == 0) {
        vec.push_back(unit_elem);
    }

    int global_sum = getParallelVecSum(vec, vec_size);

    if (rank == 0) {
        ASSERT_EQ(unit_elem, global_sum);
    }
}

TEST(Parallel_Vector_Sum_MPI, Sum_Of_Vector_Does_Not_Change) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> vec;
    const int vec_size = 1;
    const int unit_elem = 42;

    if (rank == 0) {
        vec.push_back(unit_elem);
    }

    int first_sum = getParallelVecSum(vec, vec_size);
    int second_sum = getParallelVecSum(vec, vec_size);

    if (rank == 0) {
        ASSERT_EQ(first_sum, second_sum);
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
