// Copyright 2022 Terina Alina
#include <gtest/gtest.h>
#include <vector>
#include <random>
#include <algorithm>
#include <iterator>
#include <string>
#include <iostream>
#include "./gauss_jord.h"
#include <gtest-mpi-listener.hpp>

TEST(TestMPI, SolveMatr_rang_52) {
    std::random_device dev;
    std::mt19937 gen(dev());
    const int matrix_size = 50;
    int rank;
    std::vector<double> vec;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<std::vector<double>> matrix;
    std::vector<double> resultP;
    std::vector<double> resultO;
    std::vector<double> parmatrix;

    if (rank == 0) {
        vec.resize(matrix_size);
        matrix.resize(matrix_size * (matrix_size + 1));

        for (int i = 0; i < matrix_size; i++) {
            vec[i] = gen() % 100 + 1;
        }

        matrix = getRandomMatrix(vec, matrix_size);
    }
    if (rank == 0) {
        for (int i = 0; i < matrix.size(); i++) {
            for (int j = 0; j < matrix[i].size(); j++) {
                parmatrix.push_back(matrix[i][j]);
            }
        }
    }

    resultP = ParGJ(parmatrix, matrix_size);

    if (rank == 0) {
        for (int i = 0; i < matrix_size; i++) {
            ASSERT_NO_THROW(resultP[i]);
        }
    }

    vec.clear();
    matrix.clear();
    resultP.clear();
    resultO.clear();
    parmatrix.clear();
    MPI_Barrier(MPI_COMM_WORLD);
}
TEST(TestMPI, SolveMatr_rang_10) {
    std::random_device dev;
    std::mt19937 gen(dev());
    const int matrix_size = 10;
    int rank;
    std::vector<double> vec;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<std::vector<double>> matrix;
    std::vector<double> resultP;
    std::vector<double> resultO;
    std::vector<double> parmatrix;

    if (rank == 0) {
        vec.resize(matrix_size);
        matrix.resize(matrix_size * (matrix_size + 1));

        for (int i = 0; i < matrix_size; i++) {
            vec[i] = gen() % 100 + 1;
        }

        matrix = getRandomMatrix(vec, matrix_size);
    }
    if (rank == 0) {
        for (int i = 0; i < matrix.size(); i++) {
            for (int j = 0; j < matrix[i].size(); j++) {
                parmatrix.push_back(matrix[i][j]);
            }
        }
    }

    resultP = ParGJ(parmatrix, matrix_size);

    if (rank == 0) {
        for (int i = 0; i < matrix_size; i++) {
            std::cout << resultP[i] << ' ';
        }
    }

    vec.clear();
    matrix.clear();
    resultP.clear();
    resultO.clear();
    parmatrix.clear();
    MPI_Barrier(MPI_COMM_WORLD);
}
TEST(TestMPI, SolveMatr_rang_100) {
    std::random_device dev;
    std::mt19937 gen(dev());

    const int matrix_size = 100;
    int rank;
    std::vector<double> vec;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<std::vector<double>> matrix;
    std::vector<double> resultP;
    std::vector<double> resultO;
    std::vector<double> parmatrix;

    if (rank == 0) {
        vec.resize(matrix_size);
        matrix.resize(matrix_size * (matrix_size + 1));

        for (int i = 0; i < matrix_size; i++) {
            vec[i] = gen() % 100 + 1;
        }

        matrix = getRandomMatrix(vec, matrix_size);
        resultO = ordinaryGJ(matrix, vec);
    }
    if (rank == 0) {
        for (int i = 0; i < matrix.size(); i++) {
            for (int j = 0; j < matrix[i].size(); j++) {
                parmatrix.push_back(matrix[i][j]);
            }
        }
    }

    if (rank == 0) {
        for (int i = 0; i < matrix_size; i++) {
            ASSERT_NO_THROW(resultO[i]);
        }
    }

    vec.clear();
    matrix.clear();
    resultP.clear();
    resultO.clear();
    parmatrix.clear();
    MPI_Barrier(MPI_COMM_WORLD);
}
TEST(MPIGaussJordanMethod, SolveMatr_rang_10) {
    std::random_device dev;
    std::mt19937 gen(dev());

    const int matrix_size = 10;
    int rank;
    std::vector<double> vec;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<std::vector<double>> matrix;
    std::vector<double> resultP;
    std::vector<double> resultO;
    std::vector<double> parmatrix;

    if (rank == 0) {
        vec.resize(matrix_size);
        matrix.resize(matrix_size * (matrix_size + 1));

        for (int i = 0; i < matrix_size; i++) {
            vec[i] = gen() % 100 + 1;
        }

        matrix = getRandomMatrix(vec, matrix_size);
        resultO = ordinaryGJ(matrix, vec);
    }
    if (rank == 0) {
        for (int i = 0; i < matrix.size(); i++) {
            for (int j = 0; j < matrix[i].size(); j++) {
                parmatrix.push_back(matrix[i][j]);
            }
        }
    }
    if (rank == 0) {
        for (int i = 0; i < matrix_size; i++) {
            std::cout << resultO[i] << ' ';
        }
    }

    vec.clear();
    matrix.clear();
    resultP.clear();
    resultO.clear();
    parmatrix.clear();
    MPI_Barrier(MPI_COMM_WORLD);
}
TEST(MPIGaussJordanMethod, solvematrvec_rang200) {
    std::random_device dev;
    std::mt19937 gen(dev());
    const int matrix_size = 200;
    int rank;
    std::vector<double> vec;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<std::vector<double>> matrix;
    std::vector<double> resultP;
    std::vector<double> resultO;
    std::vector<double> parmatrix;

    if (rank == 0) {
        vec.resize(matrix_size);
        matrix.resize(matrix_size * (matrix_size + 1));

        for (int i = 0; i < matrix_size; i++) {
            vec[i] = gen() % 100 + 1;
        }

        matrix = getRandomMatrix(vec, matrix_size);
    }
    if (rank == 0) {
        for (int i = 0; i < matrix.size(); i++) {
            for (int j = 0; j < matrix[i].size(); j++) {
                parmatrix.push_back(matrix[i][j]);
            }
        }
    }

    resultP = ParGJ(parmatrix, matrix_size);

    if (rank == 0) {
        for (int i = 0; i < matrix_size; i++) {
            ASSERT_NO_THROW(resultP[i]);
        }
    }

    vec.clear();
    matrix.clear();
    resultP.clear();
    resultO.clear();
    parmatrix.clear();
    MPI_Barrier(MPI_COMM_WORLD);
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

