// Copyright 2022 Mitin Roman
#ifndef MODULES_TASK_3_MITIN_R_MULTI_DIM_INTEGR_MULTI_DIM_INTEGR_H_
#define MODULES_TASK_3_MITIN_R_MULTI_DIM_INTEGR_MULTI_DIM_INTEGR_H_

#include <mpi.h>
#include <cinttypes>

struct range_t {
    double a, b;
    uint64_t n;
};

template<typename func_t>
double integrate_seq(func_t func, range_t ranges[3]) {
    double h[3];

    for (int i = 0; i < 3; i++) {
        h[i] = (ranges[i].b - ranges[i].a) / ranges[i].n;
    }

    double res = 0.0;
    double elem_volume = h[0] * h[1] * h[2];

    double x, y, z;
    x = ranges[0].a;

    for ( int i = 0; i < ranges[0].n; x += h[0], i++ ) {
        y = ranges[1].a;
        for ( int j = 0; j < ranges[1].n; y += h[1], j++ ) {
            z = ranges[2].a;
            for ( int k = 0; k < ranges[2].n; z += h[2], k++ ) {
                res += func(x, y, z) * elem_volume;
            }
        }
    }

    return res;
}


template<typename func_t>
double integrate(func_t func, range_t ranges[3]) {
    int size;
    int rank;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double lenght = (ranges[0].b - ranges[0].a) / size;

    ranges[0].a += lenght * rank;
    ranges[0].b = ranges[0].a + lenght;
    ranges[0].n = (ranges[0].n + size - 1) / size;

    static int c = 0;
    c++;

    // if ( c < 10)
    //     printf("HELLO from %lf\n", ranges[0].a);


    double local_res = integrate_seq(func, ranges);
    double global_res = 0;

    // printf("local res = %lf\n", local_res);

    MPI_Allreduce(&local_res, &global_res, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

    // printf("global res = %lf\n", global_res);

    return global_res;
}

#endif  // MODULES_TASK_3_MITIN_R_MULTI_DIM_INTEGR_MULTI_DIM_INTEGR_H_
