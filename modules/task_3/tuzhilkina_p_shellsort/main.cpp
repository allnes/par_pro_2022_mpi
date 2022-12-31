// Copyright 2022 Tuzhilkina Polina
#include <gtest/gtest.h>
#include <vector>
#include "../../../modules/task_3/tuzhilkina_p_shellsort/shellsort.h"
#include <gtest-mpi-listener.hpp>

void runShellTest(int glob_vec_size, bool printingVecs = true,
                  bool printing = true) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> globVec;
    std::vector<int> vecPar;
    std::vector<int> vecSeq;

    double t1s, t2s, t1p, t2p;

    if (rank == 0) {
        globVec = getUnsortedVec(glob_vec_size);
        vecPar = globVec;
        vecSeq = globVec;

        if (printingVecs) {
            std::cout << "Original vector:\n";
            printVecElements(globVec);
            std::cout << "\n\n";
        }

        t1p = MPI_Wtime();
    }

    shellsortPar(&vecPar, glob_vec_size);

    if (rank == 0) {
        t2p = t1s = MPI_Wtime();
        shellsortSeq(&vecSeq);
        t2s = MPI_Wtime();

        EXPECT_EQ(vecPar, vecSeq);

        if (printingVecs) {
            std::cout << "Sequential result:\n";
            printVecElements(vecSeq);
            std::cout << "\n\n";
            std::cout << "Parallel result:\n";
            printVecElements(vecPar);
        }

        if (printing)
            std::cout << "\n\nTime saved: " << (t2s - t1s) - (t2p - t1p)
                      << '\n';
    }
}

TEST(Shellsort_MPI, Shellsort_1) { runShellTest(2, false, false); }

TEST(Shellsort_MPI, Shellsort_2) { runShellTest(5, false, false); }

TEST(Shellsort_MPI, Shellsort_3) { runShellTest(10, false, false); }

TEST(Shellsort_MPI, Shellsort_4) { runShellTest(25, false, false); }

TEST(Shellsort_MPI, Shellsort_5) { runShellTest(100, false, false); }

TEST(Shellsort_MPI, Shellsort_7) { runShellTest(10000, false, false); }

/*
TEST(Shellsort_MPI, Test_Shellsort_8) { runShellTest(10000000, false); }
*/

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
