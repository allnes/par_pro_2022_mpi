  // Copyright 2022 Prokofev Denis
#include <gtest/gtest.h>
#include "../../../modules/task_2/prokofev_d_lent_vert_scheme/lent_vert_scheme.h"
#include <gtest-mpi-listener.hpp>


TEST(minValRows, test1_Gen_Matr) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        ASSERT_ANY_THROW(genMatr(-5, 5));
    }
}

TEST(minValRows, test2_Gen_Vect) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        ASSERT_ANY_THROW(genVect(-5));
    }
}


TEST(minValRows, test3_mat_3x3) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec;
    const int count_size_vector = 120;
    std::vector<int> matr = { 1, 2, 3,
                    4, 5, 6,
                    7, 8, 9 };
    std::vector<int> vect = { 2, 3, 4 };
    std::vector<int> myres = { 20, 47, 74 };
    std::vector<int> res = lentVertScheme(matr, vect, 3, 3);
    if (rank == 0) EXPECT_EQ(res, myres);
}

TEST(minValRows, test4_mat_4x4) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec;
    const int count_size_vector = 120;
    std::vector<int> matr = { 1, 2, 3, 4,
                    5, 6, 7, 8,
                    9, 10, 11, 12,
                    13, 14, 15, 16 };
    std::vector<int> vect = { 2, 3, 4, 5 };
    std::vector<int> myres = { 40, 96, 152, 208 };
    std::vector<int> res = lentVertScheme(matr, vect, 4, 4);
    if (rank == 0) EXPECT_EQ(res, myres);
}

TEST(minValRows, test5_mat_6x6) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec;
    const int count_size_vector = 120;
    std::vector<int> matr = { 1, 2, 3, 4, 5, 6,
                                7, 8, 9, 10, 11, 12,
                                13, 14, 15, 16, 17, 18,
                                19, 20, 21, 22, 23, 24,
                                25, 26, 27, 28, 29, 30,
                                31, 32, 33, 34, 35, 36};
    std::vector<int> vect = { 2, 3, 4, 5, 6, 7 };
    std::vector<int> myres = { 112, 274, 436, 598, 760, 922};
    std::vector<int> res = lentVertScheme(matr, vect, 6, 6);
    if (rank == 0) EXPECT_EQ(res, myres);
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

