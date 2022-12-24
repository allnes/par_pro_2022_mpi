// Copyright 2022 Sigachev Anton
#include <gtest/gtest.h>
#include <algorithm>
#include "../../../modules/task_3/sigachev_a_radix_sort_d_simple_merge/radix_sort_d_simple_merge.h"
#include <gtest-mpi-listener.hpp>

TEST(Radix_Sort, Test1_Not_Parallel_Radix_Sort_Double) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    if (ProcRank == 0) {
        std::vector<double> v(50);
        v = Get_Random_Vector(50);

        std::vector<double> tmp = Not_Parallel_Radix_Sort(v);
        std::sort(v.begin(), v.end());
        ASSERT_EQ(tmp, v);
    }
}

TEST(Radix_Sort, Test2_Parallel_Radix_Sort_Double) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    std::vector<double> v(50);
    std::vector<double> tmp(50);

    if (ProcRank == 0) {
        tmp = Get_Random_Vector(50);
        v = tmp;
        std::sort(tmp.begin(), tmp.end());
    }

    std::vector<double> sort = Parallel_Radix_Sort(v);

    if (ProcRank == 0) {
        ASSERT_EQ(sort, tmp);
    }
}

TEST(Radix_Sort, Test3_Not_Parallel_VS_Parallel_With_Small_Vector) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    std::vector<double> v(50);
    std::vector<double> tmp(50);

    if (ProcRank == 0) {
        tmp = Get_Random_Vector(50);
        v = tmp;
        tmp = Not_Parallel_Radix_Sort(tmp);
    }

    std::vector<double> sort = Parallel_Radix_Sort(v);

    if (ProcRank == 0) {
        ASSERT_EQ(sort, tmp);
    }
}

TEST(Radix_Sort, Test4_Not_Parallel_VS_Parallel_With_Medium_Vector) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    double t1, t2, t3, t4;

    std::vector<double> v(70);
    std::vector<double> not_p_vec, p_vec;

    if (ProcRank == 0) {
        v = Get_Random_Vector(70);
        t1 = MPI_Wtime();
        not_p_vec = Not_Parallel_Radix_Sort(v);
        t2 = MPI_Wtime();
    }

    if (ProcRank == 0) {
        t3 = MPI_Wtime();
    }

    p_vec = Parallel_Radix_Sort(v);

    if (ProcRank == 0) {
        t4 = MPI_Wtime();
    }

    if (ProcRank == 0) {
        ASSERT_EQ(p_vec, not_p_vec);
        std::cout << "NotParallel " << t2 - t1 << std::endl;
        std::cout << "Parallel " << t4 - t3 << std::endl;
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
