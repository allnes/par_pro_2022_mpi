// Copyright 2022 Shipilov Artem
#include <gtest/gtest.h>
#include <vector>
#include "./jarvis.h"
#include <gtest-mpi-listener.hpp>

TEST(Parallel_Operations_MPI, Vertex_50) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    vector<Point2d> global_vec;
    const vector<int>::size_type size = 100;

    if (rank == 0) {
        global_vec = Random(size);
    }

    vector<Point2d> global_curve = ParallelJarvis(global_vec, size);

    if (rank == 0) {
        vector<Point2d> ref_curve = SequenJarvis(global_vec);

        ASSERT_EQ(global_curve.size(), ref_curve.size());
        for (vector<int>::size_type i = 0; i < global_curve.size(); ++i) {
            ASSERT_EQ(global_curve[i].x, ref_curve[i].x);
            ASSERT_EQ(global_curve[i].y, ref_curve[i].y);
        }
    }
}

TEST(Parallel_Operations_MPI, Vertex_100) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    vector<Point2d> global_vec;
    const vector<int>::size_type size = 150;

    if (rank == 0) {
        global_vec = Random(size);
    }

    vector<Point2d> global_curve = ParallelJarvis(global_vec, size);

    if (rank == 0) {
        vector<Point2d> ref_curve = SequenJarvis(global_vec);

        ASSERT_EQ(global_curve.size(), ref_curve.size());
        for (vector<int>::size_type i = 0; i < global_curve.size(); ++i) {
            ASSERT_EQ(global_curve[i].x, ref_curve[i].x);
            ASSERT_EQ(global_curve[i].y, ref_curve[i].y);
        }
    }
}

TEST(Parallel_Operations_MPI, Vertex_150) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    vector<Point2d> global_vec;
    const vector<int>::size_type size = 200;

    if (rank == 0) {
        global_vec = Random(size);
    }

    vector<Point2d> global_curve = ParallelJarvis(global_vec, size);

    if (rank == 0) {
        vector<Point2d> ref_curve = SequenJarvis(global_vec);

        ASSERT_EQ(global_curve.size(), ref_curve.size());
        for (vector<int>::size_type i = 0; i < global_curve.size(); ++i) {
            ASSERT_EQ(global_curve[i].x, ref_curve[i].x);
            ASSERT_EQ(global_curve[i].y, ref_curve[i].y);
        }
    }
}

TEST(Parallel_Operations_MPI, Vertex_200) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    vector<Point2d> global_vec;
    const vector<int>::size_type size = 250;

    if (rank == 0) {
        global_vec = Random(size);
    }

    vector<Point2d> global_curve = ParallelJarvis(global_vec, size);

    if (rank == 0) {
        vector<Point2d> ref_curve = SequenJarvis(global_vec);

        ASSERT_EQ(global_curve.size(), ref_curve.size());
        for (vector<int>::size_type i = 0; i < global_curve.size(); ++i) {
            ASSERT_EQ(global_curve[i].x, ref_curve[i].x);
            ASSERT_EQ(global_curve[i].y, ref_curve[i].y);
        }
    }
}

TEST(Parallel_Operations_MPI, Vertex_250) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    vector<Point2d> global_vec;
    const vector<int>::size_type size = 300;

    if (rank == 0) {
        global_vec = Random(size);
    }

    vector<Point2d> global_curve = ParallelJarvis(global_vec, size);

    if (rank == 0) {
        vector<Point2d> ref_curve = SequenJarvis(global_vec);

        ASSERT_EQ(global_curve.size(), ref_curve.size());
        for (vector<int>::size_type i = 0; i < global_curve.size(); ++i) {
            ASSERT_EQ(global_curve[i].x, ref_curve[i].x);
            ASSERT_EQ(global_curve[i].y, ref_curve[i].y);
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
