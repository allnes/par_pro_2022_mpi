// Copyright 2022 Nikiforova Lada
#include <gtest/gtest.h>

#include <cmath>
#include <functional>
#include <vector>
#include <gtest-mpi-listener.hpp>

#include "./n_dev_rectangle_method.h"

TEST(Parallel_Operations_MPI, Test_x_2_pl_y_2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::function<double(std::vector<double>*)> foo =
        [](std::vector<double>* vector) {
            return pow(vector->at(0), 2) + pow(vector->at(1), 2);
        };
    std::vector<double> ADots{-1.0, -1.0};
    std::vector<double> BDots{1.0, 1.0};
    double eps = 0.01;
    double result = parallelIntegrall(ADots, BDots, foo, eps);
    if (rank == 0) {
        ASSERT_NEAR(result, 2.66, 10*eps);
    }
}

TEST(Parallel_Operations_MPI, Test_x_pl_y_pl_z) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::function<double(std::vector<double>*)> foo =
        [](std::vector<double>* vector) {
            return vector->at(0) + vector->at(1) + vector->at(2);
        };
    std::vector<double> ADots{0, 0, 0};
    std::vector<double> BDots{1.0, 1.0, 1.0};
    double eps = 0.01;
    double result = parallelIntegrall(ADots, BDots, foo, eps);
    if (rank == 0) {
        ASSERT_NEAR(result, 1.5, 0.1);
    }
}

TEST(Parallel_Operations_MPI, Test_x_2_dot_y) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::function<double(std::vector<double>*)> foo =
        [](std::vector<double>* vector) {
            return pow(vector->at(0), 2) + vector->at(1);
        };
    std::vector<double> ADots{0.0, -2.0};
    std::vector<double> BDots{1.0, 2.0};
    double eps = 0.01;
    double result = parallelIntegrall(ADots, BDots, foo, eps);
    if (rank == 0) {
        ASSERT_NEAR(result, 1.33, 0.1);
    }
}

TEST(Parallel_Operations_MPI, Test_sin_x_dot_y) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::function<double(std::vector<double>*)> foo =
        [](std::vector<double>* vector) {
            return sin(vector->at(0)*vector->at(1));
        };
    std::vector<double> ADots{0.0, 0.0};
    std::vector<double> BDots{1.0, 1.0};
    double eps = 0.01;
    double result = parallelIntegrall(ADots, BDots, foo, eps);
    if (rank == 0) {
        ASSERT_NEAR(result, 0.2398, 0.1);
    }
}

TEST(Parallel_Operations_MPI, Test_sin_x_pl_tan_y) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::function<double(std::vector<double>*)> foo =
        [](std::vector<double>* vector) {
            return sin(vector->at(0)) + tan(vector->at(1));
        };
    std::vector<double> ADots{0.0, -1.0};
    std::vector<double> BDots{2.0, 1.0};
    double eps = 0.01;
    double result = parallelIntegrall(ADots, BDots, foo, eps);
    if (rank == 0) {
        ASSERT_NEAR(result, 2.8322, 0.1);
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
