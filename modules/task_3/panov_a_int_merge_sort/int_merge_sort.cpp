// Copyright 2022 Panov Alexey
#include <mpi.h>
#include <cmath>
#include <iostream>
#include <limits>
#include <iterator>
#include <algorithm>
#include "../../../modules/task_3/panov_a_int_merge_sort/int_merge_sort.h"


void sortParallel(Vector* arr_) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    Vector& arr = *arr_;
    Vector tempArr;

    const int step = (arr.size() / size) + ((arr.size() % size) != 0);
    const int arraySizeWithShift = step * size;

    if (rank == 0) {
        tempArr = arr;
        for (int i = tempArr.size(); i < arraySizeWithShift; i++) {
            tempArr.push_back(std::numeric_limits<int>::max());
        }
    }

    Vector arrPart = Vector(step);
    MPI_Scatter(tempArr.data(), step, MPI_INT, arrPart.data(), step, MPI_INT, 0, MPI_COMM_WORLD);

    intSort(&arrPart);

    MPI_Gather(arrPart.data(), step, MPI_INT, tempArr.data(), step, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        Vector result = Vector();
        result.reserve(arraySizeWithShift);
        for (int i = 0; i < size; i++) {
            result = mergeSort(result, tempArr, i * step, (i + 1) * step);
        }
        result.resize(arr.size());
        arr = result;
    }
}

Vector mergeSort(const Vector& first, const Vector& second, int sbegin, int send) {
    Vector result = Vector();
    result.reserve(first.size() + send - sbegin);
    std::merge(
        first.begin(),
        first.end(),
        second.begin() + sbegin,
        second.begin() + send,
        std::back_inserter(result));
    return result;
}

int getMaxCategoryForVector(Vector* arr) {
    int maxCategory = 0;
    for (int num : *arr) {
        maxCategory = std::max(maxCategory, getMaxCategoryForInt(num));
    }
    return maxCategory;
}

int getMaxCategoryForInt(int num) {
    int category = -1;
    do {
        num /= 10;
        category++;
    } while (num != 0);
    return category;
}

int getCategoryValue(int num, int category) {
    for (int i = 0; i < category; i++) {
        num /= 10;
    }
    return num % 10;
}

void intSortForCategory(Vector* arr_, int category) {
    Vector& arr = *arr_;
    auto tempArr = std::vector<Vector>(CATEGORIES_COUNT * 2);

    for (int i = 0; i < arr.size(); i++) {
        int categoryValue = getCategoryValue(arr[i], category);
        tempArr[categoryValue + CATEGORIES_COUNT - 1].push_back(arr[i]);
    }

    arr.resize(0);
    for (const auto& category : tempArr) {
        for (int num : category) {
            arr.push_back(num);
        }
    }
}

void intSort(Vector* arr) {
    const int maxCategory = getMaxCategoryForVector(arr);
    for (int i = 0; i <= maxCategory; i++) {
        intSortForCategory(arr, i);
    }
}
