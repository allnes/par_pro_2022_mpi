// Copyright 2022 Kochetov Maksim
#include <gtest/gtest.h>
#include <vector>
#include "./vector_min_value.h"
#include <gtest-mpi-listener.hpp>

void runVectorMinValueTest(int vecSize) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> testVector;
    if (rank == 0) {
        testVector = genRandomVector(vecSize);
    }

    // Parallel algorithm result will be correct only in rank 0
    double parallelMin = getVectorMinParralel(testVector, vecSize);

    if (rank == 0) {
        // Calculating sequential algorithm result
        double sequentialMin = getVectorMinSequential(testVector);
        // Checking that both of the results are equal
        ASSERT_EQ(parallelMin, sequentialMin);

        /* if (testVector.size() <= 20) {
            std::cout << "testVector: ";
            for (size_t i = 0; i < testVector.size() - 1; i++) {
                std::cout << testVector[i] << " ";
            }
            std::cout << testVector[testVector.size() - 1] << std::endl;
            std::cout << "parallelMin = " << parallelMin;
        } */
    }
}

TEST(Min_Value_Of_Vector, Test1) {
    runVectorMinValueTest(6);
}

TEST(Min_Value_Of_Vector, Test2) {
    runVectorMinValueTest(20);
}

TEST(Min_Value_Of_Vector, Test3) {
    runVectorMinValueTest(1000);
}

TEST(Min_Value_Of_Vector, Test4) {
    runVectorMinValueTest(1000000);
}

TEST(Min_Value_Of_Vector, Test5) {
    runVectorMinValueTest(1000000);
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
