// Copyright 2022 Krainev Alexander
#include <mpi.h>
#include <algorithm>
#include <vector>
#include <random>
#include "../../../modules/task_2/krainev_a_bubble_sort/odd_even.h"

std::vector<int> getRandomVector(int size) {
    std::random_device device;
    std::mt19937 gen(device());
    std::vector<int> vec(size);
    for (int i = 0; i < size; ++i) {
        vec[i] = gen() % 10 + 5;
    }
    return vec;
}

std::vector<int> oddEvenSortSequential(const std::vector<int> &vec) {
    std::vector<int> copy(vec);
    size_t size = copy.size();

    for (size_t i = 0; i < size; ++i) {
        for (size_t j = (i % 2) ? 0 : 1; j + 1 < size; j += 2) {
            if (copy[j] > copy[j + 1]) {
                std::swap(copy[j], copy[j + 1]);
            }
        }
    }

    return copy;
}

std::vector<int> mergeLow(const std::vector<int> &a, const std::vector<int> &b) {
    size_t length = a.size();
    std::vector<int> result(length);

    int i, j, k;
    i = j = k = 0;
    while (k < length) {
        if (a[i] <= b[j]) {
            result[k++] = a[i++];
        } else {
            result[k++] = b[j++];
        }
    }

    return result;
}

std::vector<int> mergeHigh(const std::vector<int> &a, const std::vector<int> &b) {
    size_t length = a.size();
    std::vector<int> result(length);

    int i, j, k;
    i = j = k = length - 1;
    while (k >= 0) {
        if (a[i] > b[j]) {
            result[k--] = a[i--];
        } else {
            result[k--] = b[j--];
        }
    }

    return result;
}

std::vector<int> oddEvenSortParallel(const std::vector<int> &vec) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (size == 1) {
        return oddEvenSortSequential(vec);
    }

    size_t length = vec.size();
    const int delta = length / size;

    std::vector<int> localVec(delta);
    MPI_Scatter(vec.data() + delta * rank, delta, MPI_INT, localVec.data(), delta, MPI_INT, 0, MPI_COMM_WORLD);

    localVec = oddEvenSortSequential(localVec);
    std::vector<int> rBuf(delta);
    MPI_Status status;

    for (int i = 0; i < size; ++i) {
        std::vector<int> tmpVec;
        if (i % 2 == 0) {
            if (rank % 2 == 0) {
                MPI_Recv(rBuf.data(), delta, MPI_INT, rank + 1, 0, MPI_COMM_WORLD, &status);
                MPI_Send(localVec.data(), delta, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
                tmpVec = mergeLow(rBuf, localVec);
            } else {
                MPI_Send(localVec.data(), delta, MPI_INT, rank - 1, 0, MPI_COMM_WORLD);
                MPI_Recv(rBuf.data(), delta, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, &status);
                tmpVec = mergeHigh(rBuf, localVec);
            }
        } else if (rank > 0 && rank < size - 1) {
            if (rank % 2 == 0) {
                MPI_Recv(rBuf.data(), delta, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, &status);
                MPI_Send(localVec.data(), delta, MPI_INT, rank - 1, 0, MPI_COMM_WORLD);
                tmpVec = mergeHigh(rBuf, localVec);
            } else {
                MPI_Send(localVec.data(), delta, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
                MPI_Recv(rBuf.data(), delta, MPI_INT, rank + 1, 0, MPI_COMM_WORLD, &status);
                tmpVec = mergeLow(rBuf, localVec);
            }
        }
        localVec = tmpVec;
    }

    std::vector<int> result(length);
    MPI_Gather(localVec.data(), delta, MPI_INT, result.data(), delta, MPI_INT, 0, MPI_COMM_WORLD);

    return result;
}
