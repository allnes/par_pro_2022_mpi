// Copyright 2023 Pikin Ilya
#include <mpi.h>
#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include "./merge_q_sort.h"
#include <gtest-mpi-listener.hpp>

TEST(MERGE_Q_SORT_TESTS, Small_test) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int n = 20;
    std::vector<double> vec;
    std::vector<double> copy;
    std::vector<double> copy2;


    if (rank == 0) {
        vec = getRandVec(n);
        copy = vec;
        copy2 = vec;

        /*for (int i = 0; i < n; i++)
            std::cout << vec[i] << ' ';
        std::cout << '\n';*/

        quickSort(copy.data(), copy.size());
        std::sort(copy2.data(), copy2.data() + n);
    }

    parallelQuickSort(vec.data(), vec.size());

    if (rank == 0) {
        /*for (int i = 0; i < n; i++)
            std::cout << copy[i] << ' ';
        std::cout << '\n';
        
        for (int i = 0; i < n; i++)
            std::cout << copy2[i] << ' ';
        std::cout << '\n';

        for (int i = 0; i < n; i++)
            std::cout << vec[i] << ' ';
        std::cout << '\n';*/

        for (int i = 0; i < n; i++) {
            ASSERT_EQ(vec[i], copy[i]);
            ASSERT_EQ(vec[i], copy2[i]);
        }
    }
}

TEST(MERGE_Q_SORT_TESTS, Random_buffer_size_test) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int n;
    std::vector<double> vec;
    std::vector<double> copy;
    std::vector<double> copy2;

    if (rank == 0) {
        std::random_device dev;
        std::mt19937 generator(dev());

        n = generator() % 1000;

        vec = getRandVec(n);
        copy = vec;
        copy2 = vec;

        quickSort(copy.data(), copy.size());
        std::sort(copy2.data(), copy2.data() + n);
    }

    parallelQuickSort(vec.data(), vec.size());

    if (rank == 0) {
        for (int i = 0; i < n; i++) {
            ASSERT_EQ(vec[i], copy[i]);
            ASSERT_EQ(vec[i], copy2[i]);
        }
    }
}

TEST(MERGE_Q_SORT_TESTS, One_and_zero_size_buf_test) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int n = 0;
    int m = 1;
    std::vector<double> vec1;
    std::vector<double> vec2;
    std::vector<double> vec2_copy;

    if (rank == 0) {
        vec1 = getRandVec(n);
        vec2 = getRandVec(m);
        vec2_copy = vec2;
    }

    parallelQuickSort(vec1.data(), vec1.size());
    parallelQuickSort(vec2.data(), vec2.size());


    if (rank == 0) {
        ASSERT_EQ(vec1.size(), 0);
        ASSERT_EQ(vec2.size(), 1);
        ASSERT_EQ(vec2[0], vec2_copy[0]);
    }
}

TEST(MERGE_Q_SORT_TESTS, Sorting_a_sorted_array) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int n;
    std::vector<double> vec;
    std::vector<double> copy;

    if (rank == 0) {
        std::random_device dev;
        std::mt19937 generator(dev());

        n = 10 + generator() % 1000;

        vec = getRandVec(n);
        std::sort(vec.data(), vec.data() + n);
        copy = vec;
    }

    parallelQuickSort(copy.data(), copy.size());

    if (rank == 0) {
        for (int i = 0; i < n; i++) {
            ASSERT_EQ(vec[i], copy[i]);
        }
    }
}

TEST(MERGE_Q_SORT_TESTS, Array_with_the_same_values) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int n;
    std::vector<double> vec;
    std::vector<double> copy;

    if (rank == 0) {
        std::random_device dev;
        std::mt19937 generator(dev());

        n = 10 + generator() % 1000;

        std::vector<double> filled_vec(n, 666.666);
        vec = filled_vec;
        copy = vec;
    }

    parallelQuickSort(copy.data(), copy.size());

    if (rank == 0) {
        for (int i = 0; i < n; i++) {
            ASSERT_EQ(vec[i], copy[i]);
        }
    }
}

TEST(MERGE_Q_SORT_TESTS, Time_test) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int n = 1000000;
    std::vector<double> vec;
    std::vector<double> copy;
    std::vector<double> copy2;

    double t1, t2;
    if (rank == 0) {
        vec = getRandVec(n);
        copy = vec;
        copy2 = vec;

        std::cout.precision(17);
        std::cout << std::fixed;

        t1 = MPI_Wtime();
        quickSort(copy.data(), copy.size());
        t2 = MPI_Wtime();
        std::cout << "My quickSort time: " << t2 - t1 << "\n";
        t1 = MPI_Wtime();
        std::sort(copy2.data(), copy2.data() + n);
        t2 = MPI_Wtime();
        std::cout << "std::sort time: " << t2 - t1 << "\n";
        t1 = MPI_Wtime();
    }

    parallelQuickSort(vec.data(), vec.size());

    if (rank == 0) {
        t2 = MPI_Wtime();
        std::cout << "parallelQuickSort time: " << t2 - t1 << "\n";

        for (int i = 0; i < n; i++) {
            ASSERT_EQ(vec[i], copy[i]);
            ASSERT_EQ(vec[i], copy2[i]);
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
