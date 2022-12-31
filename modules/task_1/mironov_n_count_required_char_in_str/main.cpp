  // Copyright 2022 Mironov Nikita
#include <gtest/gtest.h>
#include "../../../modules/task_1/mironov_n_count_required_char_in_str/count_required_char_in_str.h"
#include <gtest-mpi-listener.hpp>

TEST(Count_required_char_in_str, test_1) {
    int procId;
    MPI_Comm_rank(MPI_COMM_WORLD, &procId);

    std::string str = "dadsadaababas";
    char requiredChar = 'a';

    int ppCount = countRequiredCharInStr(str, requiredChar);

    if (procId == 0) {
        int oneProcCount = countRequiredCharInProcStr(str.c_str(), requiredChar);
        ASSERT_EQ(oneProcCount, ppCount);
    }
}

TEST(Count_required_char_in_str, test_2) {
    int procId;
    MPI_Comm_rank(MPI_COMM_WORLD, &procId);

    std::string str = "deaaxabas";
    char requiredChar = 'z';

    int ppCount = countRequiredCharInStr(str, requiredChar);

    if (procId == 0) {
        int oneProcCount = countRequiredCharInProcStr(str.c_str(), requiredChar);
        ASSERT_EQ(oneProcCount, ppCount);
    }
}

TEST(Count_required_char_in_str, test_3) {
    int procId;
    MPI_Comm_rank(MPI_COMM_WORLD, &procId);

    std::string str = "fffffffffff";
    char requiredChar = 'f';

    int ppCount = countRequiredCharInStr(str, requiredChar);

    if (procId == 0) {
        int oneProcCount = countRequiredCharInProcStr(str.c_str(), requiredChar);
        ASSERT_EQ(oneProcCount, ppCount);
    }
}

TEST(Count_required_char_in_str, test_4) {
    int procId;
    MPI_Comm_rank(MPI_COMM_WORLD, &procId);

    std::string str = "";
    char requiredChar = 'd';

    ASSERT_ANY_THROW(countRequiredCharInStr(str, requiredChar));
}

TEST(Count_required_char_in_str, test_5) {
    int size = -5;
    char *str = new char[5];
    ASSERT_ANY_THROW(getRandomStr(str, size));
}

TEST(Count_required_char_in_str, test_6) {
    int procId;
    MPI_Comm_rank(MPI_COMM_WORLD, &procId);

    std::string str = "     ";
    char requiredChar = ' ';


    int ppCount = countRequiredCharInStr(str, requiredChar);

    if (procId == 0) {
        int oneProcCount = countRequiredCharInProcStr(str.c_str(), requiredChar);
        ASSERT_EQ(oneProcCount, ppCount);
    }
}

int main(int argc, char **argv) {
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
