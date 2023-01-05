// Copyright 2022 Churkin Alexander
#include <gtest/gtest.h>
#include <vector>
#include "./vector_sum.h"
#include <gtest-mpi-listener.hpp>

TEST(Vector_Sum_MPI, Test_Vector_Sum_1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int global_vector_size = 9;

    std::vector<int> globalVector;

    double t1Par, t2Par, t1Seq, t2Seq;

    if (rank == 0) {
        globalVector = getRandomVector(global_vector_size);
        t1Par = MPI_Wtime();
    }

    int resPar = getSumParallel(globalVector, global_vector_size);

    if (rank == 0) {
        t2Par = MPI_Wtime();

        t1Seq = t2Par;
        int resSeq = getSumSequential(globalVector);
        t2Seq = MPI_Wtime();
        ASSERT_EQ(resPar, resSeq);
        std::cout << "Test 1 (9 elements):" << '\n';
        std::cout << "Sequential execution time: " << (t2Seq - t1Seq) << '\n';
        std::cout << "Parralel execution time: " << (t2Par - t1Par) << "\n\n";
    }
}

TEST(Vector_Sum_MPI, Test_Vector_Sum_2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int global_vector_size = 1000;

    std::vector<int> globalVector;

    double t1Par, t2Par, t1Seq, t2Seq;

    if (rank == 0) {
        globalVector = getRandomVector(global_vector_size);
        t1Par = MPI_Wtime();
    }

    int resPar = getSumParallel(globalVector, global_vector_size);

    if (rank == 0) {
        t2Par = MPI_Wtime();

        t1Seq = t2Par;
        int resSeq = getSumSequential(globalVector);
        t2Seq = MPI_Wtime();
        ASSERT_EQ(resPar, resSeq);
        std::cout << "Test 2 (1000 elements):" << '\n';
        std::cout << "Sequential execution time: " << (t2Seq - t1Seq) << '\n';
        std::cout << "Parralel execution time: " << (t2Par - t1Par) << "\n\n";
    }
}

TEST(Vector_Sum_MPI, Test_Vector_Sum_3) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int global_vector_size = 10000;

    std::vector<int> globalVector;

    double t1Par, t2Par, t1Seq, t2Seq;

    if (rank == 0) {
        globalVector = getRandomVector(global_vector_size);
        t1Par = MPI_Wtime();
    }

    int resPar = getSumParallel(globalVector, global_vector_size);

    if (rank == 0) {
        t2Par = MPI_Wtime();

        t1Seq = t2Par;
        int resSeq = getSumSequential(globalVector);
        t2Seq = MPI_Wtime();
        ASSERT_EQ(resPar, resSeq);
        std::cout << "Test 3 (10.000 elements):" << '\n';
        std::cout << "Sequential execution time: " << (t2Seq - t1Seq) << '\n';
        std::cout << "Parralel execution time: " << (t2Par - t1Par) << "\n\n";
    }
}

TEST(Vector_Sum_MPI, Test_Vector_Sum_4) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int global_vector_size = 100000;

    std::vector<int> globalVector;

    double t1Par, t2Par, t1Seq, t2Seq;

    if (rank == 0) {
        globalVector = getRandomVector(global_vector_size);
        t1Par = MPI_Wtime();
    }

    int resPar = getSumParallel(globalVector, global_vector_size);

    if (rank == 0) {
        t2Par = MPI_Wtime();

        t1Seq = t2Par;
        int resSeq = getSumSequential(globalVector);
        t2Seq = MPI_Wtime();
        ASSERT_EQ(resPar, resSeq);
        std::cout << "Test 4 (100.000 elements):" << '\n';
        std::cout << "Sequential execution time: " << (t2Seq - t1Seq) << '\n';
        std::cout << "Parralel execution time: " << (t2Par - t1Par) << "\n\n";
    }
}

TEST(Vector_Sum_MPI, Test_Vector_Sum_5) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int global_vector_size = 1000000;

    std::vector<int> globalVector;

    double t1Par, t2Par, t1Seq, t2Seq;

    if (rank == 0) {
        globalVector = getRandomVector(global_vector_size);
        t1Par = MPI_Wtime();
    }

    int resPar = getSumParallel(globalVector, global_vector_size);

    if (rank == 0) {
        t2Par = MPI_Wtime();

        t1Seq = t2Par;
        int resSeq = getSumSequential(globalVector);
        t2Seq = MPI_Wtime();
        ASSERT_EQ(resPar, resSeq);
        std::cout << "Test 5 (1.000.000 elements):" << '\n';
        std::cout << "Sequential execution time: " << (t2Seq - t1Seq) << '\n';
        std::cout << "Parralel execution time: " << (t2Par - t1Par) << "\n\n";
    }
}

TEST(Vector_Sum_MPI, Test_Vector_Sum_6) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int global_vector_size = 10000000;

    std::vector<int> globalVector;

    double t1Par, t2Par, t1Seq, t2Seq;

    if (rank == 0) {
        globalVector = getRandomVector(global_vector_size);
        t1Par = MPI_Wtime();
    }

    int resPar = getSumParallel(globalVector, global_vector_size);

    if (rank == 0) {
        t2Par = MPI_Wtime();

        t1Seq = t2Par;
        int resSeq = getSumSequential(globalVector);
        t2Seq = MPI_Wtime();
        ASSERT_EQ(resPar, resSeq);
        std::cout << "Test 5 (10.000.000 elements):" << '\n';
        std::cout << "Sequential execution time: " << (t2Seq - t1Seq) << '\n';
        std::cout << "Parralel execution time: " << (t2Par - t1Par) << "\n\n";
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners &listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}
