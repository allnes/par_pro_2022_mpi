// Copyright 2022 Muhin Vadim
#include <mpi.h>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include "../../../modules/task_1/muhin_v_check_lex_order_strings/check_lex_order_strings.h"


std::vector<char> getRandomString(std::vector<char>::size_type size) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::vector<char> vec(size);
    for (std::vector<char>::size_type i = 0; i < size; i++) {
        vec[i] = static_cast<char> (gen() % 100);
    }
    return vec;
}

int getSequentialOperations(const std::vector<char>& str1, const std::vector<char>& str2) {
    for (std::vector<char>::size_type i = 0; i < str1.size(); i++) {
        if (str1[i] < str2[i]) {
            return -1;
        } else {
            if (str1[i] > str2[i]) {
                return 1;
            }
        }
    }
    return 0;
}

int getParallelOperations(const std::vector<char>&str1, const std::vector<char>&str2,
                          std::vector<char>::size_type global_size) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<char>::size_type local_size = global_size / size;

    if (rank == 0) {
        for (int proc = 1; proc < size; proc++) {
            std::vector<char>::size_type step = proc * local_size;

            if (global_size % size != 0) {
                step += global_size % size;
            }

            std::vector<char> send_vector;
            for (std::vector<char>::size_type i = step; i < step + local_size; i++) {
                send_vector.push_back(str1[i]);
            }
            for (std::vector<char>::size_type i = step; i < step + local_size; i++) {
                send_vector.push_back(str2[i]);
            }
            MPI_Send(send_vector.data(), static_cast<int>(send_vector.size()),
                MPI_CHAR, proc, 0, MPI_COMM_WORLD);
        }
    }

    if (rank == 0) {
        local_size += global_size % size;
    }

    std::vector<char> local_vector(local_size * 2);
    if (rank == 0) {
        for (std::vector<int>::size_type i = 0; i < local_size; i++) {
            local_vector[i] = str1[i];
            local_vector[i + local_size] = str2[i];
        }
    } else {
        MPI_Status status;
        MPI_Recv(local_vector.data(), static_cast<int>(local_vector.size()),
            MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
    }
    std::vector<char> local_str1(local_size);
    std::vector<char> local_str2(local_size);

    for (std::vector<char>::size_type i = 0; i < local_size; i++) {
        local_str1[i] = local_vector[i];
        local_str2[i] = local_vector[i + local_size];
    }

    std::vector<int> local_res(size, 0);
    local_res[rank] = getSequentialOperations(local_str1, local_str2);

    std::vector<int> global_res(size, 0);
    MPI_Reduce(local_res.data(), global_res.data(), size,
        MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        for (const auto& res : global_res) {
            if (res == 1) {
                return 1;
            } else {
                if (res == -1) {
                    return -1;
                }
            }
        }
        return 0;
    }

    return 0;
}
