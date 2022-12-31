// Copyright 2022 Nikolaev Alexander
#include <mpi.h>
#include <vector>
#include <string>
#include <iostream>
#include <random>
#include <algorithm>
#include "../../../modules/task_3/nikolaev_a_shell_batcher_sort/shell_batcher.h"

std::vector<int> getRandomArray(int ArraySize) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::vector<int> array(ArraySize);
    for (int i = 0; i < ArraySize; i++) {
      array[i] = static_cast<int>(gen() % 100);
    }
    return array;
}

void PrintArray(const std::vector<int>& array) {
    const int size = array.size();
    for (int i = 0; i < size; i++) {
        std::cout << array[i] << " ";
    }
    std::cout << std::endl;
}

std::vector<int> GetShellSortSequential(std::vector<int> array) {
    int step = array.size() / 2;
    while (step > 0) {
        int j;
        for (int i = step; i < array.size(); i++) {
            int value = array[i];
            for (j = i - step; (j >= 0) && (array[j] > value); j -= step) array[j + step] = array[j];
            array[j + step] = value;
        }
        step /= 2;
    }
    return array;
}

// void compexch(int first, int second) {
//     if (first > second) std::swap(first, second);
// }

void OddEvenMergeBatcher(std::vector<int>* array, int hi, int str, int r) {
    int step = r * 2;
    if (step < (hi - str)) {
        OddEvenMergeBatcher(array, hi, str, step);
        OddEvenMergeBatcher(array, hi, str + r, step);
        for (int i = str + r; i < hi - r; i += step) {
            if ((*array)[i] > (*array)[i + r]) std::swap((*array)[i], (*array)[i + r]);
    }
    } else {
        if ((*array)[str] > (*array)[str + r]) std::swap((*array)[str], (*array)[str + r]);
    }
}

std::vector<int> MergeArray(const std::vector<int>& array1, const std::vector<int>& array2) {
    std::vector<int> mergeArray(array1.size() + array2.size());
    for (int i = 0; i < array1.size(); i++) {
        mergeArray[i] = array1[i];
    }
    for (int i = array1.size(), j = 0; i < mergeArray.size(); i++, j++) {
        mergeArray[i] = array2[j];
    }
    return mergeArray;
}

std::vector<int> GetShellSortParallel(const std::vector<int>& array, int ArraySize) {
    std::vector<int> global_array;
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int delta = ArraySize / size;
    std::vector<int> local_array(delta);
    MPI_Scatter(array.data(), delta, MPI_INT, local_array.data(), delta, MPI_INT, 0, MPI_COMM_WORLD);
    local_array = GetShellSortSequential(local_array);
    if (rank == 0) {
        std::vector<int> res_array;
        MPI_Status status;
        global_array = MergeArray(global_array, local_array);
        for (int proc = 1; proc < size; proc++) {
            MPI_Recv(local_array.data(), delta, MPI_INT, proc, 0, MPI_COMM_WORLD, &status);
            global_array = MergeArray(global_array, local_array);
        }
        if (size != 1) {
            for (int i = 0, j = 1; i < size / 2; i++, j++) {
                if (size - 1 == 1) {
                    OddEvenMergeBatcher(&global_array, global_array.size(), 0, 1);
                } else {
                    res_array = std::vector<int>(global_array.begin() + delta * size / 2 * i,
                                             global_array.begin() + delta * size / 2 * j);
                    OddEvenMergeBatcher(&res_array, res_array.size(), 0, 1);
                    global_array.erase(global_array.begin() + delta * size / 2 * i,
                                             global_array.begin() + delta * size / 2 * j);
                    global_array.insert(global_array.begin() + delta * size / 2 * i,
                                             res_array.begin(), res_array.end());
                }
            }
        }
        OddEvenMergeBatcher(&global_array, global_array.size(), 0, 1);
    } else {
        MPI_Send(local_array.data(), delta, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    return global_array;
}
