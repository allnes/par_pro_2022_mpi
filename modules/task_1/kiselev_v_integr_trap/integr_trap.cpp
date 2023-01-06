// Copyright 2023 Kiselev Vladislav
#include "../../../modules/task_1/kiselev_v_integr_trap/integr_trap.h"
#include <mpi.h>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <random>
#include <string>
#include <vector>
#include <functional>

const int COLOR_WORKER = 0;
const int COLOR_UNUSED = 1;

double integrateSequential(double a, double b, int segQ,
                           std::function<double(double)> fun) {
    double integral = 0.0;

    int i = 0;
    double st = (b - a) / segQ;

    for (double cur_x = a; i < segQ; cur_x += st, i++)
        integral += (fun(cur_x) + fun(cur_x + st)) / 2;
    integral *= st;

    return integral;
}

double integrateParallel(double a, double b, int segQ,
                         std::function<double(double)> fun) {
    int com_size, rg;
    MPI_Comm_size(MPI_COMM_WORLD, &com_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rg);

    const int workProcsQ = std::min(com_size, segQ / 2);

    int clr = rg < workProcsQ ? COLOR_WORKER : COLOR_UNUSED;
    MPI_Comm COMM_CUSTOM;
    MPI_Comm_split(MPI_COMM_WORLD, clr, rg, &COMM_CUSTOM);
    if (clr != COLOR_WORKER) {
        return 0.0;
    }

    const int q = segQ / workProcsQ;
    const int rem = segQ % workProcsQ;

    double st = (b - a) / segQ;

    double integral = 0.0;
    double locSum = 0.0;

    if (rg < workProcsQ) {
        if (rg == 0) {
            double cur_x = a + (q * st) * workProcsQ;
            for (int i = 0; i < rem; i++, cur_x += st)
                locSum += (fun(cur_x) + fun(cur_x + st)) / 2;
        }

        double cur_x = a + (q * st) * rg;
        for (int i = 0; i < q; i++, cur_x += st)
            locSum += (fun(cur_x) + fun(cur_x + st)) / 2;
    }

    MPI_Reduce(&locSum, &integral, 1, MPI_DOUBLE, MPI_SUM, 0, COMM_CUSTOM);

    if (rg == 0) integral *= st;
    return integral;
}
