// Copyright 2022 Mitin Roman
#include <gtest/gtest.h>

#include <cmath>
#include "./dinning_philo.h"
#include <gtest-mpi-listener.hpp>


TEST(dinning_philo, test_1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if ( rank == 0 ) {
        int64_t* data_ptr = new int64_t;
        *data_ptr = 0;
        dinning(data_ptr);

        EXPECT_EQ(0, *data_ptr);
        delete data_ptr;
    } else {
        dinning(nullptr);
    }
}

TEST(dinning_philo, test_2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if ( rank == 0 ) {
        int64_t* data_ptr = new int64_t;
        *data_ptr = 100;
        dinning(data_ptr);

        EXPECT_EQ(100, *data_ptr);
        delete data_ptr;
    } else {
        dinning(nullptr);
    }
}

TEST(dinning_philo, test_3) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if ( rank == 0 ) {
        int64_t* data_ptr = new int64_t;
        *data_ptr = -100;
        dinning(data_ptr);

        EXPECT_EQ(-100, *data_ptr);
        delete data_ptr;
    } else {
        dinning(nullptr);
    }
}

TEST(dinning_philo, test_4) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if ( rank == 0 ) {
        int64_t* data_ptr = new int64_t;
        *data_ptr = 0xfff;
        dinning(data_ptr);

        EXPECT_EQ(0xfff, *data_ptr);
        delete data_ptr;
    } else {
        dinning(nullptr);
    }
}

TEST(dinning_philo, test_5) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if ( rank == 0 ) {
        int64_t* data_ptr = new int64_t;
        *data_ptr = 1e+5;
        dinning(data_ptr);

        EXPECT_EQ(1e+5, *data_ptr);
        delete data_ptr;
    } else {
        dinning(nullptr);
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
