// Copyright 2022 Yashina Anastasia

#include <gtest/gtest.h>
#include <../../../modules/task_2/yashina_a_mult_mat_vect_hor/mult_mat_vect_hor.h>
#include <gtest-mpi-listener.hpp>



TEST(Matrix_On_Vector_Multip_MPI, test1_mat_3x3_EQ) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector <int> Mat(9);
    std::vector <int> Vec(3);
    std::vector <int> res = { 3, 3, 3 };
    std::vector <int> Res(3, 0);

    Mat = { 1, 1, 1, 1, 1, 1, 1, 1, 1 };
    Vec = { 1, 1, 1 };

    Res = Multip(Mat, 3, 3, Vec);
    if (rank == 0) {
        EXPECT_EQ(Res, res);
    }
}

TEST(Matrix_On_Vector_Multip_MPI, test2_ident_mat_3x3_EQ) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector <int> Mat(9);
    std::vector <int> Vec(3);
    std::vector <int> res = { 1, 2, 3 };
    std::vector <int> Res(3, 0);

    Mat = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };
    Vec = { 1, 2, 3 };

    Res = Multip(Mat, 3, 3, Vec);
    if (rank == 0) {
        EXPECT_EQ(Res, res);
    }
}

TEST(Matrix_On_Vector_Multip_MPI, test3_mat_4x3_EQ) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector <int> Mat(9);
    std::vector <int> Vec(3);
    std::vector <int> res = { 6, 15, 24, 33 };
    std::vector <int> Res(4, 0);

    Mat = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
    Vec = { 1, 1, 1 };

    Res = Multip(Mat, 4, 3, Vec);
    if (rank == 0) {
        EXPECT_EQ(Res, res);
    }
}
TEST(Matrix_On_Vector_Multip_MPI, test4_negative_in_rows) {
    std::vector <int> Mat(9);
    std::vector <int> Vec(3);
    Mat = { 1, 1, 1, 1, 1, 1, 1, 1, 1 };
    Vec = { 1, 1, 1 };
    EXPECT_ANY_THROW(Multip(Mat, -3, 3, Vec););
}

TEST(Matrix_On_Vector_Multip_MPI, test5_negative_in_cols) {
    std::vector <int> Mat(9);
    std::vector <int> Vec(3);
    Mat = { 1, 1, 1, 1, 1, 1, 1, 1, 1 };
    Vec = { 1, 1, 1 };
    EXPECT_ANY_THROW(Multip(Mat, 3, -3, Vec););
}

TEST(Matrix_On_Vector_Multip_MPI, test6_wrong_size_for_multip) {
    std::vector <int> Mat(9);
    std::vector <int> Vec(2);
    Mat = { 1, 1, 1, 1, 1, 1, 1, 1, 1 };
    Vec = { 1, 1 };
    EXPECT_ANY_THROW(Multip(Mat, 3, 3, Vec););
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
