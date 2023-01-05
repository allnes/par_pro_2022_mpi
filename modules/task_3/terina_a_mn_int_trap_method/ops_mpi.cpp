// Copyright 2022 Terina Alina
#include <mpi.h>
#include <random>
#include <vector>
#include "../../../modules/task_3/terina_a_mn_int_trap_method/trap_method.h"

double First(std::vector<double> coefs) {
    double x = coefs[0];
    double y = coefs[1];
    return (3 * x * x + 3 * y * y);
}

double Second(std::vector<double> coefs) {
    double x = coefs[0];
    double y = coefs[1];
    double z = coefs[2];
    return (2 * x + 2 * y + 2 * z);
}
std::vector<double> getRandomVector(int sz) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::vector<double> coefs(sz);
    for (int k = 0; k < sz; k++) { coefs[k] = gen() % 100; }
    return coefs;
}
double ParInt(std::vector <double> first_c, std::vector <double> second_c,
    double(*PodIntF)(std::vector<double>), int n) {
    int size;
    int rank;
    double sol = 0.0;
    int local_sz = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int extent = first_c.size();
    int ns = pow(n, extent - 1);
    std::vector<double> h(extent);
    std::vector<double> fc(extent);
    std::vector<double> sc(extent);
    int nc = n;
    if (rank == 0) {
        for (int k = 0; k < extent; ++k) {
            h[k] = (second_c[k] - first_c[k]);
            h[k] /= n;
            fc[k] = first_c[k];
            sc[k] = second_c[k];
        }
    }

    MPI_Bcast(&ns, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&nc, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&h[0], extent, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&fc[0], extent, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&sc[0], extent, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    int remain = ns % size;
    int count_sz = ns / size;
    if (rank < remain) {
        count_sz += 1;
        local_sz = rank * count_sz;
    } else {
        local_sz = rank * count_sz + remain;
    }

    std::vector<std::vector<double>> section(count_sz);
    for (int k = 0; k < count_sz; ++k) {
        int global_s = local_sz + k;
        for (int m = 0; m < extent - 1; ++m) {
            section[k].push_back(fc[m] + h[m] * (global_s % n + 0.5));
        }
    }

    double partialsum = 0.0;
    for (int k = 0; k < count_sz; ++k) {
        for (int m = 0; m < n; ++m) {
            double partInt = fc[extent - 1] + (m + 0.5) * h[extent - 1];
            section[k].push_back(partInt);
            partialsum = partialsum + PodIntF(section[k]);
            section[k].pop_back();
        }
    }

    MPI_Reduce(&partialsum, &sol, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    for (int k = 0; k < extent; ++k) {
        sol = sol * h[k];
    }
    return sol;
}

double OrdinaryInt(std::vector <double> first_c,
    std::vector <double> second_c, double(*PodIntF)(std::vector<double>), int n) {
    int extent = first_c.size();
    std::vector<double> h(extent);
    std::vector <double> section(extent);
    int ns = pow(n, extent);
    double sol = 0.0;
    for (int k = 0; k < extent; ++k) {
        h[k] = (second_c[k] - first_c[k]);
        h[k] /= static_cast<double>(n);
    }
    for (int k = 0; k < ns; ++k) {
        for (int m = 0; m < extent; ++m) {
            section[m] = first_c[m] + h[m] * (k % n + 0.5);
        }
        sol += PodIntF(section);
    }
    for (int k = 0; k < extent; ++k) {
        sol = sol * h[k];
    }
    return sol;
}
