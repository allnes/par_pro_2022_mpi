// Copyright 2022 Krainev Alexander
#include <gtest/gtest.h>
#include <vector>
#include "./odd_even.h"
#include <gtest-mpi-listener.hpp>

TEST(ODD_EVEN_SORT, N_64) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int n = 64;
    std::vector<int> vec(n);

    if (rank == 0) {
        vec = getRandomVector(n);
    }

    std::vector<int> resultParallel = oddEvenSortParallel(vec);

    if (rank == 0) {
        std::vector<int> resultSequential = oddEvenSortSequential(vec);
        ASSERT_EQ(resultParallel, resultSequential);
    }
}


TEST(ODD_EVEN_SORT, N_256) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int n = 256;
    std::vector<int> vec(n);

    if (rank == 0) {
        vec = getRandomVector(n);
    }

    std::vector<int> resultParallel = oddEvenSortParallel(vec);

    if (rank == 0) {
        std::vector<int> resultSequential = oddEvenSortSequential(vec);
        ASSERT_EQ(resultParallel, resultSequential);
    }
}


TEST(ODD_EVEN_SORT, N_512) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int n = 512;
    std::vector<int> vec(n);

    if (rank == 0) {
        vec = getRandomVector(n);
    }

    std::vector<int> resultParallel = oddEvenSortParallel(vec);

    if (rank == 0) {
        std::vector<int> resultSequential = oddEvenSortSequential(vec);
        ASSERT_EQ(resultParallel, resultSequential);
    }
}


TEST(ODD_EVEN_SORT, N_1024) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int n = 1024;
    std::vector<int> vec(n);

    if (rank == 0) {
        vec = getRandomVector(n);
    }

    std::vector<int> resultParallel = oddEvenSortParallel(vec);

    if (rank == 0) {
        std::vector<int> resultSequential = oddEvenSortSequential(vec);
        ASSERT_EQ(resultParallel, resultSequential);
    }
}

TEST(ODD_EVEN_SORT, N_2048) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int n = 2048;
    std::vector<int> vec(n);

    if (rank == 0) {
        vec = getRandomVector(n);
    }

    std::vector<int> resultParallel = oddEvenSortParallel(vec);

    if (rank == 0) {
        std::vector<int> resultSequential = oddEvenSortSequential(vec);
        ASSERT_EQ(resultParallel, resultSequential);
    }
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
