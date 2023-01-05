  // Copyright 2022 Prokofev Denis
#ifndef MODULES_TASK_3_PROKOFEV_D_RADIX_BATCHER_DOUBLE_RADIX_BATCHER_DOUBLE_H_
#define MODULES_TASK_3_PROKOFEV_D_RADIX_BATCHER_DOUBLE_RADIX_BATCHER_DOUBLE_H_
#include <mpi.h>
void createCounters(double* data, uint64_t* counters, uint64_t N);
void radixPass(uint16_t Offset, uint64_t N, double* source,
    double* dest, uint64_t* count);
void doubleRadixLastPass(uint16_t Offset, uint64_t N,
    double* source, double* dest, uint64_t* count);
void doubleRadixSort(double** in, uint64_t N);
double* odd_even(double* vec1, double* vec2, int size1, int size2);
void RadixSortBatcher(double** vec, int size_v);
#endif  // MODULES_TASK_3_PROKOFEV_D_RADIX_BATCHER_DOUBLE_RADIX_BATCHER_DOUBLE_H_
