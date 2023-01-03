// Copyright 2022 Nikiforova Lada
#include <gtest/gtest.h>

#include <vector>
#include <gtest-mpi-listener.hpp>

#include "./unright_vec_element.h"

TEST(Parallel_Operations_MPI, Test_size_120) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> globalVec;
    const int count_size_vector = 120;

    if (rank == 0) {
        globalVec = getRandomVector(count_size_vector);
    }

    int parallelUnrightNom =
        parallelFindUnrightElements(globalVec, count_size_vector);

    if (rank == 0) {
        int UnrightNom = unrightElements(globalVec);
        ASSERT_EQ(parallelUnrightNom, UnrightNom);
    }
}

TEST(Parallel_Operations_MPI, Test_size_1000) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> globalVec;
    const int count_size_vector = 1000;

    if (rank == 0) {
        globalVec = getRandomVector(count_size_vector);
    }

    int parallelUnrightNom =
        parallelFindUnrightElements(globalVec, count_size_vector);

    if (rank == 0) {
        int UnrightNom = unrightElements(globalVec);
        ASSERT_EQ(parallelUnrightNom, UnrightNom);
    }
}

TEST(Parallel_Operations_MPI, Test_size_919) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> globalVec;
    const int count_size_vector = 919;

    if (rank == 0) {
        globalVec = getRandomVector(count_size_vector);
    }

    int parallelUnrightNom =
        parallelFindUnrightElements(globalVec, count_size_vector);

    if (rank == 0) {
        int UnrightNom = unrightElements(globalVec);
        ASSERT_EQ(parallelUnrightNom, UnrightNom);
    }
}

TEST(Parallel_Operations_MPI, Test_size_2023) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> globalVec;
    const int count_size_vector = 2023;

    if (rank == 0) {
        globalVec = getRandomVector(count_size_vector);
    }

    int parallelUnrightNom =
        parallelFindUnrightElements(globalVec, count_size_vector);

    if (rank == 0) {
        int UnrightNom = unrightElements(globalVec);
        ASSERT_EQ(parallelUnrightNom, UnrightNom);
    }
}

TEST(Parallel_Operations_MPI, Test_size_331) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> globalVec;
    const int count_size_vector = 331;

    if (rank == 0) {
        globalVec = getRandomVector(count_size_vector);
    }

    int parallelUnrightNom =
        parallelFindUnrightElements(globalVec, count_size_vector);

    if (rank == 0) {
        int UnrightNom = unrightElements(globalVec);
        ASSERT_EQ(parallelUnrightNom, UnrightNom);
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
