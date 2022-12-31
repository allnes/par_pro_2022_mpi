// Copyright 2022 Pronina Tatiana
#include <gtest/gtest.h>
#include <vector>
#include <random>
#include "./gather.h"
#include <gtest-mpi-listener.hpp>

TEST(Parallel_Operations_MPI, Test_Gather_INT) {
    int my_rank, tasks, root;
    double time1, time2;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &tasks);
    int range = 20 / tasks;
    std::vector<int> input_vec(20);
    std::vector<int> res1(20);
    std::vector<int> res2(20);
    std::vector<int> local(range);

    if (my_rank == 0) {
        input_vec = getRandomVector<int>(20);
        std::random_device dev;
        std::mt19937 gen(dev());
        root = gen() % tasks;
        /*  for (int i = 0; i < 20; i++) {
            std::cout << input_vec[i] << " ";
        }  */
        std::cout << std::endl;
    }
    MPI_Bcast(&root, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(input_vec.data(), range, MPI_INT, local.data(), range, MPI_INT, 0, MPI_COMM_WORLD);


    if (my_rank == root) time1 = MPI_Wtime();

    Gather(local.data(), range, MPI_INT, res1.data(), range, MPI_INT, root, MPI_COMM_WORLD);

    if (my_rank == root) {
        time1 = MPI_Wtime() - time1;
        time2 = MPI_Wtime();
    }

    MPI_Gather(local.data(), range, MPI_INT, res2.data(), range, MPI_INT, root, MPI_COMM_WORLD);

    if (my_rank == root) {
        time2 = MPI_Wtime() - time2;
        for (int i = 0; i < 20; i++) {
            if (res1[i] != res2[i]) {
                // std::cout << "Not a match!" << std::endl;
                ASSERT_EQ(res1[i], res2[i]);
            }
        }
        ASSERT_LT(abs(time2 - time1), 1);
    }
}

TEST(Parallel_Operations_MPI, Test_Gather_FLOAT) {
    int my_rank, tasks, root;
    double time1, time2;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &tasks);
    int range = 20 / tasks;
    std::vector<float> input_vec(20);
    std::vector<float> res1(20);
    std::vector<float> res2(20);
    std::vector<float> local(range);

    if (my_rank == 0) {
        input_vec = getRandomVector<float>(20);
        std::random_device dev;
        std::mt19937 gen(dev());
        root = gen() % tasks;
        /*  for (int i = 0; i < 20; i++) {
            std::cout << input_vec[i] << " ";
        }  */
        std::cout << std::endl;
    }
    MPI_Bcast(&root, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(input_vec.data(), range, MPI_FLOAT, local.data(), range, MPI_FLOAT, 0, MPI_COMM_WORLD);


    if (my_rank == root) time1 = MPI_Wtime();

    Gather(local.data(), range, MPI_FLOAT, res1.data(), range, MPI_FLOAT, root, MPI_COMM_WORLD);

    if (my_rank == root) {
        time1 = MPI_Wtime() - time1;
        time2 = MPI_Wtime();
    }

    MPI_Gather(local.data(), range, MPI_FLOAT, res2.data(), range, MPI_FLOAT, root, MPI_COMM_WORLD);

    if (my_rank == root) {
        time2 = MPI_Wtime() - time2;
        for (int i = 0; i < 20; i++) {
            if (res1[i] != res2[i]) {
                // std::cout << "Not a match!" << std::endl;
                ASSERT_EQ(res1[i], res2[i]);
            }
        }
        ASSERT_LT(abs(time2 - time1), 1);
    }
}

