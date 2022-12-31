// Copyright 2022 Terina Alina
#include <gtest/gtest.h>
#include <math.h>
#include <vector>
#include "../../../modules/task_3/terina_a_mn_int_trap_method/trap_method.h"
#include <gtest-mpi-listener.hpp>

TEST(trapez_int, test_1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int extent = 2;
    std::vector<double> first_c(extent);
    std::vector<double> second_c(extent);
    first_c = getRandomVector(extent);
    second_c = getRandomVector(extent);
    const int n = 100;
    double sol = ParInt(first_c, second_c, First, n);
    if (rank == 0) {
        double acr = 0.001;
        double  solOrd = OrdinaryInt(first_c, second_c, First, n);
        ASSERT_NEAR(sol, solOrd, acr);
    }
}

TEST(trapez_int, test_2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int extent = 3;
    std::vector<double> first_c(extent);
    std::vector<double> second_c(extent);
    first_c = getRandomVector(extent);
    second_c = getRandomVector(extent);
    int n = 100;
    double sol = ParInt(first_c, second_c, Second, n);
    if (rank == 0) {
        double acr = 0.001;
        double  solOrd = OrdinaryInt(first_c, second_c, Second, n);
        ASSERT_NEAR(sol, solOrd, acr);
    }
}

TEST(trapez_int, test_3) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int extent = 3;
    std::vector<double> first_c(extent);
    std::vector<double> second_c(extent);
    first_c = getRandomVector(extent);
    second_c = getRandomVector(extent);
    int n = 100;
    double sol = ParInt(first_c, second_c, Second, n);
    if (rank == 0) {
        double acr = 0.001;
        double  solOrd = OrdinaryInt(first_c, second_c, Second, n);
        ASSERT_NEAR(sol, solOrd, acr);
    }
}

TEST(trapez_int, test_4) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int extent = 2;
    std::vector<double> first_c(extent);
    std::vector<double> second_c(extent);
    first_c = getRandomVector(extent);
    second_c = getRandomVector(extent);
    int n = 100;
    double sol = ParInt(first_c, second_c, First, n);
    if (rank == 0) {
        double acr = 0.001;
        double  solOrd = OrdinaryInt(first_c, second_c, First, n);
        ASSERT_NEAR(sol, solOrd, acr);
    }
}
TEST(trapez_int, test_5) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int extent = 3;
    std::vector<double> first_c(extent);
    std::vector<double> second_c(extent);
    first_c = getRandomVector(extent);
    second_c = getRandomVector(extent);
    int n = 100;
    double sol = ParInt(first_c, second_c, Second, n);
    if (rank == 0) {
        double acr = 0.001;
        double  solOrd = OrdinaryInt(first_c, second_c, Second, n);
        ASSERT_NEAR(sol, solOrd, acr);
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
