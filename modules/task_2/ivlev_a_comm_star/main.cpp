// Copyright 2022 Ivlev A
#include <gtest/gtest.h>
#include <stdio.h>
#include <vector>
#include "./comm_star.h"
#include <gtest-mpi-listener.hpp>


TEST(Test_comm_star_MPI, Test_0) {
    int size, rank;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (size >= 4) {
        MPI_Comm star_comm;
        int index[4] = {0, 1, 2, 3};
        MPI_group_star_create(MPI_COMM_WORLD, 4,
            index, &star_comm);

        int star_rank, star_size;
        if (star_comm != MPI_COMM_NULL) {
            MPI_Comm_rank(star_comm, &star_rank);
            MPI_Comm_size(star_comm, &star_size);

            EXPECT_LT(star_rank, star_size);
            EXPECT_LT(rank, 4);
        } else {
            star_rank = -1;
            star_size = -1;
        }

        if (star_comm != MPI_COMM_NULL) {
            MPI_Comm_free(&star_comm);
        }
    }
}

TEST(Test_comm_star_MPI, Test_1) {
    int size, rank;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (size >= 4) {
        MPI_Comm star_comm;
        int index[4] = {0, 1, 2, 3};
        MPI_group_star_create(MPI_COMM_WORLD, 4,
            index, &star_comm);

        int star_rank, star_size;
        if (star_comm != MPI_COMM_NULL) {
            MPI_Comm_rank(star_comm, &star_rank);
            MPI_Comm_size(star_comm, &star_size);

            int a[3];

            if (star_rank == 3) {
                for (int i = 0; i < 3; i++) {
                    a[i] = i;
                }
            }

            Star_Send(a, 3, MPI_INT, 3, 1, 1, star_comm);

            if (star_rank == 1) {
                int b[3];
                MPI_Status status;
                MPI_Recv(b, 3, MPI_INT, MPI_ANY_SOURCE, 1, star_comm, &status);
                for (int i = 0; i < 3; i++) {
                    EXPECT_EQ(b[i], i);
                }
            }
        } else {
            star_rank = -1;
            star_size = -1;
        }

        if (star_comm != MPI_COMM_NULL) {
            MPI_Comm_free(&star_comm);
        }
    }
}

TEST(Test_comm_star_MPI, Test_2) {
    int size, rank;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (size >= 4) {
        MPI_Comm star_comm;
        int index[4] = {0, 1, 2, 3};
        MPI_group_star_create(MPI_COMM_WORLD, 4,
            index, &star_comm);

        int star_rank, star_size;
        if (star_comm != MPI_COMM_NULL) {
            MPI_Comm_rank(star_comm, &star_rank);
            MPI_Comm_size(star_comm, &star_size);

            int a[3];

            if (star_rank == 2) {
                for (int i = 0; i < 3; i++) {
                    a[i] = i;
                }
            }

            Star_Send(a, 3, MPI_INT, 2, 0, 1, star_comm);

            if (star_rank == 0) {
                int b[3];
                MPI_Status status;
                MPI_Recv(b, 3, MPI_INT, MPI_ANY_SOURCE, 1, star_comm, &status);
                for (int i = 0; i < 3; i++) {
                    EXPECT_EQ(b[i], i);
                }
            }
        } else {
            star_rank = -1;
            star_size = -1;
        }

        if (star_comm != MPI_COMM_NULL) {
            MPI_Comm_free(&star_comm);
        }
    }
}

TEST(Test_comm_star_MPI, Test_3) {
    int size, rank;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (size >= 4) {
        MPI_Comm star_comm;
        int index[4] = {0, 1, 2, 3};
        MPI_group_star_create(MPI_COMM_WORLD, 4,
            index, &star_comm);

        int star_rank, star_size;
        if (star_comm != MPI_COMM_NULL) {
            MPI_Comm_rank(star_comm, &star_rank);
            MPI_Comm_size(star_comm, &star_size);

            int a[10];

            if (star_rank == 2) {
                for (int i = 0; i < 10; i++) {
                    a[i] = i;
                }
            }

            Star_Send(a, 4, MPI_INT, 2, 1, 1, star_comm);

            if (star_rank == 1) {
                int b[4];
                MPI_Status status;
                MPI_Recv(b, 4, MPI_INT, MPI_ANY_SOURCE, 1, star_comm, &status);
                for (int i = 0; i < 4; i++) {
                    EXPECT_EQ(b[i], i);
                }
            }
        } else {
            star_rank = -1;
            star_size = -1;
        }

        if (star_comm != MPI_COMM_NULL) {
            MPI_Comm_free(&star_comm);
        }
    }
}

TEST(Test_comm_star_MPI, Test_4) {
    int size, rank;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (size >= 4) {
        MPI_Comm star_comm;
        int index[4] = {0, 1, 2, 3};
        MPI_group_star_create(MPI_COMM_WORLD, 4,
            index, &star_comm);

        int star_rank, star_size;
        if (star_comm != MPI_COMM_NULL) {
            MPI_Comm_rank(star_comm, &star_rank);
            MPI_Comm_size(star_comm, &star_size);

            int a[10];
            int b[10];

            if (star_rank == 2) {
                for (int i = 0; i < 10; i++) {
                    a[i] = i;
                }
            }

            Star_Send(a, 10, MPI_INT, 2, 1, 1, star_comm);

            if (star_rank == 1) {
                MPI_Status status;
                MPI_Recv(b, 10, MPI_INT, MPI_ANY_SOURCE, 1, star_comm, &status);
                for (int i = 0; i < 10; i++) {
                    EXPECT_EQ(b[i], i);
                }
            }

            Star_Send(b, 3, MPI_INT, 1, 0, 1, star_comm);

            if (star_rank == 0) {
                int c[3];
                MPI_Status status;
                MPI_Recv(c, 3, MPI_INT, MPI_ANY_SOURCE, 1, star_comm, &status);
                for (int i = 0; i < 3; i++) {
                    EXPECT_EQ(c[i], i);
                }
            }
        } else {
            star_rank = -1;
            star_size = -1;
        }

        if (star_comm != MPI_COMM_NULL) {
            MPI_Comm_free(&star_comm);
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
