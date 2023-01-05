// Copyright 2022 Aleksandar Simeunovic
#include<gtest/gtest.h>
#include<mpi.h>
#include"../../../modules/task_2/simeunovic_a_slepping_barber/slepping_barber.h"
#include<gtest-mpi-listener.hpp>
TEST(Slepping_Barber, Test_1_place) {
    int ProcRank;
    int ProcSize;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcSize);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    if (ProcSize != 1) {
    if (ProcRank == 0) {
        ASSERT_NO_THROW(DoBarberLoop(1, ProcSize, ProcRank));
    } else {
    ASSERT_NO_THROW(ClientLoop(ProcRank));}
    }
}
TEST(Slepping_Barber, Test_2_place) {
    int ProcRank;
    int ProcSize;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcSize);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    if (ProcSize != 1) {
    if (ProcRank == 0) {
        ASSERT_NO_THROW(DoBarberLoop(1, ProcSize, ProcRank));
    } else {
    ASSERT_NO_THROW(ClientLoop(ProcRank));}
    }
}
TEST(Slepping_Barber, Test_4_place) {
    int ProcRank;
    int ProcSize;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcSize);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    if (ProcSize != 1) {
    if (ProcRank == 0) {
        ASSERT_NO_THROW(DoBarberLoop(1, ProcSize, ProcRank));
    } else {
    ASSERT_NO_THROW(ClientLoop(ProcRank));}
    }
}
TEST(Slepping_Barber, Test_6_place) {
    int ProcRank;
    int ProcSize;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcSize);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    if (ProcSize != 1) {
    if (ProcRank == 0) {
        ASSERT_NO_THROW(DoBarberLoop(1, ProcSize, ProcRank));
    } else {
    ASSERT_NO_THROW(ClientLoop(ProcRank));}
    }
}
TEST(Slepping_Barber, Test_10_place) {
    int ProcRank;
    int ProcSize;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcSize);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    if (ProcSize != 1) {
    if (ProcRank == 0) {
        ASSERT_NO_THROW(DoBarberLoop(1, ProcSize, ProcRank));
    } else {
    ASSERT_NO_THROW(ClientLoop(ProcRank));}
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
