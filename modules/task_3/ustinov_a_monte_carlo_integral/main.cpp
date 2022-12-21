// Copyright 2022 Ustinov A.
#define _USE_MATH_DEFINES
#include <math.h>
#include <gtest/gtest.h>
#include <vector>
#include "./mc_integral.h"
#include <gtest-mpi-listener.hpp>

using std::array;
using std::function;
using std::exp;
using std::sin;
using std::cos;

template <size_t dim>
void test_monte_carlo(
        const function<double(array<double, dim>)> &f,
        const function<bool(array<double, dim>)> &in_region,
        const array<double[2], dim> &rect,
        int64_t n,
        double expected_answer,
        double epsilon) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double obtained_answer = multiple_integral_monte_carlo_parallel<dim>(
                                f, in_region, rect, n);
    if (rank == 0) {
        ASSERT_NEAR(obtained_answer, expected_answer, epsilon);
    }
}


TEST(Monte_Carlo_Multiple_Integral_MPI, Test_1) {
    // 1 dimension, function - '1', region - '[0; 0.5]'
    // The answer is 0.5
    auto f = [](const array<double, 1> &x) -> double {
        return 1.0;
    };
    auto region = [](const array<double, 1> &x) -> bool {
        return x[0] >= 0.0 && x[0] <= 0.5;
    };
    array<double[2], 1> rect;
    rect[0][0] = 0.0, rect[0][1] = 1.0;

    test_monte_carlo<1>(f, region, rect, 30000000, 0.5, 3e-4);
}

TEST(Monte_Carlo_Multiple_Integral_MPI, Test_2) {
    // 1 dimension, function - '1/(1 + x^2)', region - '[0; 1]'
    // The answer is 'pi/4'
    auto f = [](const array<double, 1> &x) -> double {
        return 1.0/(1.0 + x[0]*x[0]);
    };
    auto region = [](const array<double, 1> &x) -> bool {
        return x[0] >= 0.0 && x[0] <= 1.0;
    };
    array<double[2], 1> rect;
    rect[0][0] = 0.0, rect[0][1] = 1.0;

    test_monte_carlo<1>(f, region, rect, 30000000, M_PI_4, 3e-4);
}

TEST(Monte_Carlo_Multiple_Integral_MPI, Test_3) {
    // 2 dimension, function - '1 - x^2 - y^2', region - 'x^2 + y^2 <= 1'
    // The answer is 'pi/4'
    auto f = [](const array<double, 2> &x) -> double {
        return 1.0 - x[0]*x[0] - x[1]*x[1];
    };
    auto region = [](const array<double, 2> &x) -> bool {
        return x[0]*x[0] + x[1]*x[1] <= 1.0;
    };
    array<double[2], 2> rect;
    rect[0][0] = -1.0, rect[0][1] = 1.0;
    rect[1][0] = -1.0, rect[1][1] = 1.0;

    test_monte_carlo<2>(f, region, rect, 3000000, M_PI_2, 3e-3);
}

TEST(Monte_Carlo_Multiple_Integral_MPI, Test_4) {
    // 2 dimension, function - 'e^(-(x^2 + y^2))',
    // region - '0 <= x <= 1, 0 <= y <= 1'
    // The answer is approximately '0.557746'
    auto f = [](const array<double, 2> &x) -> double {
        return exp(-(x[0]*x[0] + x[1]*x[1]));
    };
    auto region = [](const array<double, 2> &x) -> bool {
        return 0.0 <= x[0] && x[0] <= 1.0 && 0.0 <= x[1] && x[1] <= 1.0;
    };
    array<double[2], 2> rect;
    rect[0][0] = 0.0, rect[0][1] = 1.0;
    rect[1][0] = 0.0, rect[1][1] = 1.0;

    test_monte_carlo<2>(f, region, rect, 30000000, 0.557746, 2e-4);
}

TEST(Monte_Carlo_Multiple_Integral_MPI, Test_5) {
    // 3 dimension, function - 'xyz(1-x-y-z)',
    // region - 'x >= 0, y >= 0, z >= 0, x + y + z <= 1'
    // The answer is approximately '0.557746'
    auto f = [](const array<double, 3> &x) -> double {
        return x[0]*x[1]*x[2]*(1.0 - x[0] - x[1] - x[2]);
    };
    auto region = [](const array<double, 3> &x) -> bool {
        return x[0] >= 0.0 && x[1] >= 0.0 && x[2] >= 0.0 &&
               x[0] + x[1] + x[2] <= 1.0;
    };
    array<double[2], 3> rect;
    rect[0][0] = 0.0, rect[0][1] = 1.0;
    rect[1][0] = 0.0, rect[1][1] = 1.0;
    rect[2][0] = 0.0, rect[2][1] = 1.0;

    test_monte_carlo<3>(f, region, rect, 30000000, 1.0/5040.0, 2e-5);
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
