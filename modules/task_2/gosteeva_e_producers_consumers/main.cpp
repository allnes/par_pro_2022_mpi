// Copyright 2022 Gosteeva Ekaterina

#include <gtest/gtest.h>
#include "./producers_consumers.h"
#include <gtest-mpi-listener.hpp>

void RunTest(int prod_size, int con_size) {
    int MPI_rank, MPI_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &MPI_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &MPI_size);
    int size = prod_size * ((MPI_size - 1) / 2) + con_size * (MPI_size / 2);
    int numb;
    if (MPI_rank == 0) {
        numb = GetAccess(size);
    }
    if (MPI_rank > 0) {
        if (MPI_rank % 2 == 0) {
            for (int i = 0; i < prod_size; i++) {
                Producer();
            }
        } else {
            for (int i = 0; i < con_size; i++) {
                Consumer();
            }
        }
    } else {
        ASSERT_EQ(numb, 0);
    }
    MPI_Barrier(MPI_COMM_WORLD);
}

TEST(Producers_consumers, Test1) {
    RunTest(1, 2);
}

TEST(Producers_consumers, Test2) {
     RunTest(2, 4);
}

TEST(Producers_consumers, Test3) {
    RunTest(5, 3);
}

TEST(Producers_consumers, Test4) {
    RunTest(6, 10);
}

TEST(Producers_consumers, Test5) {
    RunTest(10, 20);
}



int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}