TEST(Parallel_Operations_MPI, Test_Gather_DOUBLE) {
    int my_rank, tasks, root;
    double time1, time2;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &tasks);
    int range = 20 / tasks;
    std::vector<double> input_vec(20);
    std::vector<double> res1(20);
    std::vector<double> res2(20);
    std::vector<double> local(range);

    if (my_rank == 0) {
        input_vec = getRandomVector<double>(20);
        std::random_device dev;
        std::mt19937 gen(dev());
        root = gen() % tasks;
        /*  for (int i = 0; i < 20; i++) {
            std::cout << input_vec[i] << " ";
        }  */
        std::cout << std::endl;
    }
    MPI_Bcast(&root, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(input_vec.data(), range, MPI_DOUBLE, local.data(), range, MPI_DOUBLE, 0, MPI_COMM_WORLD);


    if (my_rank == root) time1 = MPI_Wtime();

    Gather(local.data(), range, MPI_DOUBLE, res1.data(), range, MPI_DOUBLE, root, MPI_COMM_WORLD);

    if (my_rank == root) {
        time1 = MPI_Wtime() - time1;
        time2 = MPI_Wtime();
    }

    MPI_Gather(local.data(), range, MPI_DOUBLE, res2.data(), range, MPI_DOUBLE, root, MPI_COMM_WORLD);

    if (my_rank == root) {
        time2 = MPI_Wtime() - time2;
        for (int i = 0; i < 20; i++) {
            if (res1[i] != res2[i]) {
                // std::cout << "Not a match!" << std::endl;
                ASSERT_EQ(res1[i], res2[i]);
            }
        }
        ASSERT_LT(abs(time2 - time1), 1);
    }
}

TEST(Parallel_Operations_MPI, Test_Gather_CHAR) {
    int my_rank, tasks, root;
    double time1, time2;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &tasks);
    int range = 20 / tasks;
    std::vector<char> input_vec(20);
    std::vector<char> res1(20);
    std::vector<char> res2(20);
    std::vector<char> local(range);

    if (my_rank == 0) {
        input_vec = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k',
            'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't'};
        std::random_device dev;
        std::mt19937 gen(dev());
        root = gen() % tasks;
        /*  for (int i = 0; i < 20; i++) {
            std::cout << input_vec[i] << " ";
        }  */
        std::cout << std::endl;
    }
    MPI_Bcast(&root, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(input_vec.data(), range, MPI_CHAR, local.data(), range, MPI_CHAR, 0, MPI_COMM_WORLD);


    if (my_rank == root) time1 = MPI_Wtime();

    Gather(local.data(), range, MPI_CHAR, res1.data(), range, MPI_CHAR, root, MPI_COMM_WORLD);

    if (my_rank == root) {
        time1 = MPI_Wtime() - time1;
        time2 = MPI_Wtime();
    }

    MPI_Gather(local.data(), range, MPI_CHAR, res2.data(), range, MPI_CHAR, root, MPI_COMM_WORLD);

    if (my_rank == root) {
        time2 = MPI_Wtime() - time2;
        for (int i = 0; i < 20; i++) {
            if (res1[i] != res2[i]) {
                // std::cout << "Not a match!" << std::endl;
                ASSERT_EQ(res1[i], res2[i]);
            }
        }
        ASSERT_LT(abs(time2 - time1), 1);
    }
}

TEST(Parallel_Operations_MPI, Big_Data_Test) {
    int my_rank, tasks, root;
    double time1, time2;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &tasks);
    int datasize = 100;
    std::vector<int> vec(tasks * datasize);
    std::vector<int> res1(tasks * datasize);
    std::vector<int> res2(tasks * datasize);
    std::vector<int> local(datasize);

    if (my_rank == 0) {
        vec = getRandomVector<int>(tasks * datasize);
        std::random_device dev;
        std::mt19937 gen(dev());
        root = gen() % tasks;
        /*  for (int i = 0; i < datasize * tasks; i++) {
            std::cout << vec[i] << " ";
        }  */
        std::cout << std::endl;
    }
    MPI_Bcast(&root, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(vec.data(), datasize, MPI_INT, local.data(), datasize, MPI_INT, 0, MPI_COMM_WORLD);

    if (my_rank == root) time1 = MPI_Wtime();

    Gather(local.data(), datasize, MPI_INT, res1.data(), datasize, MPI_INT, root, MPI_COMM_WORLD);

    if (my_rank == root) {
        time1 = MPI_Wtime() - time1;
        time2 = MPI_Wtime();
    }

    MPI_Gather(local.data(), datasize, MPI_INT, res2.data(), datasize, MPI_INT, root, MPI_COMM_WORLD);

    if (my_rank == root) {
        time2 = MPI_Wtime() - time2;
        for (int i = 0; i < datasize * tasks; i++) {
            if (res1[i] != res2[i]) {
                // std::cout << "Not a match!" << std::endl;
                ASSERT_EQ(res1[i], res2[i]);
            }
        }
        ASSERT_LT(abs(time2 - time1), 1);
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
