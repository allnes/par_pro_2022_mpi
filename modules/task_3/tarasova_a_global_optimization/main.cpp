// Copyright Tarasova Anastasia 2022
#include <gtest/gtest.h>
#include <gtest-mpi-listener.hpp>
#include "../../../modules/task_3/tarasova_a_global_optimization/global_optimization.h"

double lin(double* x) { return *x; }
double qw(double* x) { return *x * *x; }
double e(double* x) { return exp(*x); }
double c(double* x) { return cos(*x); }
double s(double* x) { return sin(*x); }

void GlobalOptimizationTest(const double a, const double b, std::function<double(double*)> func,
    const int part, const double eps) {
    int ProcId;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcId);
    double GlobalOptPar = GetGlobalOptParallel(a, b, func, part, eps);
    if (ProcId == 0) {
        double GlobalOptLin = GetGlobalOpt(a, b, func, part, eps);
        ASSERT_NEAR(GlobalOptPar, GlobalOptLin, eps);
    }
}

TEST(Global_Optimization, Test1) {
    GlobalOptimizationTest(0.98, 4, lin, 10000, 0.0001);
}

TEST(Global_Optimization, Test2) {
    GlobalOptimizationTest(1.5, 400, qw, 1000, 0.0001);
}

TEST(Global_Optimization, Test3) {
    GlobalOptimizationTest(0.1, 6, e, 10000, 0.01);
}

TEST(Global_Optimization, Test4) {
    GlobalOptimizationTest(0.1, 0.2, c, 10000, 0.0005);
}

TEST(Global_Optimization, Test5) {
    GlobalOptimizationTest(20.98, 28, s, 100, 0.05);
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
