// Copyright 2022 Bataev Ivan
#include <gtest/gtest.h>
#include <vector>
#include "./vector_alternations_signs.h"
#include <gtest-mpi-listener.hpp>

TEST(Vector_Alternations_Signs_MPI, Test_Par_Count1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int vSize = 15, left = -10, right = 10;

    std::vector<int> v(vSize);
    if (rank == 0) {
        v = getRandomVector(vSize, left, right);
        std::cout << "Size = " << vSize << ", generation from " << left << " to " << right << "\n";
        printVector(v, "Vector = ");
    }

    int parCount = getNumAlterSignsParallel(v);
    if (rank == 0) {
        int seqCount = getNumAlterSignsSequential(v);
        std::cout << "Alternations: " << parCount << "\n";
        ASSERT_EQ(parCount, seqCount);
    }
}

TEST(Vector_Alternations_Signs_MPI, Test_Par_Count2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int vSize = 20, left = -100, right = 100;

    std::vector<int> v(vSize);
    if (rank == 0) {
        v = getRandomVector(vSize, left, right);
        std::cout << "Size = " << vSize << ", generation from " << left << " to " << right << "\n";
        printVector(v, "Vector = ");
    }

    int parCount = getNumAlterSignsParallel(v);
    if (rank == 0) {
        int seqCount = getNumAlterSignsSequential(v);
        std::cout << "Alternations: " << parCount << "\n";
        ASSERT_EQ(parCount, seqCount);
    }
}

TEST(Vector_Alternations_Signs_MPI, Test_Par_Count3) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int vSize = 23, left = -100, right = 100;

    std::vector<int> v(vSize);
    if (rank == 0) {
        v = getRandomVector(vSize, left, right);
        std::cout << "Size = " << vSize << ", generation from " << left << " to " << right << "\n";
        printVector(v, "Vector = ");
    }

    int parCount = getNumAlterSignsParallel(v);
    if (rank == 0) {
        int seqCount = getNumAlterSignsSequential(v);
        std::cout << "Alternations: " << parCount << "\n";
        ASSERT_EQ(parCount, seqCount);
    }
}

TEST(Vector_Alternations_Signs_MPI, Test_Par_Count4) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int vSize = 1000, left = -1000, right = 1000;

    std::vector<int> v(vSize);
    if (rank == 0) {
        v = getRandomVector(vSize, left, right);
        std::cout << "Size = " << vSize << ", generation from " << left << " to " << right << "\n";
    }

    int parCount = getNumAlterSignsParallel(v);
    if (rank == 0) {
        int seqCount = getNumAlterSignsSequential(v);
        std::cout << "Alternations: " << parCount << "\n";
        ASSERT_EQ(parCount, seqCount);
    }
}

TEST(Vector_Alternations_Signs_MPI, Test_Par_Count5) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int vSize = 8419, left = -1000, right = 1000;

    std::vector<int> v(vSize);
    if (rank == 0) {
        v = getRandomVector(vSize, left, right);
        std::cout << "Size = " << vSize << ", generation from " << left << " to " << right << "\n";
    }

    int parCount = getNumAlterSignsParallel(v);
    if (rank == 0) {
        int seqCount = getNumAlterSignsSequential(v);
        std::cout << "Alternations: " << parCount << "\n";
        ASSERT_EQ(parCount, seqCount);
    }
}

TEST(Vector_Alternations_Signs_MPI, Test_Par_Count6) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int vSize = 134227, left = -10000, right = 10000;

    std::vector<int> v(vSize);
    if (rank == 0) {
        v = getRandomVector(vSize, left, right);
        std::cout << "Size = " << vSize << ", generation from " << left << " to " << right << "\n";
    }

    int parCount = getNumAlterSignsParallel(v);
    if (rank == 0) {
        int seqCount = getNumAlterSignsSequential(v);
        std::cout << "Alternations: " << parCount << "\n";
        ASSERT_EQ(parCount, seqCount);
    }
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
