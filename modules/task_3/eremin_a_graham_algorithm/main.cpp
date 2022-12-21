// Copyright 2022 Eremin Aleksandr
#include <gtest/gtest.h>
#include <vector>
#include "./ops_mpi.h"
#include <gtest-mpi-listener.hpp>

TEST(Parallel_Operations_MPI, Graham_Method) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<Point> global_vec;
    std::vector<Point> global_vec_answer;

    if (rank == 0) {
        global_vec = { {0, 1},  {0, 4}, {2, 7},  {2, 16}, {3, 16},
                      {3, 16}, {5, 1}, {5, 12}, {5, 17}, {7, 12} };
        global_vec_answer = { {0, 1}, {0, 4}, {2, 16}, {5, 17}, {7, 12}, {5, 1} };
        vector<Point> not_parallel = GrahamMethod(global_vec);
        ASSERT_EQ(global_vec_answer.size(), not_parallel.size());
        for (vector<int>::size_type i = 0; i < global_vec_answer.size(); ++i) {
            ASSERT_EQ(global_vec_answer[i].x, not_parallel[i].x);
            ASSERT_EQ(global_vec_answer[i].y, not_parallel[i].y);
        }
    }
}

TEST(Parallel_Operations_MPI, GrahamParallel_Method_1) {
    int rank, Commsize;
    MPI_Comm_size(MPI_COMM_WORLD, &Commsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<Point> global_vec;
    const vector<int>::size_type size = 12;

    if (rank == 0) {
        global_vec = random(size);
    }

    vector<Point> parallel = parallelGrahamMethod(global_vec, size);
    if (rank == 0) {
        vector<Point> not_parallel = GrahamMethod(global_vec);
        ASSERT_EQ(parallel.size(), not_parallel.size());
        for (vector<int>::size_type i = 0; i < parallel.size(); ++i) {
            ASSERT_EQ(parallel[i].x, not_parallel[i].x);
            ASSERT_EQ(parallel[i].y, not_parallel[i].y);
        }
    }
}

TEST(Parallel_Operations_MPI, GrahamParallel_Method_2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<Point> global_vec;
    const vector<int>::size_type size = 71;

    if (rank == 0) {
        global_vec = random(size);
    }
    vector<Point> parallel = parallelGrahamMethod(global_vec, size);
    if (rank == 0) {
        vector<Point> not_parallel = GrahamMethod(global_vec);
        ASSERT_EQ(parallel.size(), not_parallel.size());
        for (vector<int>::size_type i = 0; i < parallel.size(); ++i) {
            ASSERT_EQ(parallel[i].x, not_parallel[i].x);
            ASSERT_EQ(parallel[i].y, not_parallel[i].y);
        }
    }
}

TEST(Parallel_Operations_MPI, GrahamParallel_Method_3) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<Point> global_vec;
    const vector<int>::size_type size = 123;

    if (rank == 0) {
        global_vec = random(size);
    }

    vector<Point> parallel = parallelGrahamMethod(global_vec, size);

    if (rank == 0) {
        vector<Point> not_parallel = GrahamMethod(global_vec);
        ASSERT_EQ(parallel.size(), not_parallel.size());
        for (vector<int>::size_type i = 0; i < parallel.size(); ++i) {
            ASSERT_EQ(parallel[i].x, not_parallel[i].x);
            ASSERT_EQ(parallel[i].y, not_parallel[i].y);
        }
    }
}

TEST(Parallel_Operations_MPI, GrahamParallel_Method_4) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<Point> global_vec;
    const vector<int>::size_type size = 500;

    if (rank == 0) {
        global_vec = random(size);
    }

    vector<Point> parallel = parallelGrahamMethod(global_vec, size);

    if (rank == 0) {
        vector<Point> not_parallel = GrahamMethod(global_vec);
        ASSERT_EQ(parallel.size(), not_parallel.size());
        for (vector<int>::size_type i = 0; i < parallel.size(); ++i) {
            ASSERT_EQ(parallel[i].x, not_parallel[i].x);
            ASSERT_EQ(parallel[i].y, not_parallel[i].y);
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
