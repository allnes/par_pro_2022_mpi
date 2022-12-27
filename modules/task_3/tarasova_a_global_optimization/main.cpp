// Copyright 2022 Tarasova Anastasia
#include <gtest/gtest.h>
#include "./global_optimization.h"
#include <gtest-mpi-listener.hpp>

double f1(double* x) { return *x; }

double f2(double* x) { return *x * *x; }

double f3(double* x) { return exp(*x); }

double f4(double* x) { return cos(*x); }

double f5(double* x) { return sin(*x); }

void GlobalOptimizationTest(const double a, const double b, std::function<double(double*)> func,
    const int part, const double E) {
    int ProcId;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcId);
    double GlobalOptPar = GetGlobalOptParallel(a, b, func, part, E);
    if (ProcId == 0) {
        double GlobalOptLin = GetGlobalOpt(a, b, func, part, E);
        ASSERT_NEAR(GlobalOptPar, GlobalOptLin, E);
    }
}

TEST(Global_Optimization, Test1) {
    GlobalOptimizationTest(0.9, 3.2, f1, 10000, 0.01);
}

TEST(Global_Optimization, Test2) {
    GlobalOptimizationTest(1, 400, f2, 1000, 0.005);
}

TEST(GlobalOptimization, Test3) {
    GlobalOptimizationTest(0.2, 6, f3, 1000, 0.0001);
}

TEST(GlobalOptimization, Test4) {
    GlobalOptimizationTest(0.09, 0.12, f4, 10000, 0.01);
}

TEST(GlobalOptimization, Test5) {
    GlobalOptimizationTest(25.8, 28, f5, 1000, 0.01);
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
