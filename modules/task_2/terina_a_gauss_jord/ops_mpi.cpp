// Copyright 2022 Terina Alina
#include <mpi.h>
#include <algorithm>
#include <iterator>
#include <random>
#include <string>
#include <vector>
#include "../../../modules/task_2/terina_a_gauss_jord/gauss_jord.h"

std::vector<std::vector<double>> getRandomMatrix(std::vector<double> rightpart,
    int matrsize) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::vector<std::vector<double>> matrcoefs;
    matrcoefs.resize(matrsize);
    for (int i = 0; i < matrsize; i++) {
        matrcoefs[i].resize(matrsize);
    }
    for (int i = 0; i < matrsize; i++) {
        for (int j = 0; j < matrsize; j++) {
            matrcoefs[i][j] = gen() % 100;
        }
    }
    for (int i = 0; i < matrcoefs.size(); i++) {
        matrcoefs[i].push_back(rightpart[i]);
    }
    return matrcoefs;
}

std::vector<double> getRandomRight(int matrsize) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::vector<double> vec(matrsize);
    for (int i = 0; i < matrsize; i++) {
        vec[i] = gen() % 100;
    }
    return vec;
}
std::vector<std::vector<double>>
expandMatr(std::vector<std::vector<double>> matrcoefs,
    std::vector<double> rightpart) {
    for (int i = 0; i < matrcoefs.size(); i++) {
        matrcoefs[i].push_back(rightpart[i]);
    }
    return matrcoefs;
}
std::vector<double> ordinaryGJ(const std::vector<std::vector<double>> &matrcoefs,
    const std::vector<double> &rightpart) {

    std::vector<std::vector<double>> Matrix;
    std::vector<double> sol(matrcoefs.size());
    Matrix = matrcoefs;
    int i, j, c, flag = 0, m = 0;
    double pro = 0;
    int n = Matrix.size();
    for (i = 0; i < n; i++) {
        if (Matrix[i][i] == 0) {
            c = 1;
            while ((i + c) < n && Matrix[i + c][i] == 0)
                c++;
            if ((i + c) == n) {
                break;
            }
            for (int j = i, k = 0; k <= n; k++)
                std::swap(Matrix[j][k], Matrix[j + c][k]);
        }

        for (int j = 0; j < n; j++) {
            if (i != j) {
                double pro = Matrix[j][i] / Matrix[i][i];

                for (int k = 0; k <= n; k++)
                    Matrix[j][k] = Matrix[j][k] - (Matrix[i][k]) * pro;
            }
        }
    }
    for (int i = 0; i < n; i++) {
        Matrix[i][n] = Matrix[i][n] / Matrix[i][i];
        sol[i] = Matrix[i][n];
    }
    Matrix.clear();
    return sol;
}

std::vector<double> matrix_random(std::vector<double> num_vec, int matrsize) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::uniform_int_distribution<> dist(1, 10);
    std::vector<double> gendistmatr(matrsize * (matrsize + 1));
    for (int i = 0; i < matrsize; i++) {
        double sum = 0;
        for (int j = 0; j < matrsize; j++) {
            gendistmatr[i * (matrsize + 1) + j] = dist(gen) + 1;
            sum += gendistmatr[i * (matrsize + 1) + j] * num_vec[j];
        }
        gendistmatr[(i + 1) * (matrsize + 1) - 1] = sum;
    }
    return gendistmatr;
}
int evaluateforScatterv(int fragm_count, int fragm_size, int it) {
    if (fragm_count > it)
        return fragm_size + 1;
    else
        return fragm_size;
}

void carve_up(int num, int matrsize, int fragm, double* fragments,
    double* sendvec) {
    for (int i = 0; i < fragm; i++) {
        double coeff = fragments[i * (matrsize + 1) + num] / sendvec[num];
        for (int j = 0; j < matrsize + 1; j++) {
            fragments[i * (matrsize + 1) + j] -= coeff * sendvec[j];
        }
    }
}

std::vector<double> ParGJ(std::vector<double> matrix, int matrsize) {
    int rank = 0, size = 0;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> nboxes;
    std::vector<int> displs;
    int fragm_size = 0;
    int fragm_count = 0;
    int matrsize1 = matrsize + 1;

    if (rank == 0) {
        fragm_size = matrsize / size;
        fragm_count = matrsize % fragm_size;
        nboxes.resize(size);
        displs.resize(size);
        displs[0] = 0;
        int i = 0;
        int count = evaluateforScatterv(fragm_count, fragm_size, i);

        nboxes[0] = count * matrsize1;
        for (int i = 1; i < size; i++) {
            count = evaluateforScatterv(fragm_count, fragm_size, i);
            nboxes[i] = count * matrsize1;
            displs[i] = displs[i - 1] + nboxes[i - 1];
        }
    }
    MPI_Bcast(&fragm_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&fragm_count, 1, MPI_INT, 0, MPI_COMM_WORLD);
    int fragment_size = evaluateforScatterv(fragm_count, fragm_size, rank);
    int recvcount = fragment_size * matrsize1;
    std::vector<double> sendbuf(recvcount);

    std::vector<double> sendvec;
    sendvec.resize(matrsize1);

    for (int i = 0; i < matrsize; i++) {
        if (rank == 0) {
            std::copy(matrix.data() + (i * matrsize1),
                matrix.data() + (i + 1) * matrsize1, sendvec.data());
        }
        MPI_Bcast(sendvec.data(), matrsize1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Scatterv(matrix.data(), nboxes.data(), displs.data(), MPI_DOUBLE,
            sendbuf.data(), recvcount, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        carve_up(i, matrsize, fragment_size, sendbuf.data(), sendvec.data());

        MPI_Gatherv(sendbuf.data(), recvcount, MPI_DOUBLE, matrix.data(),
            nboxes.data(), displs.data(), MPI_DOUBLE, 0, MPI_COMM_WORLD);

        if (rank == 0) {
            std::copy(sendvec.data(), sendvec.data() + matrsize1,
                matrix.data() + (i * matrsize1));
        }
    }
    std::vector<double> sol(matrsize);
    if (rank == 0) {
        for (int i = 0; i < matrsize; i++) {
            sol[i] = matrix[(i + 1) * matrsize1 - 1] / matrix[i * matrsize1 + i];
        }
    }
    nboxes.clear();
    displs.clear();
    sendbuf.clear();
    sendvec.clear();
    return sol;
}
