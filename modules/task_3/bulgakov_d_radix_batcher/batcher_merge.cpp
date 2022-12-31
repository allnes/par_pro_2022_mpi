// Copyright 2022 Bulgakov Daniil

#include "../../modules/task_3/bulgakov_d_radix_batcher/batcher_merge.h"

void cmp_and_swap(double & first, double & second) {
    if (first > second) {
        std::swap(first, second);
    }
}

void merge(double * input, int size, int seq_len, int start_ind = 0, int offset = 1) {
    if (seq_len > 2) {
        merge(input, size, seq_len / 2, start_ind, offset * 2);
        merge(input, size, seq_len / 2, start_ind + offset, offset * 2);
        int iter = start_ind;
        for (int i = 1;  i < seq_len - 1; i++) {
        cmp_and_swap(input[iter], input[iter + offset]);
        iter += offset;
        }
    } else {
        cmp_and_swap(input[start_ind], input[start_ind + offset]);
    }
}

int find_size_of_pow2(int val1, int val2) {
    int val = std::max(val1, val2);
    if ((val & (val - 1))) {
        val = (1 << static_cast<int>(std::log2(val)+1));
    }
    return val;
}

bool is_pow2(int val) {
    return (!(val & (val - 1)));
}


dvec batcher_merge(dvec first, dvec second) {
    dvec output;

    if (first.size() == second.size() && is_pow2(first.size())) {
        output.insert(output.end(), first.begin(), first.end());
        output.insert(output.end(), second.begin(), second.end());
        merge(output.data(), output.size(), output.size());
    } else {
        int vec_len = find_size_of_pow2(first.size(), second.size());
        int add1 = vec_len - first.size();
        int add2 = vec_len - second.size();
        output.insert(output.end(), first.begin(), first.end());
        for (int i = 0; i < add1; i++) output.push_back(DBL_MAX);
        output.insert(output.end(), second.begin(), second.end());
        for (int i = 0; i < add2; i++) output.push_back(DBL_MAX);
        merge(output.data(), output.size(), output.size());
        output.erase(output.end() - add1 - add2, output.end());
    }
    return output;
}

dvec seq_batcher_merges(std::vector<dvec> vecs) {
    int i = 0;
    while (vecs.size() > 1) {
        if (i + 1 == vecs.size()) i = 0;
        vecs[i] = batcher_merge(vecs[i], vecs[i + 1]);
        vecs.erase(vecs.begin() + i + 1);
        if (i + 1 != vecs.size()) i++;
    }
    return vecs[0];
}
