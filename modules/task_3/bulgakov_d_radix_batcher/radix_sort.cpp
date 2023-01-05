// Copyright 2022 Bulgakov Daniil

#include "../../modules/task_3/bulgakov_d_radix_batcher/radix_sort.h"

static union {
    uint64_t bits;
    double d;
} value;

void sorter(double * arr, int size, int iter, int base, int * negatives_cnt) {
    double * local_arr = new double[size];
    int * cnt = new int[base]();
    int mask = base - 1;
    (*negatives_cnt) = 0;
    int ind;

    for (int i = 0 ; i < size; i++) {
        value.d = arr[i];
        ind = (((value.bits) >> (8 * iter)) & mask);
        cnt[ind]++;
    }

    for (int i = 1; i < base; i++) {
        if (i >= (base >> 1) ) (*negatives_cnt)+=cnt[i];
        cnt[i] += cnt[i - 1];
    }

    for (int i = size - 1; i >= 0; i--) {
        value.d = arr[i];
        ind = (((value.bits) >> (8 * iter)) & mask);
        local_arr[cnt[ind] - 1] = arr[i];
        cnt[ind]--;
    }

    for (int i = 0; i < size; i++)
        arr[i] = local_arr[i];

    delete[] local_arr;
    delete[] cnt;
}

void radix_sort(double * arr, int size) {
    int bits = 8;
    int base = (1 << bits);
    int iters = (sizeof(double) * 8) / bits;
    int negatives = 0;

    for (int i = 0; i < iters; i++) {
        sorter(arr, size, i, base, &(negatives));
    }

    if (negatives == 0) return;

    std::reverse(arr + size - negatives,
                arr + size);
    double * negatives_buff = new double[negatives];
    memcpy(negatives_buff, (arr + size - negatives), negatives * sizeof(double));
    memmove((arr + negatives), arr,
        (size - negatives) * sizeof(double));
    memcpy(arr, negatives_buff, negatives * sizeof(double));
}
