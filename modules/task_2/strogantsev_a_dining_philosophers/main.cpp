// Copyright 2022 Strogantsev Anton
#include <gtest/gtest.h>
#include "./dining_philosophers.h"
#include <gtest-mpi-listener.hpp>


TEST(DiningPhilosophers, OneProceses) {
    int rank, count;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &count);

    int philosophersCount = 0;

    if (rank == 0) {
        executeWaiterLoop(philosophersCount, 1);
    }
    MPI_Barrier(MPI_COMM_WORLD);
}

TEST(DiningPhilosophers, ThreeProcesesOneLoop) {
    int rank, count;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &count);

    int philosophersCount = count - 1;
    int activePhilosophersCount = 2;

    if (philosophersCount < 2) return;

    if (rank == 0) {
        executeWaiterLoop(activePhilosophersCount, 1);
    } else if (rank < 3) {
        executePhilosopherLoop(rank, 1);
    }
    MPI_Barrier(MPI_COMM_WORLD);
}

TEST(DiningPhilosophers, ThreeProcesesTwoLoops) {
    int rank, count;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &count);

    int philosophersCount = count - 1;
    int activePhilosophersCount = 2;

    if (philosophersCount < 2) return;

    if (rank == 0) {
        executeWaiterLoop(activePhilosophersCount, 2);
    } else if (rank < 3) {
        executePhilosopherLoop(rank, 2);
    }
    MPI_Barrier(MPI_COMM_WORLD);
}

TEST(DiningPhilosophers, TwoNProcessesOneLoop) {
    int rank, count;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &count);

    int philosophersCount = count - 1;
    int activePhilosophersCount = philosophersCount;
    activePhilosophersCount += (philosophersCount % 2 == 1) ? 0 : -1;

    if (philosophersCount < 3) return;

    if (rank == 0) {
        executeWaiterLoop(activePhilosophersCount, 1);
    } else if (rank <= activePhilosophersCount) {
        executePhilosopherLoop(rank, 1);
    }
    MPI_Barrier(MPI_COMM_WORLD);
}

TEST(DiningPhilosophers, TwoNPlusOneProcesesOneLoop) {
    int rank, count;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &count);

    int philosophersCount = count - 1;
    int activePhilosophersCount = philosophersCount;
    activePhilosophersCount += (philosophersCount % 2 == 0) ? 0 : -1;

    if (philosophersCount < 2) return;

    if (rank == 0) {
        executeWaiterLoop(activePhilosophersCount, 1);
    } else if (rank <= activePhilosophersCount) {
        executePhilosopherLoop(rank, 1);
    }
    MPI_Barrier(MPI_COMM_WORLD);
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
