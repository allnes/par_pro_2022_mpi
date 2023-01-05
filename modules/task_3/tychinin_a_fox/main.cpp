// Copyright 2022 Tychinin Alexey


#include <gtest/gtest.h>
#include <cmath>
#include "../../../modules/task_3/tychinin_a_fox/fox.h"
#include <gtest-mpi-listener.hpp>

TEST(Fox, Matrix1X1) {
    int proc_rank, proc_num;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &proc_num);
    size_t size = 1;
    int blocks = static_cast<int>(sqrt(proc_num));
    if (blocks * blocks == proc_num) {
        std::vector<double> a, b, ab_fox;
        int add = size % blocks;
        if (add) {
            size += blocks - (size % blocks);
        }
        if (proc_rank == 0) {
            a = task3::generateMatrix(size);
            b = task3::generateMatrix(size);
        }
        ab_fox = task3::foxMult(a, b, size);
        if (proc_rank == 0) {
            std::vector<double> ab_seq = task3::simpleMult(a, b, size);
            for (int i = 0; i < size * size; i++) {
                ASSERT_NEAR(ab_fox[i], ab_seq[i], 0.0001);
            }
        }
    }
}

TEST(Fox, Matrix5X5) {
    int proc_rank, proc_num;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &proc_num);
    size_t size = 5;
    int blocks = static_cast<int>(sqrt(proc_num));
    if (blocks * blocks == proc_num) {
        std::vector<double> a, b, ab_fox;
        int add = size % blocks;
        if (add) {
            size += blocks - (size % blocks);
        }
        if (proc_rank == 0) {
            a = task3::generateMatrix(size);
            b = task3::generateMatrix(size);
        }
        ab_fox = task3::foxMult(a, b, size);
        if (proc_rank == 0) {
            std::vector<double> ab_seq = task3::simpleMult(a, b, size);
            for (int i = 0; i < size * size; i++) {
                ASSERT_NEAR(ab_fox[i], ab_seq[i], 0.0001);
            }
        }
    }
}

TEST(Fox, Matrix10X10) {
    int proc_rank, proc_num;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &proc_num);
    size_t size = 10;
    int blocks = static_cast<int>(sqrt(proc_num));
    if (blocks * blocks == proc_num) {
        std::vector<double> a, b, ab_fox;
        int add = size % blocks;
        if (add) {
            size += blocks - (size % blocks);
        }
        if (proc_rank == 0) {
            a = task3::generateMatrix(size);
            b = task3::generateMatrix(size);
        }
        ab_fox = task3::foxMult(a, b, size);
        if (proc_rank == 0) {
            std::vector<double> ab_seq = task3::simpleMult(a, b, size);
            for (int i = 0; i < size * size; i++) {
                ASSERT_NEAR(ab_fox[i], ab_seq[i], 0.0001);
            }
        }
    }
}

TEST(Fox, Matrix20X20) {
    int proc_rank, proc_num;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &proc_num);
    size_t size = 20;
    int blocks = static_cast<int>(sqrt(proc_num));
    if (blocks * blocks == proc_num) {
        std::vector<double> a, b, ab_fox;
        int add = size % blocks;
        if (add) {
            size += blocks - (size % blocks);
        }
        if (proc_rank == 0) {
            a = task3::generateMatrix(size);
            b = task3::generateMatrix(size);
        }
        ab_fox = task3::foxMult(a, b, size);
        if (proc_rank == 0) {
            std::vector<double> ab_seq = task3::simpleMult(a, b, size);
            for (int i = 0; i < size * size; i++) {
                ASSERT_NEAR(ab_fox[i], ab_seq[i], 0.0001);
            }
        }
    }
}

TEST(Fox, Matrix50X50) {
    int proc_rank, proc_num;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &proc_num);
    size_t size = 50;
    int blocks = static_cast<int>(sqrt(proc_num));
    if (blocks * blocks == proc_num) {
        std::vector<double> a, b, ab_fox;
        int add = size % blocks;
        if (add) {
            size += blocks - (size % blocks);
        }
        if (proc_rank == 0) {
            a = task3::generateMatrix(size);
            b = task3::generateMatrix(size);
        }
        ab_fox = task3::foxMult(a, b, size);
        if (proc_rank == 0) {
            std::vector<double> ab_seq = task3::simpleMult(a, b, size);
            for (int i = 0; i < size * size; i++) {
                ASSERT_NEAR(ab_fox[i], ab_seq[i], 0.0001);
            }
        }
    }
}

TEST(Fox, Matrix100X100) {
    int proc_rank, proc_num;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &proc_num);
    size_t size = 100;
    int blocks = static_cast<int>(sqrt(proc_num));
    if (blocks * blocks == proc_num) {
        std::vector<double> a, b, ab_fox;
        int add = size % blocks;
        if (add) {
            size += blocks - (size % blocks);
        }
        if (proc_rank == 0) {
            a = task3::generateMatrix(size);
            b = task3::generateMatrix(size);
        }
        ab_fox = task3::foxMult(a, b, size);
        if (proc_rank == 0) {
            std::vector<double> ab_seq = task3::simpleMult(a, b, size);
            for (int i = 0; i < size * size; i++) {
                ASSERT_NEAR(ab_fox[i], ab_seq[i], 0.0001);
            }
        }
    }
}

int main(int argc, char** argv) {
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
