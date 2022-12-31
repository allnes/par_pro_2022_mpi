// Copyright 2022 Bochkarev Vladimir
#include <mpi.h>
#include <vector>
#include <random>
#include <algorithm>
#include <iostream>
#include "../../../modules/task_3/bochkarev_v_radix_sort/radix_sort.h"

std::vector<int> getRandomVector(int size) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::vector<int> vec(size);
    for (int i = 0; i < size; i++)
        vec[i] = gen() % 100;

    return vec;
}

void countSort(int* v, int d, int size) {
    std::vector<int> res(size);
    int count[10] = {0};

    for (int i = 0; i < size; i++) {
        count[(v[i] / d) % 10]++;
    }

    for (int i = 1; i < 10; i++) {
        count[i] += count[i - 1];
    }

    for (int i = size - 1; i >= 0; i--) {
        res[count[(v[i] / d) % 10] - 1] = v[i];
        count[(v[i] / d) % 10]--;
    }

    for (int i = 0; i < size; i++)
        v[i] = res[i];
}

std::vector<int> getSequentialOperations(std::vector<int> vector) {
    const int size = vector.size();
    int max = vector[0];
    for (int i = 0; i < size; i++) {
        max = std::max(max, vector[i]);
    }

    for (int i = 1; max / i > 0; i*=10) {
        countSort(vector.data(), i, size);
    }

    return vector;
}

std::vector<int> mergeAll(std::vector<std::vector<int> > m) {
    if (m.size() == 1) {
        return m[0];
    }
    const int size = m.size();
    std::vector<int> res(m[0].begin(), m[0].end());
    for (int i = 1; i < size; i++) {
        std::vector<int> v = m[i];
        auto iter = v.begin();
        int k = 0;

        while (k < res.size()) {
            if (iter != v.end() && res[k] > *iter) {
                res.insert(res.begin() + k, *iter);
                iter++;
            }
            k++;
        }
        while (iter != v.end()) {
            res.insert(res.end(), *iter);
            iter++;
        }
    }
    return res;
}

std::vector<int> getParallelOperations(std::vector<int> vector, int vec_size) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int delta = vec_size/size;
    std::vector<int> g_res(vec_size);

    if (delta != 0) {
        if (rank == 0) {
            for (int proc = 1; proc < size; proc++) {
                MPI_Send(vector.data() + proc * delta, delta, MPI_INT, proc, 0, MPI_COMM_WORLD);
            }
        }

        std::vector<int> local_vec(delta);
        if (rank == 0) {
            local_vec = std::vector<int>(vector.begin(), vector.begin() + delta);
        } else {
            MPI_Status status;
            MPI_Recv(local_vec.data(), delta, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        }

        std::vector<int> l_res = getSequentialOperations(local_vec);

        if (rank == 0) {
            std::vector<std::vector<int> > res;
            res.push_back(l_res);
            for (int proc = 1; proc < size; proc++) {
                MPI_Status status1;
                MPI_Recv(l_res.data(), delta, MPI_INT, proc, 0, MPI_COMM_WORLD, &status1);
                res.push_back(l_res);
            }
            g_res = mergeAll(res);
        } else {
            MPI_Send(l_res.data(), delta, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }
    if (vec_size % size != 0 && rank == 0) {
        std::vector<int> local_vec = std::vector<int>(vector.end() - vec_size % size, vector.end());
        std::vector<int> l_res = getSequentialOperations(local_vec);
        std::vector<std::vector<int> > res;
        if (delta != 0) {
            res.push_back(g_res);
        }
        res.push_back(l_res);
        g_res = mergeAll(res);
    }

    return g_res;
}
