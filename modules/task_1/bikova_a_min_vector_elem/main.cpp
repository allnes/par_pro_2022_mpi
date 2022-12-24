// Copyright 2022 Alexandra Bikova
#include<gtest/gtest.h>
#include<mpi.h>
#include<iostream>
#include<algorithm>
#include<gtest-mpi-listener.hpp>
#include"../../../modules/task_1/bikova_a_min_vector_elem/min_vector_elem.h"
TEST(Min_elem_vec, Test_1) {
    int ProcRank, ProcSize, min_element;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcSize);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    std::vector<int> vec;
    if (ProcRank == 0) {
        CreateRandomVec(&vec);
    }
    Smallest_Element(ProcRank, ProcSize, &vec, &min_element);
    if (ProcRank == 0) {
        int std_min_element = *std::min_element(vec.begin(), vec.end());
        ASSERT_EQ(min_element, std_min_element);
    }
}
TEST(Min_elem_vec, Test_2) {
    int ProcRank, ProcSize, min_element;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcSize);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    std::vector<int> vec;
    if (ProcRank == 0) {
        CreateRandomVec(&vec);
    }
    Smallest_Element(ProcRank, ProcSize, &vec, &min_element);
    if (ProcRank == 0) {
        int std_min_element = *std::min_element(vec.begin(), vec.end());
        ASSERT_EQ(min_element, std_min_element);
    }
}
TEST(Min_elem_vec, Test_3) {
    int ProcRank, ProcSize, min_element;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcSize);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    std::vector<int> vec;
    if (ProcRank == 0) {
        CreateRandomVec(&vec);
    }
    Smallest_Element(ProcRank, ProcSize, &vec, &min_element);
    if (ProcRank == 0) {
        int std_min_element = *std::min_element(vec.begin(), vec.end());
        ASSERT_EQ(min_element, std_min_element);
    }
}
TEST(Min_elem_vec, Test_4) {
    int ProcRank, ProcSize, min_element;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcSize);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    std::vector<int> vec;
    if (ProcRank == 0) {
        CreateRandomVec(&vec);
    }
    Smallest_Element(ProcRank, ProcSize, &vec, &min_element);
    if (ProcRank == 0) {
        int std_min_element = *std::min_element(vec.begin(), vec.end());
        ASSERT_EQ(min_element, std_min_element);
    }
}
TEST(Min_elem_vec, Test_5) {
    int ProcRank, ProcSize, min_element;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcSize);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    std::vector<int> vec;
    if (ProcRank == 0) {
        CreateRandomVec(&vec);
    }
    Smallest_Element(ProcRank, ProcSize, &vec, &min_element);
    if (ProcRank == 0) {
        int std_min_element = *std::min_element(vec.begin(), vec.end());
        ASSERT_EQ(min_element, std_min_element);
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
