// Copyright 2022 Alexey Tychinin


#include "../../../modules/task_2/tychinin_a_simple_iteration/simple_iteration.h"
#include <mpi.h>

#include <iostream>
#include <cmath>

namespace task2 {
    double Norm(const std::vector<double>& vec) {
        double norm = 0.;
        for (const double curr_val : vec) {
            double abs_val = std::abs(curr_val);
            if (abs_val > norm) {
            norm = abs_val;
            }
        }
        return norm;
    }

    std::vector<double> simpleIterationSeq(std::vector<double> a,
                                           std::vector<double> b,
                                           const double eps) {
        if (a.size() != b.size() * b.size()) {
            throw "Non-equal dimensions";
        }

        size_t b_size = b.size();
        double temp, norm = 0.;

        for (size_t i = 0; i < b_size; i++) {
            temp = a[i * b_size + i];
            if (temp == 0) {
                throw "Zero element detected";
            }
            b[i] /= temp;
            for (size_t j = 0; j < b_size; j++) {
                if (i == j) {
                    a[i * b_size + j] = 0;
                } else {
                    a[i * b_size + j] /= temp;
                }
                if (std::abs(a[i * b_size + j]) > norm) {
                    norm = std::abs(a[i * b_size + j]);
                }
            }
        }

        if (norm >= 1) {
            throw "No diagonal prevalence";
        }
        std::vector<double> x_old(b);
        std::vector<double> x_new(b);

        do {
            x_old = x_new;
            x_new = b;
            norm = 0;
            for (size_t i = 0; i < b_size; i++) {
                for (size_t j = 0; j < b_size; j++) {
                    x_new[i] -= a[i * b_size + j] * x_old[j];
                }
                if (std::abs(x_new[i] - x_old[i]) > norm) {
                    norm = std::abs(x_new[i] - x_old[i]);
                }
            }
        } while (norm > eps);
        return x_new;
    }

    std::vector<double> simpleIteration(std::vector<double> a,
                                        std::vector<double> b,
                                        const double eps) {
        if (a.size() != b.size() * b.size()) {
            throw "Non-equal dimensions";
        }
        int b_size = static_cast<int>(b.size());

        int size, rank;
        MPI_Comm_size(MPI_COMM_WORLD, &size);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        if (b_size < size) {
            return simpleIterationSeq(a, b, eps);
        }
        std::vector<int> countsvec(size), countsmat(size), displsvec(size), displsmat(size);
        int len = b_size / size;
        int rem = b_size % size;
        countsvec[0] = len + rem;
        countsmat[0] = (len + rem) * b_size;
        displsvec[0] = displsmat[0] = 0;
        for (int i = 1; i < size; i++) {
            countsvec[i] = len;
            countsmat[i] = len * b_size;
            displsvec[i] = rem + len * i;
            displsmat[i] = (rem + len * i) * b_size;
        }

        std::vector<double> lA(countsmat[rank]);
        std::vector<double> lb(countsvec[rank]);
        std::vector<double> x_new(countsmat[rank]);
        std::vector<double> x_old(b_size);

        MPI_Scatterv(&a[0], &countsmat[0], &displsmat[0], MPI_DOUBLE, &lA[0],
                    countsmat[rank], MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Scatterv(&b[0], &countsvec[0], &displsvec[0], MPI_DOUBLE, &lb[0],
                    countsvec[rank], MPI_DOUBLE, 0, MPI_COMM_WORLD);

        double diag, norm, lnorm = 0, lflag = 1, flag;
        for (int i = 0; i < countsvec[rank]; i++) {
            diag = lA[displsvec[rank] + i * b_size + i];
            if (diag == 0) {
                lflag = 0;
            }
            lb[i] /= diag;
            for (size_t j = 0; j < b_size; j++) {
                if (j == displsvec[rank] + i) {
                    lA[b_size * i + j] = 0;
                } else {
                    lA[b_size * i + j] /= diag;
                }
                if (std::abs(lA[b_size * i + j]) > lnorm) {
                    lnorm = std::abs(lA[b_size * i + j]);
                }
            }
        }
        MPI_Allreduce(&lflag, &flag, 1, MPI_DOUBLE, MPI_PROD, MPI_COMM_WORLD);
        if (flag == 0) {
            throw "Zero element";
        }

        MPI_Allreduce(&lnorm, &norm, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
        if (norm >= 1) {
            throw "No diagonal prevalence";
        }

        MPI_Gatherv(&lb[0], countsvec[rank], MPI_DOUBLE,
                    &x_old[0], &countsvec[0], &displsvec[0], MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Bcast(&x_old[0], b_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        do {
            lnorm = 0;
            for (int i = 0; i < countsvec[rank]; i++) {
                x_new[i] = lb[i];
                for (int j = 0; j < b_size; j++) {
                    x_new[i] -= lA[i * b_size + j] * x_old[j];
                }
                if (std::abs(x_new[i] - x_old[displsvec[rank] + i]) > lnorm) {
                    lnorm = std::abs(x_new[i] - x_old[displsvec[rank] + i]);
                }
            }
            MPI_Gatherv(&x_new[0], countsvec[rank], MPI_DOUBLE,
                        &x_old[0], &countsvec[0], &displsvec[0], MPI_DOUBLE, 0, MPI_COMM_WORLD);
            MPI_Bcast(&x_old[0], b_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
            MPI_Allreduce(&lnorm, &norm, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
        } while (norm > eps);

        return x_old;
    }
}  // namespace task2

