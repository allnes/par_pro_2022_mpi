  // Copyright 2022 Mironov Nikita
#include <gtest/gtest.h>
#include "../../../modules/task_2/mironov_n_gaussian_method_vertical/gaussian_method_vertical.h"
#include <gtest-mpi-listener.hpp>

TEST(Gaussian_method_vertical, test_1) {
    int procId;
    MPI_Comm_rank(MPI_COMM_WORLD, &procId);
    std::vector<double> coefs;
    const size_t rows = 2;
    const size_t columns = 3;

    if (procId == 0) {
        coefs = {
            -1, 1, 5,
            2, -1, -7
        };
    }

    std::vector <double> paralSolution = ParSolution(coefs, rows, columns);
    if (procId == 0) {
        std::vector <double> nonParalSolution =
            nonParSolution(coefs, rows, columns);
        ASSERT_TRUE(checkEqualOfMatrix(nonParalSolution, paralSolution));
        ASSERT_TRUE(checkSolution(coefs, rows, columns, paralSolution));
    }
}

TEST(Gaussian_method_vertical, test_2) {
    int procId;
    MPI_Comm_rank(MPI_COMM_WORLD, &procId);
    std::vector<double> coefs;
    const size_t rows = 3;
    const size_t columns = 4;

    if (procId == 0) {
        coefs = {
            2, 3, -4, 9,
            3, 4, 1, 3,
            1, 0, 3, 5
        };
    }

    std::vector <double> paralSolution = ParSolution(coefs, rows, columns);
    if (procId == 0) {
        std::vector <double> nonParalSolution =
            nonParSolution(coefs, rows, columns);
        ASSERT_TRUE(checkEqualOfMatrix(nonParalSolution, paralSolution));
        ASSERT_TRUE(checkSolution(coefs, rows, columns, paralSolution));
    }
}

TEST(Gaussian_method_vertical, test_3) {
    int procId;
    MPI_Comm_rank(MPI_COMM_WORLD, &procId);
    std::vector<double> coefs;
    const size_t rows = 3;
    const size_t columns = 4;

    if (procId == 0) {
        coefs = {
            2, 3, -1, 9,
            1, -2, 1, 3,
            1, 0, 2, 2
        };
    }

    EXPECT_ANY_THROW(ParSolution(coefs, rows, columns + 1));
}

TEST(Gaussian_method_vertical, test_4) {
    int procId;
    MPI_Comm_rank(MPI_COMM_WORLD, &procId);
    std::vector<double> coefs;
    const size_t rows = 3;
    const size_t columns = 4;

    if (procId == 0) {
        coefs = {
            2, 3, -1, 9,
            1, -2, 1, 3,
            1, 0, 2, 2
        };
    }

    if (procId == 0) {
        EXPECT_ANY_THROW(nonParSolution(coefs, rows, columns + 1));
    }
}

TEST(Gaussian_method_vertical, test_5) {
    int procId;
    MPI_Comm_rank(MPI_COMM_WORLD, &procId);
    std::vector<double> coefs;
    const size_t rows = 3;
    const size_t columns = 4;

    if (procId == 0) {
        coefs = {
            2, 3, -1,
            1, -2, 1,
            1, 0, 2
        };
    }

    EXPECT_ANY_THROW(ParSolution(coefs, rows, columns));
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
