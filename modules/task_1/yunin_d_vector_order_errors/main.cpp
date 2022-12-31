// Copyright 2022 Yunin D.
#include <mpi.h>
#include <gtest/gtest.h>
#include <gtest-mpi-listener.hpp>
#include "./vector_order_errors.h"

TEST(count_number_errors_order_neighboring_elements_vector, test_ordered_vector) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> vec(50);
    if (rank == 0) {
        vec = CreateRandomVector(50, 20, 1);
        std::sort(vec.begin(), vec.end());
        // for (int i = 0; i < vec.size(); i++) {
        //     std::cout << vec[i] << ' ';
        // }
    }
    int  global_count = CountErrorsOrderNeigboringElementsVectorParallel(vec);
    if (rank == 0) {
        int global_count_s = CountErrorsOrderNeigboringElementsVector(vec);
        // std::cout << std::endl << global_count_s << " " << global_count << std::endl;
        ASSERT_EQ(global_count_s, global_count);
    }
}

TEST(count_number_errors_order_neighboring_elements_vector, test_random_vector) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> vec(35);
    if (rank == 0) {
        vec = CreateRandomVector(35, 100, 1);
        // for (int i = 0; i < vec.size(); i++) {
        //     std::cout << vec[i] << ' ';
        // }
    }
    int  global_count = CountErrorsOrderNeigboringElementsVectorParallel(vec);
    if (rank == 0) {
        int global_count_s = CountErrorsOrderNeigboringElementsVector(vec);
        // std::cout << std::endl << global_count_s << " " << global_count << std::endl;
        ASSERT_EQ(global_count_s, global_count);
    }
}

TEST(count_number_errors_order_neighboring_elements_vector, test_unordered_vector) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> vec(55);
    if (rank == 0) {
        vec = CreateRandomVector(55, 13, 2);
        std::sort(vec.begin(), vec.end(), std::greater<int>());
        // for (int i = 0; i < vec.size(); i++) {
        //     std::cout << vec[i] << ' ';
        // }
    }
    int  global_count = CountErrorsOrderNeigboringElementsVectorParallel(vec);
    if (rank == 0) {
        int global_count_s = CountErrorsOrderNeigboringElementsVector(vec);
        // std::cout << std::endl << global_count_s << " " << global_count << std::endl;
        ASSERT_EQ(global_count_s, global_count);
    }
}

TEST(count_number_errors_order_neighboring_elements_vector, test_vector_with_1_elem) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> vec(1);
    if (rank == 0) {
        vec = CreateRandomVector(1, 10, 1);
        // for (int i = 0; i < vec.size(); i++) {
        //     std::cout << vec[i] << ' ';
        // }
    }
    int  global_count = CountErrorsOrderNeigboringElementsVectorParallel(vec);
    if (rank == 0) {
        int global_count_s = CountErrorsOrderNeigboringElementsVector(vec);
        // std::cout << std::endl << global_count_s << " " << global_count << std::endl;
        ASSERT_EQ(global_count_s, global_count);
    }
}

TEST(count_number_errors_order_neighboring_elements_vector, test_vector_with_2_elems) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> vec(2);
    if (rank == 0) {
        vec = CreateRandomVector(2, 10, 1);
        // for (int i = 0; i < vec.size(); i++) {
        //     std::cout << vec[i] << ' ';
        // }
    }
    int  global_count = CountErrorsOrderNeigboringElementsVectorParallel(vec);
    if (rank == 0) {
        int global_count_s = CountErrorsOrderNeigboringElementsVector(vec);
        // std::cout << std::endl << global_count_s << " " << global_count << std::endl;
        ASSERT_EQ(global_count_s, global_count);
    }
}

TEST(count_number_errors_order_neighboring_elements_vector, test_large_vector) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> vec(1000000);
    if (rank == 0) {
        vec = CreateRandomVector(1000000, 1000, 3);
        // for (int i = 0; i < vec.size(); i++) {
        //     std::cout << vec[i] << ' ';
        // }
    }
    int  global_count = CountErrorsOrderNeigboringElementsVectorParallel(vec);
    if (rank == 0) {
        int global_count_s = CountErrorsOrderNeigboringElementsVector(vec);
        // std::cout << std::endl << global_count_s << " " << global_count << std::endl;
        ASSERT_EQ(global_count_s, global_count);
    }
}

TEST(count_number_errors_order_neighboring_elements_vector, test_random_vector_2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> vec(15);
    if (rank == 0) {
        vec = CreateRandomVector(15, 10, 1);
        // for (int i = 0; i < vec.size(); i++) {
        //     std::cout << vec[i] << ' ';
        // }
    }
    int  global_count = CountErrorsOrderNeigboringElementsVectorParallel(vec);
    if (rank == 0) {
        int global_count_s = CountErrorsOrderNeigboringElementsVector(vec);
        // std::cout << std::endl << global_count_s << " " << global_count << std::endl;
        ASSERT_EQ(global_count_s, global_count);
    }
}

TEST(count_number_errors_order_neighboring_elements_vector, test_random_vector_3) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> vec(120);
    if (rank == 0) {
        vec = CreateRandomVector(120, 10, 1);
        // for (int i = 0; i < vec.size(); i++) {
        //     std::cout << vec[i] << ' ';
        // }
    }
    int  global_count = CountErrorsOrderNeigboringElementsVectorParallel(vec);
    if (rank == 0) {
        int global_count_s = CountErrorsOrderNeigboringElementsVector(vec);
        // std::cout << std::endl << global_count_s << " " << global_count << std::endl;
        ASSERT_EQ(global_count_s, global_count);
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
