  // Copyright 2022 Prokofev Denis
#include <gtest/gtest.h>
#include "../../../modules/task_3/prokofev_d_radix_batcher_double/radix_batcher_double.h"
#include <gtest-mpi-listener.hpp>


TEST(RadixSortBatcher, test1_int_chet) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int s = 4;
    double check[4] = {2, 3, 0, 5};
    double res[4] = {0, 2, 3, 5};

    double ne[4];
    double *vec1 = new double[4];
    for (int i=0; i < s; i++) {
        vec1[i] = check[i];
    }

    RadixSortBatcher(&vec1, s);
        for (int i = 0; i < s; i++) {
        ne[i] = vec1[i];
    }

    if (rank == 0) {
        for (int i = 0; i < s; i++) {
            EXPECT_EQ(ne[i], res[i]);
        }
    }
}

TEST(RadixSortBatcher, test2_int_chet_with_neg) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int s = 4;
    double check[4] = {2, -3, 0, 5};
    double res[4] = {-3, 0, 2, 5};

    double ne[4];
    double *vec1 = new double[4];
    for (int i = 0; i < s; i++) {
        vec1[i] = check[i];
    }

    RadixSortBatcher(&vec1, s);
        for (int i = 0; i < s; i++) {
        ne[i] = vec1[i];
    }

    if (rank == 0) {
        for (int i = 0; i < s; i++) {
            EXPECT_EQ(ne[i], res[i]);
        }
    }
}

TEST(RadixSortBatcher, test3_double_chet) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int s = 4;
    double check[4] = {2.1, 3.014, 0.2, 5.7};
    double res[4] = {0.2, 2.1, 3.014, 5.7};

    double ne[4];
    double *vec1 = new double[4];
    for (int i = 0; i < s; i++) {
        vec1[i] = check[i];
    }

    RadixSortBatcher(&vec1, s);
        for (int i = 0; i < s; i++) {
        ne[i] = vec1[i];
    }

    if (rank == 0) {
        for (int i = 0; i < s; i++) {
            EXPECT_EQ(ne[i], res[i]);
        }
    }
}

TEST(RadixSortBatcher, test4_double_chet_with_neg) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int s = 4;
    double check[4] = {2.1, -3.24, 0.014, 5.6};
    double res[4] = {-3.24, 0.014, 2.1, 5.6};

    double ne[4];
    double *vec1 = new double[4];
    for (int i = 0; i < s; i++) {
        vec1[i] = check[i];
    }

    RadixSortBatcher(&vec1, s);
        for (int i = 0; i < s; i++) {
        ne[i] = vec1[i];
    }

    if (rank == 0) {
        for (int i = 0; i < s; i++) {
            EXPECT_EQ(ne[i], res[i]);
        }
    }
}

TEST(RadixSortBatcher, test5_double_nechet_with_neg) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int s = 3;
    double check[3] = {2.1, -3.24, -5.4};
    double res[3] = {-5.4, -3.24, 2.1};

    double ne[3];
    double *vec1 = new double[3];
    for (int i = 0; i < s; i++) {
        vec1[i] = check[i];
    }

    RadixSortBatcher(&vec1, s);
        for (int i = 0; i < s; i++) {
        ne[i] = vec1[i];
    }

    if (rank == 0) {
        for (int i = 0; i < s; i++) {
            EXPECT_EQ(ne[i], res[i]);
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
