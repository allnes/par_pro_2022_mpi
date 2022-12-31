// Copyright 2022 Yashina Anastasia

#include <gtest/gtest.h>
#include <mpi.h>
#include "../../../modules/task_1/yashina_a_sum_elem_mat/sum_elem_mat.h"
#include <gtest-mpi-listener.hpp>




void testing_lab(int size) {
    int ans, res = 0;
    int rank;
    std::vector<int> matrix(size, 0);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        matrix = getMat(size);
    }
    ans = Sum(size, matrix);
    if (rank == 0) {
      res = SumPart(matrix);
    }
    ASSERT_EQ(res, ans);
}

TEST(Sum_of_matrix_elements_MPI, Test_On_Size_1) {
    int size = 1;
    testing_lab(size);
}

TEST(Sum_of_matrix_elements_MPI, Test_On_Size_2) {
    int size = 2;
    testing_lab(size);
}

TEST(Sum_of_matrix_elements_MPI, Test_On_Size_3) {
    int size = 3;
    testing_lab(size);
}

TEST(Sum_of_matrix_elements_MPI, Test_On_Size_9) {
    int size = 9;
    testing_lab(size);
}

TEST(Sum_of_matrix_elements_MPI, Test_On_Size_91) {
    int size = 91;
    testing_lab(size);
}

TEST(Sum_of_matrix_elements_MPI, Test_On_Size_100) {
    int size = 100;
    testing_lab(size);
}

TEST(Sum_of_matrix_elements_MPI, Test_On_Size_5000) {
    int size = 5000;
    testing_lab(size);
}

TEST(Sum_of_matrix_elements_MPI, Test_On_Size_1000000) {
    int size = 1000000;
    testing_lab(size);
}

TEST(Sum_of_matrix_elements_MPI, Negative_matrix_size) {
    int size = -10;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        ASSERT_ANY_THROW(std::vector<int> matrix(size, 0));
    }
}

TEST(Sum_of_matrix_elements_MPI, Empty_matrix) {
    int size = 0;
    int rank;
    std::vector<int> matrix(size, 0);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        ASSERT_ANY_THROW(Sum(size, matrix));
    }
}

int main(int argc, char **argv) {
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
