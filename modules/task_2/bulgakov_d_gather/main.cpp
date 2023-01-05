// Copyright 2022 Bulgakov Daniil

#include <gtest/gtest.h>
#include <string>
#include <random>
#include <iostream>
#include "./gather_mpi.h"
#include <gtest-mpi-listener.hpp>

// #define debug
// #define time
#define test1
#define test2
#define test3
#define test4
#define test5


#ifdef test1
TEST(Parallel_Operations_MPI, Test_Int) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    const int packCnt = 1000;
    int * global_buff_my = nullptr;
    int * global_buff_lib = nullptr;
    double s1, e1, s2, e2;
    int * local_buff = new int[packCnt];

    std::random_device dev;
    std::mt19937 gen(dev());

    int rankt = (size - (size / 2)) > size - 1 ? size - 1 : (size - (size / 2));

    for (int  i = 0; i < packCnt; i++) { local_buff[i] = gen() % 100; }

    if (rank == rankt) {
        global_buff_my = new int[size * packCnt];
        global_buff_lib = new int[size * packCnt];
    }

    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == rankt) {
        s1 = MPI_Wtime();
        MPI_Gather(local_buff, packCnt, MPI_INT, global_buff_lib, packCnt, MPI_INT, rankt, MPI_COMM_WORLD);
        e1 = MPI_Wtime();
    } else {
        MPI_Gather(local_buff, packCnt, MPI_INT, global_buff_lib, packCnt, MPI_INT, rankt, MPI_COMM_WORLD);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == rankt) {
        s2 = MPI_Wtime();
        MPI_Own_Gather(local_buff, packCnt, MPI_INT, global_buff_my, packCnt, MPI_INT, rankt, MPI_COMM_WORLD);
        e2 = MPI_Wtime();
    } else {
        MPI_Own_Gather(local_buff, packCnt, MPI_INT, global_buff_my, packCnt, MPI_INT, rankt, MPI_COMM_WORLD);
    }

    if (rank == rankt) {
        #ifdef time
        std::cout << std::endl << "----------------" << std::endl;
        std::cout << "Gather Lib Time = " << e1 - s1 << std::endl;
        std::cout << "Gather Own Time = " << e2 - s2 << std::endl;
        std::cout << std::endl << "----------------" << std::endl;
        #endif
        #ifdef debug
        for (int i = 0; i < size * packCnt; i++) {
            std::cout << "{" << global_buff_lib[i] << "," << global_buff_my[i] << "}";
        }
        #endif
        for (int i = 0; i < size * packCnt; i++) {
            ASSERT_EQ(global_buff_lib[i], global_buff_my[i]);
        }
        // ASSERT_LT((e2 - s2), (e1 - s1) * 30);
        delete[] global_buff_my;
        delete[] global_buff_lib;
    }

    delete[] local_buff;
}
#endif

#ifdef test2
TEST(Parallel_Operations_MPI, Test_Float) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    const int packCnt = 1000;
    float * global_buff_my = nullptr;
    float * global_buff_lib = nullptr;
    double s1, e1, s2, e2;
    float * local_buff = new float[packCnt];


    std::random_device dev;
    std::mt19937 gen(dev());

    int rankt = size - 1;

    for (int  i = 0; i < packCnt; i++) { local_buff[i] = ((gen() % 100) / static_cast<float>(gen() % 30 + 1)); }

    if (rank == rankt) {
        global_buff_my = new float[size * packCnt];
        global_buff_lib = new float[size * packCnt];
    }

    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == rankt) {
        s1 = MPI_Wtime();
        MPI_Gather(local_buff, packCnt, MPI_FLOAT, global_buff_lib, packCnt, MPI_FLOAT, rankt, MPI_COMM_WORLD);
        e1 = MPI_Wtime();
    } else {
        MPI_Gather(local_buff, packCnt, MPI_FLOAT, global_buff_lib, packCnt, MPI_FLOAT, rankt, MPI_COMM_WORLD);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == rankt) {
        s2 = MPI_Wtime();
        MPI_Own_Gather(local_buff, packCnt, MPI_FLOAT, global_buff_my, packCnt, MPI_FLOAT, rankt, MPI_COMM_WORLD);
        e2 = MPI_Wtime();
    } else {
        MPI_Own_Gather(local_buff, packCnt, MPI_FLOAT, global_buff_my, packCnt, MPI_FLOAT, rankt, MPI_COMM_WORLD);
    }

    if (rank == rankt) {
        #ifdef time
        std::cout << std::endl << "----------------" << std::endl;
        std::cout << "Gather Lib Time = " << e1 - s1 << std::endl;
        std::cout << "Gather Own Time = " << e2 - s2 << std::endl;
        std::cout << std::endl << "----------------" << std::endl;
        #endif
        #ifdef debug
        for (int i = 0; i < size * packCnt; i++) {
            std::cout << "{" << global_buff_lib[i] << "," << global_buff_my[i] << "}";
        }
        #endif
        for (int i = 0; i < size * packCnt; i++) {
            ASSERT_EQ(global_buff_lib[i], global_buff_my[i]);
        }
        // ASSERT_LT((e2 - s2), (e1 - s1) * 30);
        delete[] global_buff_my;
        delete[] global_buff_lib;
    }

    delete[] local_buff;
}
#endif


