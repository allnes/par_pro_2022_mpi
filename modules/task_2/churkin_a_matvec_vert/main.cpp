// Copyright 2022 Churkin Alexander
#include <gtest/gtest.h>
#include <vector>
#include "./matvec_vert.h"
#include <gtest-mpi-listener.hpp>

void runTest(int m_glob, int n_glob, bool printValues = true) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0 && printValues) {
        std::cout << '\n';
        std::cout << "Test " << m_glob << " x " << n_glob << ":\n\n";
    }

    std::vector<int> globalMatrix;
    std::vector<int> globalVector;

    double t1Par, t2Par, t1Seq, t2Seq;

    if (rank == 0) {
        globalMatrix = getRandomMatrix(m_glob, n_glob);
        globalVector = getRandomVector(n_glob);

        if (printValues) {
            std::cout << "Matrix:\n";
            printMatrix(globalMatrix, m_glob, n_glob);
            std::cout << '\n';
            std::cout << "Vector:\n";
            printVector(globalVector);
            std::cout << "\n\n";
        }

        t1Par = MPI_Wtime();
    }

    std::vector<int> resPar =
        getMultParallel(globalMatrix, globalVector, m_glob, n_glob);

    if (rank == 0) {
        t2Par = MPI_Wtime();
        t1Seq = t2Par;
        std::vector<int> resSeq =
            getMultSequential(globalMatrix, globalVector, m_glob, n_glob);
        t2Seq = MPI_Wtime();

        if (printValues) {
            std::cout << "Result (parallel):\n";
            printVector(resPar);
        }

        ASSERT_EQ(resPar, resSeq);

        if (printValues) {
            std::cout << '\n';
            std::cout << "Sequential execution time: " << (t2Seq - t1Seq)
                      << '\n';
            std::cout << "Parralel execution time: " << (t2Par - t1Par)
                      << "\n\n";
        }
    }
}

/* 2 x 3 matrix */
TEST(Matrix_On_Vector_Mult_Vert_MPI, Test_1) { runTest(2, 3, false); }

/* 3 x 2 matrix */
TEST(Matrix_On_Vector_Mult_Vert_MPI, Test_2) { runTest(3, 2, false); }

/* 3 x 3 matrix */
TEST(Matrix_On_Vector_Mult_Vert_MPI, Test_3) { runTest(3, 3, false); }

/* 4 x 4 matrix */
TEST(Matrix_On_Vector_Mult_Vert_MPI, Test_4) { runTest(4, 4, false); }

/* 10 x 10 matrix */
TEST(Matrix_On_Vector_Mult_Vert_MPI, Test_5) { runTest(10, 10, false); }

/* 25 x 25 matrix */
TEST(Matrix_On_Vector_Mult_Vert_MPI, Test_6) { runTest(25, 25, false); }

/* 100 x 100 matrix */
TEST(Matrix_On_Vector_Mult_Vert_MPI, Test_7) { runTest(100, 100, false); }

/* 10000 x 10000 matrix */
/*TEST(Matrix_On_Vector_Mult_Vert_MPI, Test_8) {
    runTest(10000, 10000, false);
}*/

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
