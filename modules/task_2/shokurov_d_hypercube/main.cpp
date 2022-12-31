// Copyright 2022 Shokurov Daniil
#include <gtest/gtest.h>

#include "./hypercube.h"

#include <gtest-mpi-listener.hpp>

TEST(hypercube, test1) {
    int rank = 0;
    int ProcNum = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::string str = "Hello World!";
    int rank_in = 0;
    int rank_out = 0;
    if (ProcNum == 1) {
        rank_in = 0;
        rank_out = 0;
    } else if (ProcNum == 2) {
        rank_in = 1;
        rank_out = 0;
    } else if (ProcNum == 3) {
        rank_in = 0;
        rank_out = 2;
    } else if (ProcNum == 4) {
        rank_in = 1;
        rank_out = 3;
    } else if (ProcNum == 5) {
        rank_in = 1;
        rank_out = 4;
    } else if (ProcNum == 6) {
        rank_in = 2;
        rank_out = 5;
    } else {
        rank_in = 1;
        rank_out = 6;
    }
    char* ch = nullptr;
    int count;
    if (rank == rank_in) {
        count = str.size();
        ch = new char[count];
        for (int i = 0; i < count; ++i) {
            ch[i] = str[i];
        }
        send(rank_in, rank_out, &ch, &count);
    } else {
        send(-1, -1, &ch, &count);
    }
    if (rank == rank_out) {
        std::string str2(ch, count);
        EXPECT_EQ(str, str2);
    }
    if (ch != nullptr) {
        delete[] ch;
    }
}

TEST(hypercube, test2) {
    int rank = 0;
    int ProcNum = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::string str = "Hello World!";
    int rank_in = 0;
    int rank_out = 1;
    if (ProcNum == 1) {
        rank_in = 0;
        rank_out = 0;
    } else if (ProcNum == 2) {
        rank_in = 1;
        rank_out = 0;
    } else if (ProcNum == 3) {
        rank_in = 0;
        rank_out = 2;
    } else if (ProcNum == 4) {
        rank_in = 1;
        rank_out = 3;
    } else if (ProcNum == 5) {
        rank_in = 1;
        rank_out = 4;
    } else if (ProcNum == 6) {
        rank_in = 2;
        rank_out = 5;
    } else {
        rank_in = 1;
        rank_out = 6;
    }
    char* ch = nullptr;
    int count;
    if (rank == rank_in) {
        count = str.size();
        ch = new char[count];
        for (int i = 0; i < count; ++i) {
            ch[i] = str[i];
        }
        send(rank_in, rank_out, &ch, &count);
    } else {
        send(-1, -1, &ch, &count);
    }
    if (rank == rank_out) {
        std::string str2(ch, count);
        EXPECT_EQ(str, str2);
    }
    if (ch != nullptr) {
        delete[] ch;
    }
}

TEST(hypercube, test3) {
    int rank = 0;
    int ProcNum = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::string str = "Hello World!";
    int rank_in = 1;
    int rank_out = 0;
    if (ProcNum == 1) {
        rank_in = 0;
        rank_out = 0;
    } else if (ProcNum == 2) {
        rank_in = 1;
        rank_out = 0;
    } else if (ProcNum == 3) {
        rank_in = 0;
        rank_out = 2;
    } else if (ProcNum == 4) {
        rank_in = 1;
        rank_out = 3;
    } else if (ProcNum == 5) {
        rank_in = 1;
        rank_out = 4;
    } else if (ProcNum == 6) {
        rank_in = 2;
        rank_out = 5;
    } else {
        rank_in = 1;
        rank_out = 6;
    }
    char* ch = nullptr;
    int count;
    if (rank == rank_in) {
        count = str.size();
        ch = new char[count];
        for (int i = 0; i < count; ++i) {
            ch[i] = str[i];
        }
        send(rank_in, rank_out, &ch, &count);
    } else {
        send(-1, -1, &ch, &count);
    }
    if (rank == rank_out) {
        std::string str2(ch, count);
        EXPECT_EQ(str, str2);
    }
    if (ch != nullptr) {
        delete[] ch;
    }
}

TEST(hypercube, test4) {
    int rank = 0;
    int ProcNum = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::string str = "Hello World!";
    int rank_in = 1;
    int rank_out = 1;
    if (ProcNum == 1) {
        rank_in = 0;
        rank_out = 0;
    } else if (ProcNum == 2) {
        rank_in = 1;
        rank_out = 0;
    } else if (ProcNum == 3) {
        rank_in = 0;
        rank_out = 2;
    } else if (ProcNum == 4) {
        rank_in = 1;
        rank_out = 3;
    } else if (ProcNum == 5) {
        rank_in = 1;
        rank_out = 4;
    } else if (ProcNum == 6) {
        rank_in = 2;
        rank_out = 5;
    } else {
        rank_in = 1;
        rank_out = 6;
    }
    char* ch = nullptr;
    int count;
    if (rank == rank_in) {
        count = str.size();
        ch = new char[count];
        for (int i = 0; i < count; ++i) {
            ch[i] = str[i];
        }
        send(rank_in, rank_out, &ch, &count);
    } else {
        send(-1, -1, &ch, &count);
    }
    if (rank == rank_out) {
        std::string str2(ch, count);
        EXPECT_EQ(str, str2);
    }
    if (ch != nullptr) {
        delete[] ch;
    }
}

TEST(hypercube, test5) {
    int rank = 0;
    int ProcNum = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::string str = "Hello World!";
    int rank_in = 1;
    int rank_out = 2;
    if (ProcNum == 1) {
        rank_in = 0;
        rank_out = 0;
    } else if (ProcNum == 2) {
        rank_in = 1;
        rank_out = 0;
    } else if (ProcNum == 3) {
        rank_in = 0;
        rank_out = 2;
    } else if (ProcNum == 4) {
        rank_in = 1;
        rank_out = 3;
    } else if (ProcNum == 5) {
        rank_in = 1;
        rank_out = 4;
    } else if (ProcNum == 6) {
        rank_in = 2;
        rank_out = 5;
    } else {
        rank_in = 1;
        rank_out = 6;
    }
    char* ch = nullptr;
    int count;
    if (rank == rank_in) {
        count = str.size();
        ch = new char[count];
        for (int i = 0; i < count; ++i) {
            ch[i] = str[i];
        }
        send(rank_in, rank_out, &ch, &count);
    } else {
        send(-1, -1, &ch, &count);
    }
    if (rank == rank_out) {
        std::string str2(ch, count);
        EXPECT_EQ(str, str2);
    }
    if (ch != nullptr) {
        delete[] ch;
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}
