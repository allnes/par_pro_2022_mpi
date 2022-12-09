// Copyright 2022 Artemev Alexej

#include <mpi.h>
#include "../../../modules/task_1/artemev_a_rect_intg/rect_intg.h"

float mp_intg_seq(float l, float r, int n, float(*f)(float)) {
    if (n <= 0) throw - 1;
    float intg = 0.0;
    float h = (r - l) / static_cast<float>(n);
    float x;
    for (int i = 0; i < n; i++) {
        x = l + (static_cast<float>(i) + 0.5) * h;
        intg = intg + f(x);
    }
    intg = intg * h;
    return intg;
}

float lrect_intg_seq(float l, float r, int n, float(*f)(float)) {
    if (n <= 0) throw - 1;
    float intg = 0.0;
    float h = (r - l) / static_cast<float>(n);
    float x;
    for (int i = 0; i < n; i++) {
        x = l + (static_cast<float>(i)) * h;
        intg = intg + f(x);
    }
    intg = intg * h;
    return intg;
}


float mp_intg_par(float l, float r, int n, float(*f)(float)) {
    if (n <= 0) throw - 1;

    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    float h = (r - l) / static_cast<float>(n);
    float local_sum = 0.0;
    float global_sum = 0.0;
    float x;

    for (int i = rank; i < n; i += size) {
        x = l + (static_cast<float>(i)+0.5) * h;
        local_sum =local_sum+ f(x);
    }
    local_sum = local_sum* h;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    return global_sum;
}

float lrect_intg_par(float l, float r, int n, float(*f)(float)) {
    if (n <= 0) throw - 1;

    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    float local_sum = 0.0;
    float global_sum = 0.0;
    float x;
    float h = (r - l) / static_cast<float>(n);
    
    for (int i = rank; i < n; i+=size) {
        x = l + static_cast<float>(i) * h;
        local_sum = local_sum + f(x);
    }
    local_sum = local_sum * h;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    return global_sum;
}


