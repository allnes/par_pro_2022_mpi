// Copyright 2023 Pikin Ilya

#include <mpi.h>
#include <vector>
#include <queue>
#include <random>
#include <iostream>
#include "../../../modules/task_3/pikin_i_merge_q_sort/merge_q_sort.h"

std::vector<double> getRandVec(int size) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(1.0, 1000.0);
    std::vector<double> vec(size);

    for (int i = 0; i < size; i++) {
        vec[i] = dist(gen);
    }
    return vec;
}

void merge(double* res, double* a, double* b, int size_a, int size_b) {
    int ind_a = 0;
    int ind_b = 0;
    int ind_res = 0;

    while (ind_a < size_a && ind_b < size_b) {
        if (a[ind_a] <= b[ind_b]) {
            res[ind_res] = a[ind_a];
            ind_a++;
        } else {
            res[ind_res] = b[ind_b];
            ind_b++;
        }
        ind_res++;
    }
    while (ind_a < size_a) {
        res[ind_res++] = a[ind_a++];
    }
    while (ind_b < size_b) {
        res[ind_res++] = b[ind_b++];
    }
}

void quickSort(double* ar, int n) {
    int i = 0;
    int j = n - 1;
    double mid = ar[n / 2];
    do {
        while (ar[i] < mid) {
            i++;
        }
        while (ar[j] > mid) {
            j--;
        }
        if (i <= j) {
            double tmp = ar[i];
            ar[i] = ar[j];
            ar[j] = tmp;

            i++;
            j--;
        }
    } while (i <= j);

    if (j > 0) {
        quickSort(ar, j + 1);
    }
    if (i < n) {
        quickSort(&ar[i], n - i);
    }
}

void parallelQuickSort(double* ar, int n) {
    int procNum, procRank;
    MPI_Comm_size(MPI_COMM_WORLD, &procNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);

    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (n == 0 || n == 1) return;

    if (n < procNum) {
        if (procRank == 0) quickSort(ar, n);
        return;
    }

    std::vector<int> counts;
    std::vector<int> displs;
    std::vector<double> local_part;

    if (procRank == 0) {
        counts.resize(procNum);
        displs.resize(procNum);

        int quotient = n / procNum;
        int remainder = n % procNum;
        int displ = 0;
        for (int rank_i = 0; rank_i < procNum; rank_i++) {
            int local_size = quotient;

            if (remainder != 0 && rank_i < remainder) local_size++;

            counts[rank_i] = local_size;
            displs[rank_i] = displ;
            displ += local_size;
        }
    }

    int local_size = n / procNum;
    int remainder = n % procNum;
    if (remainder != 0 && procRank < remainder) local_size++;
    local_part.resize(local_size);

    MPI_Scatterv(ar, counts.data(), displs.data(), MPI_DOUBLE,
        local_part.data(), local_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    quickSort(local_part.data(), local_size);

    MPI_Gatherv(local_part.data(), local_size, MPI_DOUBLE, ar, counts.data(),
        displs.data(), MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (procRank == 0) {
        std::deque<std::vector<double>> merge_queue;

        for (int i = 0; i < procNum; i++) {
            double* begin, * end;
            begin = &ar[0] + displs[i];
            end = begin + counts[i];
            merge_queue.push_back(std::vector<double>(begin, end));
        }

        while (merge_queue.size() > 1) {
            int size1, size2;
            size1 = merge_queue[0].size();
            size2 = merge_queue[1].size();
            std::vector<double> merge_result(size1 + size2);

            merge(merge_result.data(), merge_queue[0].data(),
                merge_queue[1].data(), size1, size2);

            merge_queue.pop_front();
            merge_queue.pop_front();
            merge_queue.push_back(merge_result);
        }

        std::vector<double>& result = merge_queue.front();
        for (int i = 0; i < result.size(); i++) {
            ar[i] = result[i];
        }
    }
    return;
}