#ifdef test3
TEST(Parallel_Operations_MPI, Test_Double) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    const int packCnt = 1000;
    double * global_buff_my = nullptr;
    double * global_buff_lib = nullptr;
    double s1, e1, s2, e2;
    double * local_buff = new double[packCnt];

    int rankt = 3 > size - 1 ? size - 1 : 3;

    std::random_device dev;
    std::mt19937 gen(dev());
    for (int  i = 0; i < packCnt; i++) { local_buff[i] = ((gen() % 100) / static_cast<double>(gen() % 30 + 1)); }

    if (rank == rankt) {
        global_buff_my = new double[size * packCnt];
        global_buff_lib = new double[size * packCnt];
    }

    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == rankt) {
        s1 = MPI_Wtime();
        MPI_Gather(local_buff, packCnt, MPI_DOUBLE, global_buff_lib, packCnt, MPI_DOUBLE, rankt, MPI_COMM_WORLD);
        e1 = MPI_Wtime();
    } else {
        MPI_Gather(local_buff, packCnt, MPI_DOUBLE, global_buff_lib, packCnt, MPI_DOUBLE, rankt, MPI_COMM_WORLD);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == rankt) {
        s2 = MPI_Wtime();
        MPI_Own_Gather(local_buff, packCnt, MPI_DOUBLE, global_buff_my, packCnt, MPI_DOUBLE, rankt, MPI_COMM_WORLD);
        e2 = MPI_Wtime();
    } else {
        MPI_Own_Gather(local_buff, packCnt, MPI_DOUBLE, global_buff_my, packCnt, MPI_DOUBLE, rankt, MPI_COMM_WORLD);
    }

    if (rank == rankt) {
        #ifdef time
        std::cout << std::endl << "----------------" << std::endl;
        std::cout << "Gather Lib Time = " << e1 - s1 << std::endl;
        std::cout << "Gather Own Time = " << e2 - s2 << std::endl;
        std::cout << std::endl << "----------------" << std::endl;
        #endif
        #ifdef debug
        for (int i = 0; i < size * packCnt; i++) {
            std::cout << "{" << global_buff_lib[i] << "," << global_buff_my[i] << "}";
        }
        #endif
        for (int i = 0; i < size * packCnt; i++) {
            ASSERT_EQ(global_buff_lib[i], global_buff_my[i]);
        }
        // ASSERT_LT((e2 - s2), (e1 - s1) * 30);
        delete[] global_buff_my;
        delete[] global_buff_lib;
    }

    delete[] local_buff;
}
#endif

