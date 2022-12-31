// Copyright 2022 Khramov Egor
#include <gtest/gtest.h>
#include <vector>
#include "./contrast.h"
#include <gtest-mpi-listener.hpp>

void testWithSize(int w, int h) {
    int rank;

    std::vector<int> matrix;
    std::vector<int> resultSequential;
    std::vector<int> resultParallel;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        matrix = getRandomMatrix(w, h);
        resultSequential = getContrastedMatrixSequential(matrix);
    }

    resultParallel = getContrastedMatrixParallel(matrix, w * h);

    if (rank == 0) {
        // std::cout << "sequential: "; printVector(resultSequential);
        // std::cout << "parallel: "; printVector(resultParallel);
        ASSERT_EQ(resultSequential, resultParallel);
    }
}

TEST(Test_contrast, Test_100x50) { testWithSize(100, 50); }

TEST(Test_contrast, Test_512x512) { testWithSize(512, 512); }

TEST(Test_contrast, Test_278x451) { testWithSize(278, 451); }

TEST(Test_contrast, Test_567x129) { testWithSize(567, 129); }

TEST(Test_contrast, Test_2560x1920) { testWithSize(2560, 1920); }

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
