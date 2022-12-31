 /*Copyright 2022 MUKHIN VADIM*/
#include <gtest/gtest.h>
#include "./star_topology.h"
#include <gtest-mpi-listener.hpp>

TEST(star_topology_MPI, test1_can_create_star_is_star_topology) {
    ASSERT_NO_THROW(Star(MPI_COMM_WORLD));
}

TEST(star_topology_MPI, test2_check_star_is_star_topology) {
    EXPECT_TRUE(IsStar(Star(MPI_COMM_WORLD)));
}

TEST(star_topology_MPI, test3_delete_star_is_star_topology) {
    MPI_Comm star = Star(MPI_COMM_WORLD);
    EXPECT_TRUE(IsStar(star));
    MPI_Comm_free(&star);
}

TEST(star_topology_MPI, test4_check_MPI_COMM_WORLD_is_star_topology) {
    EXPECT_FALSE(IsStar(MPI_COMM_WORLD));
}

TEST(star_topology_MPI, test5_return_true_with_star_topology) {
    int ProcRank, ProcNum;
    MPI_Comm star;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    int* index = new int[ProcNum];
    int* edges = new int[(ProcNum - 1) * 2];

    index[0] = ProcNum - 1;
    for (int i = 1; i < ProcNum; i++) {
        index[i] = index[i - 1] + 1;
    }

    for (int i = 0; i < (ProcNum - 1) * 2; i++) {
        if (i < ProcNum - 1) {
            edges[i] = i + 1;
        } else {
            edges[i] = 0;
        }
    }

    MPI_Graph_create(MPI_COMM_WORLD, ProcNum, index, edges, false, &star);

    EXPECT_TRUE(IsStar(star));
}

TEST(star_topology_MPI, test6_return_false_with_not_star_topology) {
    int ProcRank, ProcNum;
    MPI_Comm star;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    int* index = new int[ProcNum];
    int* edges = new int[(ProcNum - 1) * 2];
    for (int i = 0; i < ProcNum; i++) {
        index[i] = i + 1;
        edges[i] = i + 1;
    }

    MPI_Graph_create(MPI_COMM_WORLD, ProcNum, index, edges, false, &star);

    EXPECT_FALSE(IsStar(star));
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
