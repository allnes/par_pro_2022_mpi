// Copyright 2022 Aleksandar Simeunovic
#include<gtest/gtest.h>
#include<mpi.h>
#include<vector>
#include<gtest-mpi-listener.hpp>
#include"../../../modules/task_1/simeunovic_a_column_sums/column_sums.h"
TEST(Column_Sums, Test_example1) {
    std::vector<int>a, b;
    DoWork(&a, &b);
    ASSERT_EQ(a, b);
}
TEST(Column_Sums, Test_example2) {
    std::vector<int>a, b;
    DoWork(&a, &b);
    ASSERT_EQ(a, b);
}
TEST(Column_Sums, Test_example3) {
    std::vector<int>a, b;
    DoWork(&a, &b);
    ASSERT_EQ(a, b);
}
TEST(Column_Sums, Test_example4) {
    std::vector<int>a, b;
    DoWork(&a, &b);
    ASSERT_EQ(a, b);
}
TEST(Column_Sums, Test_example5) {
    std::vector<int>a, b;
    DoWork(&a, &b);
    ASSERT_EQ(a, b);
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
