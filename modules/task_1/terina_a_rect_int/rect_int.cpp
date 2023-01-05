// Copyright 2022 Terina Alina
#include <mpi.h>
#include <cmath>
#include "../../../modules/task_1/terina_a_rect_int/rect_int.h"

double twox(double x) { return x * 2; }
double triplex(double x) { return x * x * x; }
double cosinus(double x) { return cos(x); }
double sinus(double x) { return sin(x); }
double ordinaryInt(double a, double b, double (*fotx)(double), int n) {
    const double dx = (b - a) / static_cast<double>(n);
    double z = 0;
    const double c = 0.5;
    for (int k = 0; k < n; k++) {
        z = z + fotx(a + (dx * k) + (c * dx));
    }
    return (z * dx);
}

double paralInt(double a, double b, double (*fotx)(double), int n) {
    int shag;
    const double dx = (b - a) / static_cast<double>(n);
    double final_res = 0;
    const double c = 0.5;
    int rank;
    MPI_Comm_size(MPI_COMM_WORLD, &shag);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double partial_res = 0;

    for (int k = rank; k < n; k += shag) {
        partial_res = partial_res + fotx(a + (dx * k) + (c * dx));
    }

    MPI_Reduce(&partial_res, &final_res, 1, MPI_DOUBLE, MPI_SUM, 0,
        MPI_COMM_WORLD);
    return (final_res * dx);
}
