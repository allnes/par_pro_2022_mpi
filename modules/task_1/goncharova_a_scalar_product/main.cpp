// Copyright 2019 Antipin Alexander
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include <iostream>
#include "./scalar_product.h"

TEST(scalar_product, can_product_vectors_with_100_range) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> vec1(100);
    std::vector<int> vec2(100);
    if (rank == 0) {
        vec1 = creatRandomVector(100);
        vec2 = creatRandomVector(100);
    }

    int res1 = getParallelScalarProduct(vec1, vec2);
    if (rank == 0) {
        int res2 = getSequentialScalarProduct(vec1, vec2);
        ASSERT_EQ(res1, res2);
    }
}

TEST(scalar_product, can_throw_if_product_vectors_with_different_ranges) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> vec1 = creatRandomVector(100);
    std::vector<int> vec2 = creatRandomVector(120);
    if (rank == 0) {
        ASSERT_ANY_THROW(getParallelScalarProduct(vec1, vec2));
    }
}

TEST(scalar_product, can_product_vectors_with_odd_range) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> vec1(125);
    std::vector<int> vec2(125);
    if (rank == 0) {
        vec1 = creatRandomVector(125);
        vec2 = creatRandomVector(125);
    }

    int res1 = getParallelScalarProduct(vec1, vec2);
    if (rank == 0) {
        int res2 = getSequentialScalarProduct(vec1, vec2);
        ASSERT_EQ(res1, res2);
    }
}

TEST(scalar_product, can_product_vectors_with_big_range) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> vec1(100000);
    std::vector<int> vec2(100000);
    if (rank == 0) {
        vec1 = creatRandomVector(100000);
        vec2 = creatRandomVector(100000);
    }

    int res1 = getParallelScalarProduct(vec1, vec2);
    if (rank == 0) {
        int res2 = getSequentialScalarProduct(vec1, vec2);
        ASSERT_EQ(res1, res2);
    }
}

TEST(scalar_product, can_product_vectors_with_very_big_range) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> vec1(1000000 - 1);
    std::vector<int> vec2(1000000 - 1);
    if (rank == 0) {
        vec1 = creatRandomVector(1000000 - 1);
        vec2 = creatRandomVector(1000000 - 1);
    }
    int res1 = getParallelScalarProduct(vec1, vec2);
    if (rank == 0) {
        int res2 = getSequentialScalarProduct(vec1, vec2);
        ASSERT_EQ(res1, res2);
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
