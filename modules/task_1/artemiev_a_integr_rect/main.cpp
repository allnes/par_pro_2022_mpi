// Copyright 2022 Artemiev Aleksey
#include <gtest/gtest.h>
#include <cmath>
#include "./integr_rect.h"
#include <gtest-mpi-listener.hpp>

void test(double (*f)(double), double a, double b, int n) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double parrResult = integrateParallel(f, a, b, n);

    if (rank == 0) {
        // std::cout << "\nparrResult = " << parrResult << '\n';
        double seqResult = integrateSequential(f, a, b, n);
        // std::cout << "\nseqResult =  " << seqResult << '\n';
        EXPECT_NEAR(parrResult, seqResult, 0.00001);
    }
}

TEST(RectIntegration, RectIntegration_polynom) {
    auto f = [](double x) { return pow(x, 3) + pow(x, 2) + x; };
    double a = 1, b = 2;
    int n = 500;

    test(f, a, b, n);
}

TEST(RectIntegration, RectIntegration_exp) {
    auto f = [](double x) { return exp(x); };
    double a = 0, b = 10;
    int n = 450;

    test(f, a, b, n);
}

TEST(RectIntegration, RectIntegration_sin) {
    auto f = [](double x) { return sin(x); };
    double a = -10, b = 10;
    int n = 10;

    test(f, a, b, n);
}

TEST(RectIntegration, RectIntegration_cos) {
    auto f = [](double x) { return cos(x); };
    double a = 10, b = 20;
    int n = 100;

    test(f, a, b, n);
}

TEST(RectIntegration, RectIntegration_complex) {
    auto f = [](double x) { return x * sin(2 * x); };
    double a = -3, b = 100;
    int n = 10000;

    test(f, a, b, n);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners &listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}
