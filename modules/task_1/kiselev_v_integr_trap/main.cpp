// Copyright 2023 Kiselev Vladislav
#include <gtest/gtest.h>
#include <cmath>
#include <functional>
#include "./integr_trap.h"
#include <gtest-mpi-listener.hpp>

void TrapezoidalIntegrTest(double a, double b, int segQ,
                           std::function<double(double)> fun) {
    int rg;
    MPI_Comm_rank(MPI_COMM_WORLD, &rg);
    bool isMainProcess = (rg == 0);

    double integralPar = integrateParallel(a, b, segQ, fun);

    if (isMainProcess) {
        double integralSeq = integrateSequential(a, b, segQ, fun);
        // std::cout << "* integralSeq =  " << integralSeq << std::endl;
        // std::cout << "* integralPar = " << integralPar << std::endl;
        ASSERT_NEAR(integralPar, integralSeq, 0.00001);
    }
}

TEST(TrapezoidalIntegr, TrapezoidalIntegr_TestExp_fraq_x_5) {
    double a = 5, b = 25;
    int segQ = 500;
    // e^(2x)
    std::function<double(double)> fun = [](double x) { return exp(x / 5); };

    TrapezoidalIntegrTest(a, b, segQ, fun);
}

TEST(TrapezoidalIntegr, TrapezoidalIntegr_TestPolynomial) {
    double a = 1;
    double b = 4;
    int segQ = 1000;
    // x^2 + x + 2
    std::function<double(double)> fun = [](double x) {
        return pow(x, 2) + pow(x, 1) + 2;
    };

    TrapezoidalIntegrTest(a, b, segQ, fun);
}

TEST(TrapezoidalIntegr, TrapezoidalIntegr_TestSin3x) {
    double a = -5, b = 3;
    int segQ = 300;
    // sin(3x)
    std::function<double(double)> fun = [](double x) { return sin(3 * x); };

    TrapezoidalIntegrTest(a, b, segQ, fun);
}

TEST(TrapezoidalIntegr, TrapezoidalIntegr_TestCosxHalf) {
    double a = 10, b = 20;
    int segQ = 250;
    // cos(x / 2)
    std::function<double(double)> fun = [](double x) { return cos(x / 2); };

    TrapezoidalIntegrTest(a, b, segQ, fun);
}

TEST(TrapezoidalIntegr, TrapezoidalIntegr_TestComplexFun) {
    double a = -3, b = 34;
    int segQ = 10000;
    // 2*x*sin(x)
    std::function<double(double)> fun = [](double x) { return 2 * x * sin(x); };

    TrapezoidalIntegrTest(a, b, segQ, fun);
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
