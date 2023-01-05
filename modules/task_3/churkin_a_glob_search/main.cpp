// Copyright 2022 Churkin Alexander
#include <gtest/gtest.h>

#include <iostream>
#include <cmath>

#include "./glob_search.h"

#include <gtest-mpi-listener.hpp>

#define MATH_PI 3.141592653589793

void runTest(double (*f)(double), double a, double b, double correctResult) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    // std::cout << "Process " << rank << "started\n";
    double result = globalSearchOneDimParallel(f, a, b, 0.001);
    if (rank == 0) ASSERT_NEAR(correctResult, result, 0.01);
}

TEST(GlobalSearchOneDim, Test_1_X2) {
    auto f = [](double x) { return pow(x, 2); };
    double a = -15;
    double b = 20;
    double correctResult = 0;
    runTest(f, a, b, correctResult);
}

TEST(GlobalSearchOneDim, Test_2_cosXhalf) {
    auto f = [](double x) { return cos(x / 2); };
    double a = -4 * MATH_PI;
    double b = -MATH_PI;
    double correctResult = -1;
    runTest(f, a, b, correctResult);
}

TEST(GlobalSearchOneDim, Test_3_cbrtX) {
    auto f = [](double x) { return cbrt(x); };
    double a = 8;
    double b = 101;
    double correctResult = 2;
    runTest(f, a, b, correctResult);
}

TEST(GlobalSearchOneDim, Test_4_XsinX) {
    auto f = [](double x) { return x * sin(x); };
    double a = 0;
    double b = 5 * MATH_PI;
    double correctResult = -11.041;
    runTest(f, a, b, correctResult);
}

TEST(GlobalSearchOneDim, Test_5_sineX) {
    auto f = [](double x) { return sin(exp(x)); };
    double a = 1;
    double b = 2.2;
    double correctResult = -1;
    runTest(f, a, b, correctResult);
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
