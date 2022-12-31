  // Copyright 2022 Prokofev Denis
#include <gtest/gtest.h>
#include "../../../modules/task_1/prokofev_d_min_val_in_rows/min_val_in_rows.h"
#include <gtest-mpi-listener.hpp>


TEST(minValRows, test1_Gen_Matr) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        ASSERT_ANY_THROW(genMatr(-5, 5));
    }
}

TEST(minValRows, test2_Pos_Val) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec;
    const int count_size_vector = 120;
    std::vector<int> matr = {1, 4, 3,
                    5, 2, 7,
                    9, 4, 2};
    std::vector<int> mymin = {1, 2, 2};
    std::vector<int> res = minValRows(matr, 3, 3);
    if (rank == 0) EXPECT_EQ(res, mymin);
}


TEST(minValRows, test3_Neg_Val) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec;
    const int count_size_vector = 120;
    std::vector<int> matr = {-1, -4, -3,
                    -5, -2, -7,
                    -9, -4, -2};
    std::vector<int> mymin = { -4, -7, -9};
    std::vector<int> res = minValRows(matr, 3, 3);
    if (rank == 0) EXPECT_EQ(res, mymin);
}

TEST(minValRows, test4_Mix_Val) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec;
    const int count_size_vector = 120;
    std::vector<int> matr = { -1, 4, 3,
                    5, 2, -7,
                    9, -4, 2 };
    std::vector<int> mymin = { -1, -7, -4 };
    std::vector<int> res = minValRows(matr, 3, 3);
    if (rank == 0) EXPECT_EQ(res, mymin);
}


TEST(minValRows, test5_Pos_Nonqwad) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec;
    const int count_size_vector = 120;
    std::vector<int> matr = { 1, 4, 3, 2,
                    5, 2, 7, 4,
                    9, 4, 2, 7 };
    std::vector<int> mymin = { 1, 2, 2 };
    std::vector<int> res = minValRows(matr, 3, 4);
    if (rank == 0) EXPECT_EQ(res, mymin);
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

