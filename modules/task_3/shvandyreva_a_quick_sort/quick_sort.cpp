// Copyright 2022 Shvandyreva Alina
#include <mpi.h>
#include <iostream>
#include <vector>
#include <ctime>
#include <random>
#include <algorithm>
#include <climits>
#include "../../modules/task_3/shvandyreva_a_quick_sort/quick_sort.h"

std::vector<int> getRandomVector(int size) {
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    std::vector<int> vector(size, 0);
    for (int i = 0; i < size; ++i) {
        vector[i] = gen() % 100;
    }
    return vector;
}

void quickSortImplementation(std::vector<int> *array, int a, int b) {
    if (array == 0)
        throw "Array is incorrect";
    int first = a, last = b;
    int middle, tmp;
    middle = (*array)[(first + last) / 2];
    do {
        while ((*array)[first] < middle)
            first++;
        while ((*array)[last] > middle)
            last--;
        if (first <= last) {
            tmp = (*array)[first];
            (*array)[first] = (*array)[last];
            (*array)[last] = tmp;
            first++;
            last--;
        }
    } while (first < last);

    if (first < b)
        quickSortImplementation(&(*array), first, b);
    if (a < last)
        quickSortImplementation(&(*array), a, last);
}

std::vector<int> quickSortSequential(std::vector<int> arr) {
    quickSortImplementation(&arr, 0, static_cast<int>(arr.size()) - 1);
    return arr;
}

std::vector<int> mergeSort(std::vector<int> arr1, std::vector<int> arr2) {
    int i = 0, j = 0, l = 0;
    int arr1S = static_cast<int>(arr1.size());
    int arr2S = static_cast<int>(arr2.size());
    std::vector<int> res(arr1S + arr2S);
    while (i < arr1S && j < arr2S) {
        if (arr1[i] <= arr2[j]) {
            res[l] = arr1[i];
            i++;
        } else {
            res[l] = arr2[j];
            j++;
        }
        l++;
    }
    while (i < arr1S) {
        res[l] = arr1[i];
        i++;
        l++;
    }
    while (j < arr2S) {
        res[l] = arr2[j];
        j++;
        l++;
    }
    return res;
}

std::vector<int> quickSortParallel(std::vector<int> arr) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int old_size = static_cast<int>(arr.size());
    if (old_size <= size)
        return quickSortSequential(arr);
    int arr_size = static_cast<int>(std::ceil(static_cast<double>(old_size) / size)) * size;
    int local_size = arr_size / size;
    arr.resize(arr_size, INT_MAX);

    std::vector<int> local_vec(local_size);
    MPI_Scatter(arr.data(), local_size, MPI_INT, local_vec.data(), local_size, MPI_INT, 0, MPI_COMM_WORLD);
    local_vec = quickSortSequential(local_vec);
    MPI_Gather(local_vec.data(), local_size, MPI_INT, arr.data(), local_size, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        std::vector<int> indexes;
        for (int i = 0; i < size; i++)
            indexes.push_back((i + 1) * arr_size / size);
        for (int i = 0; i < size - 1; i++) {
            std::vector<int> left = { arr.begin(), arr.begin() + indexes[i] };
            std::vector<int> right = { arr.begin() + indexes[i], arr.begin() + indexes[i + 1] };
            std::vector<int> merged = mergeSort(left, right);
            std::copy(merged.begin(), merged.end(), arr.begin());
        }
    }
    MPI_Bcast(arr.data(), arr_size, MPI_INT, 0, MPI_COMM_WORLD);
    arr.resize(old_size);
    return arr;
}
