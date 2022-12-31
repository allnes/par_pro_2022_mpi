// Copyright 2022 Shipilov Artem
#include <gtest/gtest.h>
#include <vector>
#include <iostream>
#include <gtest-mpi-listener.hpp>
#include "./find_most_diff.h"


TEST(Parallel_Operations_MPI, Test_SequenOperation) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec;
    const int count_size_vector = 120;

    if (rank == 0) {
        global_vec = Random(count_size_vector);
    }

    if (rank == 0) {
        ASSERT_NO_THROW(SequenOperation(global_vec));
    }
}
TEST(Parallel_Operations_MPI, Test_SequenOperation_correct) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        vector<int> global_vec = { 2, 7, 4, 4, 3, 10, 45 };
        auto res = SequenOperation(global_vec);

        EXPECT_EQ(res.first, 10);
        EXPECT_EQ(res.second, 45);
    }
}
TEST(Parallel_Operations_MPI, Test_ParallelOperation) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    std::vector<int> global_vec(6);

    if (ProcRank == 0) {
        global_vec = Random(6);
    }
    ASSERT_NO_THROW(ParallelOperation(global_vec));
}
TEST(Parallel_Operations_MPI, Test_ParallelOperation_and_SequenOperation_compared) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    std::vector<int> global_vec(12);

    if (ProcRank == 0) {
        global_vec = Random(12);
    }

    auto parallel_result = ParallelOperation(global_vec);

    if (ProcRank == 0) {
        auto sequen_result = SequenOperation(global_vec);
        ASSERT_EQ(parallel_result, sequen_result);
    }
}
TEST(Parallel_Operations_MPI, Test_ParallelOperation_and_SequenOperation_big_size_compared) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    std::vector<int> global_vec(11733);

    if (ProcRank == 0) {
        global_vec = Random(11733);
    }

    auto parallel_result = ParallelOperation(global_vec);

    if (ProcRank == 0) {
        auto sequen_result = SequenOperation(global_vec);
        ASSERT_EQ(parallel_result, sequen_result);
    }
}
TEST(Parallel_Operations_MPI, test_ParallelOperation_and_SequenOperation_compared_correct) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    std::vector<int> global_vec = { 2, 10, 7, 2, 9, 12 };

    if (ProcRank == 0) {
        global_vec = { 2, 10, 7, 2, 9, 12 };
    }

    auto parallel_result = ParallelOperation(global_vec);

    if (ProcRank == 0) {
        auto sequen_result = SequenOperation(global_vec);
        ASSERT_EQ(parallel_result, sequen_result);
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
