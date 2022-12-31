// Copyright 2022 Tarasova Anastasia

#include <gtest/gtest.h>
#include "./simple_iterations.h"
#include <gtest-mpi-listener.hpp>

void SimpleIterationsTest(const int min, const int max, const int size, const double eps) {
    int ProcId;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcId);
    std::vector<std::vector<double>> A;
    std::vector<double> B;

    if (ProcId == 0) {
        A = CreateMatrix(size);
        B = CreateVector(size, min, max);
    }

    std::vector<double> IterPar = GetSimpleIterParallel(A, B, size, eps);
    if (ProcId == 0) {
        std::vector<double> IterLin = GetSimpleIter(A, B, size, eps);
        for (int i = 0; i < size; i++) {
            EXPECT_TRUE(std::abs(IterPar.at(i) - IterLin.at(i)) < eps);
        }
    }
}
TEST(Simple_Iter, Test1) {
    SimpleIterationsTest(-10, 10, 5, 0.0005);
}

TEST(Simple_Iter, Test2) {
    SimpleIterationsTest(-100, 100, 50, 0.00001);
}

TEST(Simple_Iter, Test3) {
    SimpleIterationsTest(-500, 500, 100, 0.000004);
}

TEST(Simple_Iter, Test4) {
    SimpleIterationsTest(-1000, 1000, 5, 0.00005);
}

TEST(Simple_Iter, Test5) {
    SimpleIterationsTest(-105, 105, 100, 0.0005);
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
