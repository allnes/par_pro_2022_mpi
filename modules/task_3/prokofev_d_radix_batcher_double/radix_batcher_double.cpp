  // Copyright 2022 Prokofev Denis
#include "../../../modules/task_3/prokofev_d_radix_batcher_double/radix_batcher_double.h"

#include <cmath>
#include <ctime>
#include <algorithm>
#include <cstring>
#include <utility>



void createCounters(double* data, uint64_t* counters, uint64_t N) {
    memset(counters, 0, 256 * sizeof(double) * sizeof(uint64_t));

    uint8_t* bp = reinterpret_cast<uint8_t*>(data);
    uint8_t* dataEnd = reinterpret_cast<uint8_t*>(data + N);
    uint16_t i;

    while (bp != dataEnd) {
        for (i = 0; i < sizeof(double); i++) counters[256 * i + *bp++]++;
    }
}

void radixPass(uint16_t Offset, uint64_t N, double* source, double* dest,
    uint64_t* count) {
    double* sp;
    uint64_t s, c, i, * cp;
    uint8_t* bp;

    s = 0;
    cp = count;
    for (i = 256; i > 0; --i, ++cp) {
        c = *cp;
        *cp = s;
        s += c;
    }

    bp = reinterpret_cast<uint8_t*>(source) + Offset;
    sp = source;
    for (i = N; i > 0; --i, bp += sizeof(double), ++sp) {
        cp = count + *bp;
        dest[*cp] = *sp;
        ++(*cp);
    }
}

void doubleRadixLastPass(uint16_t Offset, uint64_t N,
    double* source, double* dest, uint64_t* count) {
    double* sp;
    uint64_t s, c, i, * cp;
    uint8_t* bp;

    uint64_t numNeg = 0;
    for (i = 128; i < 256; i++) numNeg += count[i];

    s = numNeg;
    cp = count;
    for (i = 0; i < 128; ++i, ++cp) {
        c = *cp;
        *cp = s;
        s += c;
    }

    s = count[255] = 0;
    cp = count + 254;
    for (i = 254; i >= 128; --i, --cp) {
        *cp += s;
        s = *cp;
    }

    bp = reinterpret_cast<uint8_t*>(source) + Offset;
    sp = source;
    for (i = N; i > 0; --i, bp += sizeof(double), ++sp) {
        cp = count + *bp;
        if (*bp < 128)
            dest[(*cp)++] = *sp;
        else
            dest[--(*cp)] = *sp;
    }
}

void doubleRadixSort(double** in, uint64_t N) {
    double* out = new double[N];
    uint16_t i;

    uint64_t* counters = new uint64_t[sizeof(double) * 256], * count;
    createCounters((*in), counters, N);

    for (i = 0; i < sizeof(double) - 1; i++) {
        count = counters + 256 * i;
        if (count[0] == N) continue;
        radixPass(i, N, (*in), out, count);
        std::swap((*in), out);
    }
    count = counters + 256 * i;
    doubleRadixLastPass(i, N, (*in), out, count);

    delete (*in);
    (*in) = out;
    delete[] counters;
}


double* odd_even(double* vec1, double* vec2, int size1, int size2) {
    int el1 = 0, el2 = 0, k = 0;
    double* res = new double[size1 + size2];

    while (el1 < size1 && el2 < size2)
        if (vec1[el1] < vec2[el2]) {
            res[k] = vec1[el1];
            el1++;
            k++;
        } else {
            res[k] = vec2[el2];
            el2++;
            k++;
        }
    if (el1 == size1) {
        while (el2 < size2) {
            res[k] = vec2[el2];
            el2++;
            k++;
        }
    } else {
        while (el1 < size1) {
            res[k] = vec1[el1];
            el1++;
            k++;
        }
    }
    return res;
}

void RadixSortBatcher(double** vec, int size_v) {
    double* temp;
    int m = size_v;
    int id, size, delta;
    int step = 1;
    double* other;
    bool flag = false;
    double* myvec = *vec;
    double max = **vec;

    if (m % 2 != 0) {
        myvec = new double[m + 1];
        flag = true;
        for (int i = 0; i < m; i++) {
            *(myvec+i) = *(*vec+i);
            if (max < *(myvec + i)) max = *(myvec + i);
        }
        *(myvec+m) = max+1;
        m++;
    }


    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (id != 0) {
        MPI_Bcast(&delta, 1, MPI_INT, 0, MPI_COMM_WORLD);
        temp = new double[delta];
        MPI_Scatter(myvec, delta, MPI_DOUBLE, temp, delta, MPI_DOUBLE, 0,
            MPI_COMM_WORLD);
        doubleRadixSort(&temp, delta);
    } else {
        delta = m / size;

        MPI_Bcast(&delta, 1, MPI_INT, 0, MPI_COMM_WORLD);
        temp = new double[delta];
        MPI_Scatter(myvec, delta, MPI_DOUBLE, temp, delta, MPI_DOUBLE, 0,
            MPI_COMM_WORLD);
        doubleRadixSort(&temp, delta);
    }


    while (step < size) {
        int cur_id = id % (2 * step);
        if (cur_id == 0) {
            if (id + step < size) {
                MPI_Recv(&m, 1, MPI_INT, id + step, 0, MPI_COMM_WORLD,
                    MPI_STATUSES_IGNORE);
                other = new double[m];
                MPI_Recv(other, m, MPI_DOUBLE, id + step, 0, MPI_COMM_WORLD,
                    MPI_STATUSES_IGNORE);
                temp = odd_even(temp, other, delta, m);
                delta = delta + m;
                fflush(stdout);
            }
        } else {
            int near = id - step;
            MPI_Send(&delta, 1, MPI_INT, near, 0, MPI_COMM_WORLD);
            MPI_Send(temp, delta, MPI_DOUBLE, near, 0, MPI_COMM_WORLD);
            break;
        }
        step = step * 2;
    }
    if (id == 0) {
        if (flag) {
            for (int i = 0; i < m-1; i++) {
                *(myvec+i) = *(* vec + i);
            }
        }
        *vec = temp;
    }
}
