// Copyright 2022 Polyackov Lev
#include <gtest/gtest.h>
#include <vector>
#include "./alg_fox.h"
#include <gtest-mpi-listener.hpp>

TEST(Alg_Fox_MPI, Can_Control_Number_Of_Processes) {
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::vector<double> vec = { 0.0, 1.0, 2.0, 3.0 };

    if (size == 1 || size == 4) {
        ASSERT_NO_THROW(getResOfFoxMult(vec, vec, 2));
    }

    if (size == 2 || size == 3) {
        ASSERT_ANY_THROW(getResOfFoxMult(vec, vec, 2));
    }

}

TEST(Alg_Fox_MPI, No_Throw_Find_Mult_Of_An_Empty_Matr) {
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size == 1 || size == 4) {
        std::vector<double> vec;
        ASSERT_NO_THROW(getResOfFoxMult(vec, vec, 0));
    }
}

TEST(Alg_Fox_MPI, Matr_Is_Zero_When_Find_Mult_Of_An_Empty_Matr) {
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size == 1 || size == 4) {
        std::vector<double> vec;
        std::vector<double> res = getResOfFoxMult(vec, vec, 0);
        ASSERT_EQ(0, res.size());
    }
}

TEST(Alg_Fox_MPI, Can_Find_Mult_Of_1x1_Matr) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (size == 1 || size == 4) {
        std::vector<double> vec1, vec2;

        if (rank == 0) {
            vec1.push_back(2.0);
            vec2.push_back(3.0);
        }

        std::vector<double> res = getResOfFoxMult(vec1, vec2, 1);

        if (rank == 0) {
            for (int i = 0; i < vec1.size(); i++) {
                ASSERT_NEAR(6.0, res[i], std::numeric_limits<double>::epsilon());
            }
        }
    }
}

TEST(Alg_Fox_MPI, Can_Find_Mult_Of_4x4_Matr) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (size == 1 || size == 4) {
        std::vector<double> vec1, vec2;
        int matr_size = 4;
        if (rank == 0) {
            vec1 = { 3, 1, 4, 1,
                     8, 4, 2, 4,
                     4, 6, 7, 5,
                     7, 5, 4, 2 };

            vec2 = { 3, 6, 7, 6,
                    2, 9, 2, 1,
                    7, 7, 4, 2,
                    5, 3, 5, 3 };
        }


        std::vector<double> res = getResOfFoxMult(vec1, vec2, matr_size);

        if (rank == 0) {
            std::vector<double> reference_res;
            reference_res.resize(matr_size * matr_size);
            getResOfMatrMult(vec1.data(), vec2.data(), reference_res.data(), matr_size);
            for (int i = 0; i < reference_res.size(); i++) {
                ASSERT_NEAR(reference_res[i], res[i], std::numeric_limits<double>::epsilon());
            }
        }

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
