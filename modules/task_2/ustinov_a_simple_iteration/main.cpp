// Copyright 2018 Nesterov Alexander
#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include <random>
#include "./smpl_iter.h"
#include <gtest-mpi-listener.hpp>

using std::cout;
using std::vector;
using std::mt19937_64;
using std::random_device;
using std::uniform_real_distribution;

vector<double> A;
vector<double> b;
vector<double> b_check;
vector<double> x_parallel;
vector<double> x_sequential;

void test_random_linear_system(int n, double epsilon) {
    int rank;
    A = get_random_stable_matrix(n);
    b = get_random_vector(n);
    x_parallel = simple_iteration_method_parallel(
                                    A, b, n, 100, 1e-10);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        x_sequential = simple_iteration_method_sequential(
                                          A, b, n, 100, 1e-10);
        b_check = matrix_vector_product(A, x_parallel, n);

        for (int i = 0; i < n; ++i)
            ASSERT_DOUBLE_EQ(x_parallel[i], x_sequential[i]);
        for (int i = 0; i < n; ++i)
            ASSERT_NEAR(b[i], b_check[i], epsilon);
    }
}

TEST(Simple_Iteration_Method_MPI, Test_1) {
    for (int i = 0; i < 10; ++i)
        test_random_linear_system(5, 1e-9);
}

TEST(Simple_Iteration_Method_MPI, Test_2) {
    for (int i = 0; i < 10; ++i)
        test_random_linear_system(10, 2e-9);
}

TEST(Simple_Iteration_Method_MPI, Test_3) {
    for (int i = 0; i < 10; ++i)
        test_random_linear_system(50, 1e-8);
}

TEST(Simple_Iteration_Method_MPI, Test_4) {
    for (int i = 0; i < 10; ++i)
        test_random_linear_system(100, 2e-8);
}

TEST(Simple_Iteration_Method_MPI, Test_5) {
    for (int i = 0; i < 5; ++i)
        test_random_linear_system(500, 5e-8);
}

TEST(Simple_Iteration_Method_MPI, Test_6) {
    for (int i = 0; i < 5; ++i)
        test_random_linear_system(1000, 1e-7);
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
