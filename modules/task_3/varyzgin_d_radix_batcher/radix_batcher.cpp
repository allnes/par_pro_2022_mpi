// Copyright 2023 Varyzgin Dmitriy

#include "../../modules/task_3/varyzgin_d_radix_batcher/radix_batcher.h"

void merge(double* arr, size_t size, size_t lengh, int begin = 0,
           int delta = 1) {
    if (lengh > 2) {
        // разбиваем пополам массив и сорт части рекурентно
        merge(arr, size, lengh / 2, begin, delta * 2);
        merge(arr, size, lengh / 2, begin + delta, delta * 2);
        // получился массив, половины которого отсортированы
        int step = begin;
        for (int i = 1; i < lengh - 1; i++, step += delta)
            if (arr[step] > arr[step + delta])
                std::swap(arr[step], arr[step + delta]);
    } else {
        if (arr[begin] > arr[begin + delta])
            std::swap(arr[begin], arr[begin + delta]);
    }
}

int find_size_of_pow2(int val1, int val2) {
    int val = std::max(val1, val2);
    if ((val & (val - 1))) {
        val = (1 << static_cast<int>(std::log2(val) + 1));
    }
    return val;
}
bool is_pow2(int val) { return (!(val & (val - 1))); }


std::vector<double> batcher_merge(std::vector<double> first,
                                  std::vector<double> second) {
    std::vector<double> res;

    if (first.size() == second.size() && is_pow2(first.size())) {
        res.insert(res.end(), first.begin(), first.end());
        res.insert(res.end(), second.begin(), second.end());
        merge(res.data(), res.size(), res.size());
    } else {
        size_t vec_len = find_size_of_pow2(first.size(), second.size());
        size_t add1 = vec_len - first.size();
        size_t add2 = vec_len - second.size();
        res.insert(res.end(), first.begin(), first.end());
        for (int i = 0; i < add1; i++) res.push_back(DBL_MAX);
        res.insert(res.end(), second.begin(), second.end());
        for (int i = 0; i < add2; i++) res.push_back(DBL_MAX);
        merge(res.data(), res.size(), res.size());
        res.erase(res.end() - add1 - add2, res.end());
    }
    return res;
}
std::vector<double> partsBatchMerge(std::vector<std::vector<double>> vecs) {
    int i = 0;
    while (vecs.size() > 1) {
        if (i + 1 == vecs.size()) i = 0;
        vecs[i] = batcher_merge(vecs[i], vecs[i + 1]);
        vecs.erase(vecs.begin() + i + 1);
        if (i + 1 != vecs.size()) i++;
    }
    return vecs[0];
}

std::vector<double> genDobVec(int size) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::vector<double> vector(size);
    // int генерация: for (int i = 0; i < m * n; i++) matrix[i] = gen() % 10000;
    for (int i = 0; i < size; i++) {
        vector[i] = (gen() % 1000) / static_cast<double>((gen() % 1000) + 1);
        // какие-то значения пусть будут отрицательными
        if (static_cast<double>(gen() % 4) == 2) vector[i] *= -1;
    }
    return vector;
}

// разрядная сортировка
static union {
    uint64_t bits;
    double d;
} value;
void sorter(double* arr, int size, int iter, int base, int* negatives_cnt) {
    double* local_arr = new double[size];
    int* cnt = new int[base]();
    int mask = base - 1;
    (*negatives_cnt) = 0;
    int ind;

    for (int i = 0; i < size; i++) {
        value.d = arr[i];
        ind = ((value.bits >> 8 * iter) & mask);
        cnt[ind]++;
    }

    for (int i = 1; i < base; i++) {
        if (i >= (base >> 1)) (*negatives_cnt) += cnt[i];
        cnt[i] += cnt[i - 1];
    }

    for (int i = size - 1; i >= 0; i--) {
        value.d = arr[i];
        ind = (((value.bits) >> (8 * iter)) & mask);
        local_arr[cnt[ind] - 1] = arr[i];
        cnt[ind]--;
    }

    for (int i = 0; i < size; i++) arr[i] = local_arr[i];

    delete[] local_arr;
    delete[] cnt;
}
void radix_sort(double* arr, int size) {
    int bits = 8;
    int base = 1 << bits; // побитовый сдвиг влево на 8
    int iters = (sizeof(double) * 8) / bits;
    int negatives = 0;

    for (int i = 0; i < iters; i++)
        sorter(arr, size, i, base, &(negatives));

    if (negatives == 0) return;

    std::reverse(arr + size - negatives, arr + size);
    double* negatives_buff = new double[negatives];
    memcpy(negatives_buff, (arr + size - negatives),
           negatives * sizeof(double));
    memmove((arr + negatives), arr, (size - negatives) * sizeof(double));
    memcpy(arr, negatives_buff, negatives * sizeof(double));
}

std::vector<double> radixBatcherParallel(std::vector<double> globVec) {
    int comm_size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int tasks = globVec.size() / comm_size;
    std::vector<double> locVec(tasks);

    MPI_Scatter(globVec.data(), tasks, MPI_DOUBLE, locVec.data(), tasks, MPI_DOUBLE,
                0, MPI_COMM_WORLD);

    radix_sort(locVec.data(), locVec.size());

    if (rank == 0) {
        std::vector<std::vector<double>> buff;
        buff.push_back(locVec);
        for (int i = 1; i < comm_size; i++) {
            MPI_Recv(locVec.data(), tasks, MPI_DOUBLE, i, 0, MPI_COMM_WORLD,
                     MPI_STATUS_IGNORE);
            buff.push_back(locVec);
        }
        return partsBatchMerge(buff);

    } else
        MPI_Send(locVec.data(), tasks, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    return locVec;
}
