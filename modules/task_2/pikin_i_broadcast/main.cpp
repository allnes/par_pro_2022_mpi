// Copyright 2022 Pikin Ilya
#include <mpi.h>
#include <gtest/gtest.h>
#include <iostream>
#include <random>
#include <gtest-mpi-listener.hpp>
#include "./broadcast.h"

TEST(BROADCAST_TESTS, Int_type_test) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int number;

    if (rank == 0) {
        number = 123456;
    }

    myBroadcast(&number, 1, MPI_INT, 0, MPI_COMM_WORLD);

    ASSERT_EQ(number, 123456);
}

TEST(BROADCAST_TESTS, Float_type_test) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    float number;

    if (rank == 0) {
        number = 666.0f;
    }

    myBroadcast(&number, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);

    ASSERT_EQ(number, 666.0f);
}

TEST(BROADCAST_TESTS, Double_type_test) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    double number;

    if (rank == 0) {
        number = 123456.0;
    }

    myBroadcast(&number, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    ASSERT_EQ(number, 123456.0);
}

TEST(BROADCAST_TESTS, Array_broadcast_test) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int* data1, *data2;
    int local_result = 0, global_result;

    data1 = new int[50];
    data2 = new int[50];

    if (rank == 0) {
        std::random_device dev;
        std::mt19937 gen(dev());

        for (int i = 0; i < 50; i++) {
            data1[i] = gen() % 100;
            data2[i] = data1[i];
        }
    }

    MPI_Bcast(data1, 50, MPI_INT, 0, MPI_COMM_WORLD);

    myBroadcast(data2, 50, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < 50; i++) {
        if (data1[i] != data2[i]) {
            local_result = 1;
            std::cout << data1[i] << " != " << data2[i]
                << " in proc " << rank << std::endl;
        }
    }

    MPI_Reduce(&local_result, &global_result, 1, MPI_INT,
        MPI_MAX, 0, MPI_COMM_WORLD);

    delete[] data1;
    delete[] data2;

    if (rank == 0) {
        ASSERT_EQ(global_result, 0);
    }
}

TEST(BROADCAST_TESTS, Root_test) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int data;

    for (int i = 0; i < size; i++) {
        if (rank == i) {
            data = i;
        }
        /*std::cout << "Iter #" << i;
        std::cout << " by proc #" << rank;
        std::cout << std::endl;*/
        myBroadcast(&data, 1, MPI_INT, i, MPI_COMM_WORLD);
        /*std::cout << "data = " << data;
        std::cout << " by proc #" << rank;
        std::cout << std::endl;*/
        ASSERT_EQ(data, i);
    }
}

TEST(BROADCAST_TESTS, Random_buffer_size_test) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int* data, buf_size;

    if (rank == 0) {
        std::random_device dev;
        std::mt19937 gen(dev());

        buf_size = 10 + (gen() % 90);
    }

    myBroadcast(&buf_size, 1, MPI_INT, 0, MPI_COMM_WORLD);

    data = new int[buf_size];

    if (rank == 0) {
        for (int i = 0; i < buf_size; i++) {
            data[i] = i + 100;
        }
    }

    myBroadcast(data, buf_size, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < buf_size; i++) {
        ASSERT_EQ(data[i], i + 100);
    }

    delete[] data;
}

TEST(BROADCAST_TESTS, Time_test) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int n = 10 * size, local_result = 0;
    double t1, t2;
    int* vec, * mat, * local_column, * result_vec1, *result_vec2;


    vec = new int[n];
    local_column = new int[n];
    result_vec1 = new int[size];
    result_vec2 = new int[size];

    if (rank == 0) {
        std::random_device dev;
        std::mt19937 gen(dev());

        mat = new int[n * size];

        for (int i = 0; i < n; i++) {
            vec[i] = gen() % 100;
            /*std::cout << vec[i] << " ";*/
        }
        /*std::cout << std::endl;*/
        for (int i = 0; i < n * size; i++)
            mat[i] = gen() % 100;

        /*for (int i = 0; i < size; i++) {
            for (int j = 0; j < n; j++) {
                std::cout << mat[i * n + j] << " ";
            }
            std::cout << std::endl;
        }*/

        t1 = MPI_Wtime();
    }

    //  Test 1
    myBroadcast(vec, n, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Scatter(mat, n, MPI_INT, local_column, n, MPI_INT,
        0, MPI_COMM_WORLD);

    for (int i = 0; i < n; i++) {
        local_result += vec[i] * local_column[i];
    }

    MPI_Gather(&local_result, 1, MPI_INT, result_vec1, 1, MPI_INT,
        0, MPI_COMM_WORLD);

    local_result = 0;
    if (rank == 0) {
        t2 = MPI_Wtime();
        std::cout.precision(17);
        std::cout << std::fixed;
        std::cout << "myBroadcast time: " << t2 - t1 << "\n";

        /*for (int i = 0; i < size; i++)
            std::cout << result_vec1[i] << " ";
        std::cout << std::endl;*/

        t1 = MPI_Wtime();
    }

    //  Test 2
    MPI_Bcast(vec, n, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Scatter(mat, n, MPI_INT, local_column, n, MPI_INT,
        0, MPI_COMM_WORLD);

    for (int i = 0; i < n; i++) {
        local_result += vec[i] * local_column[i];
    }

    MPI_Gather(&local_result, 1, MPI_INT, result_vec2, 1, MPI_INT,
        0, MPI_COMM_WORLD);

    local_result = 0;
    if (rank == 0) {
        t2 = MPI_Wtime();
        std::cout << "MPI_Bcast time: " << t2 - t1 << "\n";

        /*for (int i = 0; i < size; i++)
            std::cout << result_vec2[i] << " ";
        std::cout << std::endl;*/

        for (int i = 0; i < size; i++)
            ASSERT_EQ(result_vec1[i], result_vec2[i]);
        delete[] mat;
    }

    delete[] vec;
    delete[] local_column;
    delete[] result_vec1;
    delete[] result_vec2;
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
