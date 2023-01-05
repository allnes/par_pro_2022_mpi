// Copyright 2022 Artemiev Aleksey
#include <gtest/gtest.h>
#include <cmath>
#include "./graham_alg.h"
#include <gtest-mpi-listener.hpp>

void test(int pointsCount) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<double> points;

    if (rank == 0) {
        points = getRandomPoints(pointsCount);
        /* std::cout << "Generated points:\n";
        printPoints(points);
        std::cout << '\n'; */
    }

    std::vector<double> parrResult = grahamParallel(points, pointsCount);

    if (rank == 0) {
        std::vector<double> seqResult = grahamSequential(points);

        /* std::cout << "\nSequential result:\n";
        printPoints(seqResult);
        std::cout << "\n\n";

        std::cout << "\nParallel result:\n";
        printPoints(parrResult);
        std::cout << "\n"; */

        EXPECT_EQ(parrResult, seqResult);
    }
}

TEST(GrahamAlg, GrahamAlg_1) { test(4); }

TEST(GrahamAlg, GrahamAlg_2) { test(9); }

TEST(GrahamAlg, GrahamAlg_3) { test(17); }

TEST(GrahamAlg, GrahamAlg_4) { test(99); }

TEST(GrahamAlg, GrahamAlg_5) { test(100); }

TEST(GrahamAlg, GrahamAlg_6) { test(1000); }

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
