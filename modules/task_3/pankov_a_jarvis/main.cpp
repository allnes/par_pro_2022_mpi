// Copyright 2023 Pankov Anatoliy
#include <gtest/gtest.h>
#include <cmath>
#include "./jarvis.h"
#include <gtest-mpi-listener.hpp>

void JarvisTest(int count) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<double> pointsVec;

    if (rank == 0) {
        pointsVec = createRandomPoints(0, 50, count);
    }

    std::vector<double> parrResult = jarvisParallel(pointsVec, count);

    if (rank == 0) {
        std::vector<double> seqResult = jarvisSequential(pointsVec);
        EXPECT_EQ(parrResult, seqResult);
    }
}

TEST(JarvisTest, JarvisTest_1) { JarvisTest(4); }

TEST(JarvisTest, JarvisTest_2) { JarvisTest(10); }

TEST(JarvisTest, JarvisTest_3) { JarvisTest(50); }

TEST(JarvisTest, JarvisTest_4) { JarvisTest(500); }

TEST(JarvisTest, JarvisTest_5) { JarvisTest(900); }

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
