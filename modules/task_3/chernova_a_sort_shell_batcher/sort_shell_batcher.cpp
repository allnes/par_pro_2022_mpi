// Copyright 2022 Chernova Anna

#include "../../modules/task_3/chernova_a_sort_shell_batcher/sort_shell_batcher.h"

vector<int> randArr(int size) {
    vector<int> res;
    std::random_device rd;
    std::mt19937 RNG(rd());
    std::uniform_int_distribution<int> dist(0, 512);
    for (int i = 0; i < size; i++) {
        res.push_back(dist(RNG));
    }

    return res;
}

void oddEvenMerge(vector<int>* arr, int n, int lo, int r) {
    int m = r * 2;

    if (m < n) {
        oddEvenMerge(arr, n, lo, m);
        oddEvenMerge(arr, n, lo + r, m);

        for (int i = lo + r; i + r < lo + n; i += m) {
            if ((*arr)[i] > (*arr)[i + r])
                std::swap((*arr)[i], (*arr)[i + r]);
        }
    } else {
        if ((*arr)[lo] > (*arr)[lo + r]) {
            std::swap((*arr)[lo], (*arr)[lo+r]);
        }
    }
}

vector<int> merge(vector<vector<int>> vectors) {
    while (vectors.size() != 1) {
        for (int i = 0; i < static_cast<int>(vectors.size()); i++) {
            auto temp1 = vectors[i];
            temp1.insert(temp1.end(),
                vectors[i + 1].begin(), vectors[i + 1].end());
            oddEvenMerge(&temp1, temp1.size());
            vectors[i] = temp1;
            vectors.erase(vectors.begin() + i);
        }
    }
    return vectors[0];
}

void shellSort(vector<int> *arr) {
    for (int s = (*arr).size() / 2; s > 0; s /= 2) {
        for (int i = s; i < static_cast<int>((*arr).size()); ++i) {
            for (int j = i - s; j >= 0 && (*arr)[j] > (*arr)[j + s]; j -= s) {
                std::swap((*arr)[j], (*arr)[j + s]);
            }
        }
    }
}

vector<int> shellSortMPI(vector<int> arr, int size) {
    int procNum, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &procNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int sizeOfBuff = size / procNum;
    vector<int> localVector, result;

    localVector.resize(sizeOfBuff);

    MPI_Scatter(arr.data(), sizeOfBuff, MPI_INT, localVector.data(),
        sizeOfBuff, MPI_INT, 0, MPI_COMM_WORLD);
    shellSort(&localVector);

    if (rank != 0) {
        MPI_Send(localVector.data(), sizeOfBuff, MPI_INT, 0, 0, MPI_COMM_WORLD);
    } else {
        vector<vector<int>> vectorOfVal;
        vectorOfVal.push_back(localVector);

        for (int i = 1; i < procNum; ++i) {
            MPI_Recv(localVector.data(), sizeOfBuff,
                MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
            vectorOfVal.push_back(localVector);
        }

        result = merge(vectorOfVal);
    }

    return result;
}
