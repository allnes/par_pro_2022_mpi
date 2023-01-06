// Copyright 2022 Bushmakin Maxim
#include <gtest/gtest.h>
#include "../../../modules/task_3/bushmakin_m_mark_binary/mark_binary.h"
#include <gtest-mpi-listener.hpp>


void MarkTest(int w, int h) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        MarkImage img(w, h);
        img.Random();
        MarkImage res = MarkParal(img);
        ASSERT_TRUE(CheckMark(res));
    } else {
        MarkParal(MarkImage(1, 1));
    }
}

TEST(Sobel_Filter_Mpi, Test_1) { MarkTest(3, 5); }

TEST(Sobel_Filter_Mpi, Test_2) { MarkTest(10, 3); }

TEST(Sobel_Filter_Mpi, Test_3) { MarkTest(27, 13); }

TEST(Sobel_Filter_Mpi, Test_4) { MarkTest(50, 50); }

TEST(Sobel_Filter_Mpi, Test_5) { MarkTest(100, 100); }

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
