// Copyright 2022 Selivankin Sergey
#include <mpi.h>
#include <vector>
#include <random>
#include <algorithm>
#include "../../../modules/task_2/selivankin_s_median_filter/median_filter.h"


std::vector<int> getRandomMatrix(int m, int n) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::vector<int> vec(m * n);
    for (int i = 0; i < m * n; i++) { vec[i] = static_cast<int>(gen() % 100); }
    return vec;
}

int clamp(int value, int min, int max) {
    if (value < min) {
        return min;
    }
    if (value > max) {
        return max;
    }
    return value;
}

void appendSubMatrixToMatrix(std::vector<int> sub_mat, std::vector<int>* mat, int* begin_i) {
    for (int i = 0; i < sub_mat.size(); i++) {
        (*mat)[*begin_i + i] = sub_mat[i];
    }

    *begin_i += static_cast<int>(sub_mat.size());
}

std::vector<int> getMedianFilterSequence(std::vector<int> mat, int m, int n) {
    std::vector<int> result_mat(m * n);

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            int kernel_size = 3;
            int kernel_rad = kernel_size / 2;
            std::vector<int> kernel(kernel_size * kernel_size);
            int count_in_kernel = 0;

            for (int l = -kernel_rad; l <= kernel_rad; l++) {
                for (int k = -kernel_rad; k <= kernel_rad; k++) {
                    int x = clamp(i + l, 0, m - 1);
                    int y = clamp(j + k, 0, n - 1);

                    kernel[count_in_kernel++] = mat[x * n + y];
                }
            }

            std::sort(kernel.begin(), kernel.end());
            result_mat[i * n + j] = kernel[(kernel_size * kernel_size) / 2];
        }
    }
    return result_mat;
}

std::vector<int> getMedianFilterParallel(std::vector<int> global_mat, int m, int n) {
    int rank, size;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int delta = m / size;
    const int additional_delta = m % size;

    std::vector<int> global_result(m * n, 0);

    if (rank == 0) {
        for (int proc = 1; proc < size; proc++) {
            if (proc == size - 1) {
                MPI_Send(global_mat.data() + (proc * delta - 1) * n,
                    (delta + additional_delta + 1) * n, MPI_INT, proc, 0, MPI_COMM_WORLD);
            } else {
                MPI_Send(global_mat.data() + (proc * delta - 1) * n,
                    (delta + 2) * n, MPI_INT, proc, 0, MPI_COMM_WORLD);
            }
        }

        std::vector<int> local_mat((delta + (size == 1 ? 0 : 1)) * n);
        local_mat = std::vector<int>(global_mat.begin(), global_mat.begin() + (delta + (size == 1 ? 0 : 1)) * n);

        std::vector<int> result;
        result = getMedianFilterSequence(local_mat, delta + (size == 1 ? 0 : 1), n);
        std::vector<int> local_result(result.begin(), result.end() - (size == 1 ? 0 : 1) * n);

        int begin_i = 0;
        appendSubMatrixToMatrix(local_result, &global_result, &begin_i);

        for (int proc = 1; proc < size; proc++) {
            std::vector<int> proc_result((delta + (proc == size - 1 && size != 1 ? additional_delta : 0)) * n);

            MPI_Status status;
            MPI_Recv(proc_result.data(), (delta + (proc == size - 1 && size != 1 ? additional_delta : 0)) * n,
                MPI_INT, proc, 0, MPI_COMM_WORLD, &status);

            appendSubMatrixToMatrix(proc_result, &global_result, &begin_i);
        }
    } else if (rank == size - 1 && size != 1) {
        std::vector<int> local_mat((delta + additional_delta + 1) * n);

        MPI_Status status;
        MPI_Recv(local_mat.data(),
            (delta + additional_delta + 1) * n, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

        std::vector<int> result;
        result = getMedianFilterSequence(local_mat, delta + additional_delta + 1, n);

        std::vector<int> local_result(result.begin() + n, result.end());

        MPI_Send(local_result.data(),
            (delta + additional_delta) * n, MPI_INT, 0, 0, MPI_COMM_WORLD);
    } else {
        std::vector<int> local_mat((delta + 2) * n);

        MPI_Status status;
        MPI_Recv(local_mat.data(),
            (delta + 2) * n, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

        std::vector<int> result;
        result = getMedianFilterSequence(local_mat, delta + 2, n);

        std::vector<int> local_result(result.begin() + n, result.end() - n);

        MPI_Send(local_result.data(),
            delta * n, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    return global_result;
}
