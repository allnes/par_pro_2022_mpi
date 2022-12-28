// Copyright 2022 Shipilov Artem
#include <gtest/gtest.h>
#include <vector>
#include "./producers_consumers.h"
#include <gtest-mpi-listener.hpp>

void test_producers_consumers(int size1, int size2) {
    int size, rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int size_turn;

    if (rank == 0) {
        size_turn = ParallelOperation((size1 * ((size - 1) / 2) + size2 * (size / 2)));
    }
    Seconds(0.1);
    if (rank != 0) {
        if (!(rank % 2 == 0)) {
            for (int i = 0; i < size2; i++) {
                Producers();
            }
        }
        else {
            for (int i = 0; i < size1; i++) {
                Consumers();
            }
        }
    }
    if (rank == 0) {
        ASSERT_EQ(size_turn, (size2 * (size / 2) - size1 * ((size - 1) / 2)));
    }
    MPI_Barrier(MPI_COMM_WORLD);
}
TEST(Producers_Consumers_MPI, Test_1) {
    test_producers_consumers(1, 3);
}
TEST(Producers_Consumers_MPI, Test_2) {
    test_producers_consumers(3, 3);
}
TEST(Producers_Consumers_MPI, Test_3) {
    test_producers_consumers(6, 7);
}
TEST(Producers_Consumers_MPI, Test_4) {
    test_producers_consumers(7, 9);
}
TEST(Producers_Consumers_MPI, Test_5) {
    test_producers_consumers(12, 12);
}
TEST(Producers_Consumers_MPI, Test_6) {
    test_producers_consumers(23, 45);
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
