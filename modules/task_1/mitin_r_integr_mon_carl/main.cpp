// Copyright 2022 Mitin Roman
#include <gtest/gtest.h>

#include <cmath>
#include "./integr_mon_carl.h"
#include <gtest-mpi-listener.hpp>

constexpr double max_err = 1e-2;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif  // M_PI

TEST(integrate_monte_carlo, test_const) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    constexpr double val = 1.0;
    constexpr double start = 0.0;
    constexpr double finish = 1.0;
    constexpr double expected = finish - start;


    auto const_func = [=](double){ return val; };

    double res = integrate_monte_carlo(const_func, start, finish, val * 2, 1000000);
    EXPECT_LE(abs(res - expected * val), max_err);
}

TEST(integrate_monte_carlo, test_linear) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    constexpr double val = 2.0;
    constexpr double start = 0.0;
    constexpr double finish = 1.0;
    constexpr double expected = val / 2.0 * (finish * finish - start * start);

    auto linear_func = [=](double num){ return val * num; };

    double res = integrate_monte_carlo(linear_func, start, finish, finish * val, 1000000);
    EXPECT_LE(abs(res - expected), max_err);
}

TEST(integrate_monte_carlo, test_square) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    constexpr double val = 3.0;
    constexpr double start = 0.0;
    constexpr double finish = 1.0;
    constexpr double expected = val / 3.0 * (finish * finish * finish - start * start * start);

    auto square_func = [=](double num) { return val * num * num; };

    double res = integrate_monte_carlo(square_func, start, finish, finish * finish * val, 1000000);

    EXPECT_LE(abs(res - expected), max_err);
}

TEST(integrate_monte_carlo, test_cos) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    constexpr double start = 0.0;
    constexpr double finish = M_PI / 2;
    constexpr double expected = 1;

    auto square_func = [=](double x) { return cos(x); };

    double res = integrate_monte_carlo(square_func, start, finish, 1.0, 1000000);

    EXPECT_LE(abs(res - expected), max_err);
}

TEST(integrate_monte_carlo, test_exp) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    constexpr double start = 0.0;
    constexpr double finish = M_PI;
    constexpr double expected = 1.25120817;

    auto square_func = [=](double x) { return exp(-x * x / 2.0); };

    double res = integrate_monte_carlo(square_func, start, finish, 1.0, 1000000);

    EXPECT_LE(abs(res - expected), max_err);
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