#ifdef test4
TEST(Parallel_Operations_MPI, Test_Communicator) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    const int packCnt = 1000;
    float * global_buff_my = nullptr;
    float * global_buff_lib = nullptr;
    double s1, e1, s2, e2;
    float * local_buff = new float[packCnt];

    int n = size / 3;
    int * ranks = new int[n];
    for (int i = 0; i < n; i++) {
        ranks[i] = i;
    }

    MPI_Group world_group;
    MPI_Comm_group(MPI_COMM_WORLD, &world_group);

    MPI_Group new_group;
    MPI_Group_incl(world_group, size / 3, ranks, &new_group);

    MPI_Comm new_comm;
    MPI_Comm_create(MPI_COMM_WORLD, new_group, &new_comm);

    if (MPI_COMM_NULL != new_comm) {
        MPI_Comm_rank(new_comm, &rank);
        MPI_Comm_size(new_comm, &size);

        int rankt = size / 3;

        std::random_device dev;
        std::mt19937 gen(dev());
        for (int  i = 0; i < packCnt; i++) { local_buff[i] = ((gen() % 100) / static_cast<float>(gen() % 30 + 1)); }

        if (rank == rankt) {
            global_buff_my = new float[size * packCnt];
            global_buff_lib = new float[size * packCnt];
        }

        MPI_Barrier(new_comm);
        if (rank == rankt) {
            s1 = MPI_Wtime();
            MPI_Gather(local_buff, packCnt, MPI_FLOAT, global_buff_lib, packCnt, MPI_FLOAT, rankt, new_comm);
            e1 = MPI_Wtime();
        } else {
            MPI_Gather(local_buff, packCnt, MPI_FLOAT, global_buff_lib, packCnt, MPI_FLOAT, rankt, new_comm);
        }

        MPI_Barrier(new_comm);
        if (rank == rankt) {
            s2 = MPI_Wtime();
            MPI_Own_Gather(local_buff, packCnt, MPI_FLOAT, global_buff_my, packCnt, MPI_FLOAT, rankt, new_comm);
            e2 = MPI_Wtime();
        } else {
            MPI_Own_Gather(local_buff, packCnt, MPI_FLOAT, global_buff_my, packCnt, MPI_FLOAT, rankt, new_comm);
        }

        if (rank == rankt) {
            #ifdef time
            std::cout << std::endl << "----------------" << std::endl;
            std::cout << "Gather Lib Time = " << e1 - s1 << std::endl;
            std::cout << "Gather Own Time = " << e2 - s2 << std::endl;
            std::cout << std::endl << "----------------" << std::endl;
            #endif
            #ifdef debug
            for (int i = 0; i < size * packCnt; i++) {
                std::cout << "{" << global_buff_lib[i] << "," << global_buff_my[i] << "}";
            }
            #endif
            for (int i = 0; i < size * packCnt; i++) {
                ASSERT_EQ(global_buff_lib[i], global_buff_my[i]);
            }

            // ASSERT_LT((e2 - s2), (e1 - s1) * 30);
            delete[] global_buff_my;
            delete[] global_buff_lib;
        }

        delete[] local_buff;
    }
}
#endif

#ifdef test5
TEST(Parallel_Operations_MPI, Test_Rank_0) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    const int packCnt = 1000;
    double * global_buff_my = nullptr;
    double * global_buff_lib = nullptr;
    double s1, e1, s2, e2;
    double * local_buff = new double[packCnt];

    int rankt = 0;

    std::random_device dev;
    std::mt19937 gen(dev());
    for (int  i = 0; i < packCnt; i++) { local_buff[i] = ((gen() % 100) / static_cast<double>(gen() % 30 + 1)); }

    if (rank == rankt) {
        global_buff_my = new double[size * packCnt];
        global_buff_lib = new double[size * packCnt];
    }

    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == rankt) {
        s1 = MPI_Wtime();
        MPI_Gather(local_buff, packCnt, MPI_DOUBLE, global_buff_lib, packCnt, MPI_DOUBLE, rankt, MPI_COMM_WORLD);
        e1 = MPI_Wtime();
    } else {
        MPI_Gather(local_buff, packCnt, MPI_DOUBLE, global_buff_lib, packCnt, MPI_DOUBLE, rankt, MPI_COMM_WORLD);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == rankt) {
        s2 = MPI_Wtime();
        MPI_Own_Gather(local_buff, packCnt, MPI_DOUBLE, global_buff_my, packCnt, MPI_DOUBLE, rankt, MPI_COMM_WORLD);
        e2 = MPI_Wtime();
    } else {
        MPI_Own_Gather(local_buff, packCnt, MPI_DOUBLE, global_buff_my, packCnt, MPI_DOUBLE, rankt, MPI_COMM_WORLD);
    }

    if (rank == rankt) {
        #ifdef time
        std::cout << std::endl << "----------------" << std::endl;
        std::cout << "Gather Lib Time = " << e1 - s1 << std::endl;
        std::cout << "Gather Own Time = " << e2 - s2 << std::endl;
        std::cout << std::endl << "----------------" << std::endl;
        #endif
        #ifdef debug
        for (int i = 0; i < size * packCnt; i++) {
            std::cout << "{" << global_buff_lib[i] << "," << global_buff_my[i] << "}";
        }
        #endif
        for (int i = 0; i < size * packCnt; i++) {
            ASSERT_EQ(global_buff_lib[i], global_buff_my[i]);
        }
        // ASSERT_LT((e2 - s2), (e1 - s1) * 30);
        delete[] global_buff_my;
        delete[] global_buff_lib;
    }

    delete[] local_buff;
}
#endif


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
