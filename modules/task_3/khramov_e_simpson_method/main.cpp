// Copyright 2022 Khramov Egor
#include <gtest/gtest.h>
#include <cmath>
#include <gtest-mpi-listener.hpp>
#include "./simpson.h"

const double epsilon = 0.1;
const double e = 2.71828182846;

void testIntegral(function func) {
    int rank;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double resultSequential, resultParallel;
    double a[3], b[3], n[3];

    for (int i = 0; i < 3; i++) {
        a[i] = 0;
        b[i] = 1;
        n[i] = 100;
    }

    if (rank == 0) {
        resultSequential = integrateSequential(func, a, b, n);
    }

    resultParallel = integrateParallel(func, a, b, n);

    if (rank == 0) {
        // std::cout << "Parallel: " << resultParallel;
        // std::cout << "Sequential: " << resultSequential;
        ASSERT_NEAR(resultSequential, resultParallel, epsilon);
    }
}

TEST(Test_simpson_method, test_1) {
    testIntegral([](double x, double y, double z) { return 1; });
}

TEST(Test_simpson_method, test_e) {
    testIntegral([](double x, double y, double z) { return e; });
}

TEST(Test_simpson_method, test_xyz) {
    testIntegral([](double x, double y, double z) { return x * y * z; });
}

TEST(Test_simpson_method, test_some) {
    testIntegral([](double x, double y, double z) { return y * x - y - z; });
}

TEST(Test_simpson_method, test_last) {
    testIntegral(
        [](double x, double y, double z) { return 2 * x * y * pow(z, 2); });
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
