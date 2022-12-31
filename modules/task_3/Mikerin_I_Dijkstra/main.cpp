// Copyright 2022 Mikerin Ilya
#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include "../../../modules/task_3/Mikerin_I_Dijkstra/Dijkstra.h"
#include <gtest-mpi-listener.hpp>



TEST(Parallel, concret) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int** vec2 = new int*[5];
    for ( int i =0; i < 5; i++ ) {
            vec2[i] = new int[5];
        }
    int* result;

    if (rank == 0) {
        const int vec[5][5] = {   {0, 10, -1, 30, 100},
                            {-1, 0, 50, -1, -1},
                            {-1, -1, 0, -1, 10},
                            {-1, -1, 20, 0, 60},
                            {-1, -1, -1, -1, 0}};

        for ( int i =0; i < 5; i++ ) {
            for ( int j = 0; j < 5; j++ ) {
                vec2[i][j] = vec[i][j];
            }
        }
    }

    result = ParallelDijkstra(vec2, 5, 0);

    if ( rank== 0 ) {
        int realresult[5] = {0, 10, 50, 30, 60 };

        for ( int i = 0; i < 5; i++ ) {
            ASSERT_EQ(result[i], realresult[i]);
        }
    }
}

TEST(Parallel, small) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int count = size * 2;

    int** vec2 = new int*[count];
    int* result;

    if (rank == 0) {
        for ( int i =0; i < count; i++ ) {
            vec2[i] = getRandomVector(count);
            vec2[i][i] = 0;
        }
    }

    result = ParallelDijkstra(vec2, count, 0);

    if ( rank== 0 ) {
        int* seqresult = SequentialDijkstra(vec2, count, 0);

        for ( int i = 0; i < count; i++ ) {
            ASSERT_EQ(result[i], seqresult[i]);
        }
    }
}

TEST(Parallel, fromSecond) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int count = size * 3;

    int** vec2 = new int*[count];

    int* result;

    if (rank == 0) {
        for ( int i =0; i < count; i++ ) {
            vec2[i] = getRandomVector(count);
            vec2[i][i] = 0;
        }
    }

    result = ParallelDijkstra(vec2, count, 1);

    if ( rank== 0 ) {
        int* seqresult = SequentialDijkstra(vec2, count, 1);

        for ( int i = 0; i < count; i++ ) {
            ASSERT_EQ(result[i], seqresult[i]);
        }
    }
}

TEST(Parallel, Big) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int count = size * 4 + 1;

    int** vec2 = new int*[count];

    int* result;

    if (rank == 0) {
        for ( int i =0; i < count; i++ ) {
            vec2[i] = getRandomVector(count);
            vec2[i][i] = 0;
        }
    }

    result = ParallelDijkstra(vec2, count, 0);

    if ( rank== 0 ) {
        int* seqresult = SequentialDijkstra(vec2, count, 0);

        for ( int i = 0; i < count; i++ ) {
            ASSERT_EQ(result[i], seqresult[i]);
        }
    }
}

TEST(Parallel, Medium) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int count = size * 3;

    int** vec2 = new int*[count];

    int* result;

    if (rank == 0) {
        for ( int i =0; i < count; i++ ) {
            vec2[i] = getRandomVector(count);
            vec2[i][i] = 0;
        }
    }

    result = ParallelDijkstra(vec2, count, 0);

    if ( rank== 0 ) {
        int* seqresult = SequentialDijkstra(vec2, count, 0);

        for ( int i = 0; i < count; i++ ) {
            ASSERT_EQ(result[i], seqresult[i]);
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
