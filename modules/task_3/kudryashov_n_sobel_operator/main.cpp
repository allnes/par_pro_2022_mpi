  // Copyright 2022 Kudryashov Nikita
#include <gtest/gtest.h>
#include <mpi.h>
#include <vector>
#include "./kudryashov_n_sobel_operator.h"
#include <gtest-mpi-listener.hpp>

TEST(sobel_operator, test_little_square_image) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int height = 2;
    int width = 2;
    std::vector<std::vector<int>> img(height, std::vector<int>(width));

    if (rank == 0) {
        img = generateRandomImage(height, width);
    }

    std::vector<std::vector<int>>  global_res = calcSobelParallel(img, height, width);

    if (rank == 0) {
        std::vector<std::vector<int>> sequential_res = calcSobel(img, height, width);
        ASSERT_EQ(global_res, sequential_res);
    }
}

TEST(sobel_operator, test_little_not_square_image1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int height = 3;
    int width = 1;
    std::vector<std::vector<int>> img(height, std::vector<int>(width));

    if (rank == 0) {
        img = generateRandomImage(height, width);
    }

    std::vector<std::vector<int>>  global_res = calcSobelParallel(img, height, width);

    if (rank == 0) {
        std::vector<std::vector<int>> sequential_res = calcSobel(img, height, width);
        ASSERT_EQ(global_res, sequential_res);
    }
}

TEST(sobel_operator, test_little_not_square_image2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int height = 1;
    int width = 3;
    std::vector<std::vector<int>> img(height, std::vector<int>(width));

    if (rank == 0) {
        img = generateRandomImage(height, width);
    }

    std::vector<std::vector<int>>  global_res = calcSobelParallel(img, height, width);

    if (rank == 0) {
        std::vector<std::vector<int>> sequential_res = calcSobel(img, height, width);
        ASSERT_EQ(global_res, sequential_res);
    }
}

TEST(sobel_operator, test_large_square_image) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int height = 20;
    int width = 20;
    std::vector<std::vector<int>> img(height, std::vector<int>(width));

    if (rank == 0) {
        img = generateRandomImage(height, width);
    }

    std::vector<std::vector<int>>  global_res = calcSobelParallel(img, height, width);

    if (rank == 0) {
        std::vector<std::vector<int>> sequential_res = calcSobel(img, height, width);
        ASSERT_EQ(global_res, sequential_res);
    }
}

TEST(sobel_operator, test_large_not_square_image1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int height = 15;
    int width = 25;
    std::vector<std::vector<int>> img(height, std::vector<int>(width));

    if (rank == 0) {
        img = generateRandomImage(height, width);
    }

    std::vector<std::vector<int>>  global_res = calcSobelParallel(img, height, width);

    if (rank == 0) {
        std::vector<std::vector<int>> sequential_res = calcSobel(img, height, width);
        ASSERT_EQ(global_res, sequential_res);
    }
}

TEST(sobel_operator, test_large_not_square_image2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int height = 25;
    int width = 15;
    std::vector<std::vector<int>> img(height, std::vector<int>(width));

    if (rank == 0) {
        img = generateRandomImage(height, width);
    }

    std::vector<std::vector<int>>  global_res = calcSobelParallel(img, height, width);

    if (rank == 0) {
        std::vector<std::vector<int>> sequential_res = calcSobel(img, height, width);
        ASSERT_EQ(global_res, sequential_res);
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
