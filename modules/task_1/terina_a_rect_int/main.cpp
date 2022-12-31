// Copyright 2022 Terina Alina
#include <gtest/gtest.h>
#include <cmath>
#include "../../../modules/task_1/terina_a_rect_int/rect_int.h"
#include <gtest-mpi-listener.hpp>


TEST(Parallel_Operations_MPI, int_cos_0_to_1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double p_res = paralInt(0, 1, cosinus, 5000);
    if (rank == 0) {
        double ord_res = ordinaryInt(0, 1, cosinus, 5000);
        ASSERT_LT(std::fabs(p_res - ord_res),
            std::numeric_limits<double>::epsilon() * 1000);
    }
}
TEST(Parallel_Operations_MPI, int_xcub_0_to_5) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double p_res = paralInt(0, 5, triplex, 20000);
    if (rank == 0) {
        double ord_res = ordinaryInt(0, 5, triplex, 20000);
        ASSERT_NEAR(p_res, ord_res, 1);
    }
}

TEST(Parallel_Operations_MPI, int_with_huge_border) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double p_res = paralInt(0, 500, sinus, 5000);
    if (rank == 0) {
        double ord_res = ordinaryInt(0, 500, sinus, 5000);
        ASSERT_LT(std::fabs(p_res - ord_res),
            std::numeric_limits<double>::epsilon() * 1000);
    }
}
TEST(Parallel_Operations_MPI, int_with_small_border) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double p_res = paralInt(2, 2.2, sinus, 5000);
    if (rank == 0) {
        double ord_res = ordinaryInt(2, 2.2, sinus, 5000);
        ASSERT_LT(std::fabs(p_res - ord_res),
            std::numeric_limits<double>::epsilon() * 1000);
    }
}
TEST(Parallel_Operations_MPI, int_with_neg_border) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double p_res = paralInt(-2, 11, triplex, 20000);
    if (rank == 0) {
        double ord_res = ordinaryInt(-2, 11, triplex, 20000);
        ASSERT_NEAR(p_res, ord_res, 1);
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
