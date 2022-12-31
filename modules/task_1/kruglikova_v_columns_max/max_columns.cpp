// Copyright 2022 Kruglikova Valeriia
#include <ctime>
#include <algorithm>
#include <random>
#include <iostream>
#include <vector>
#include "../../../modules/task_1/kruglikova_v_columns_max/max_columns.h"


std::vector<int> getMatrix(int n, int m) {
    if (n <= 0 || m <= 0) {
       throw -1;
    }
    if (n*m <= 4)
        throw -1;
    std::vector<int> res(n*m);
    std::default_random_engine rand_val;
    rand_val.seed(static_cast<unsigned int>(std::time(0)));
    for (int i = 0; i < n*m; i++) {
       res[i] = rand_val() % 100;
    }
    return res;
}

std::vector<int> getParallelMax(const std::vector<int>& mat, int n, int m) {
    int ProcRank, ProcNum;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    int codeErr = 0;
    int cols_delta = m / ProcNum;
    int cols_left = m % ProcNum;

    if ( ProcRank == 0 ) {
        if (static_cast<int>(mat.size()) != n*m)
            codeErr = -1;
        for (int prcs = 1; prcs < ProcNum; prcs++) {
            MPI_Send(&codeErr, 1, MPI_INT, prcs, 2, MPI_COMM_WORLD);
        }
    } else {
        MPI_Status status;
        MPI_Recv(&codeErr, 1, MPI_INT, 0, 2, MPI_COMM_WORLD, &status);
    }
    if (codeErr == -1)
        throw -1;
    std::vector<int> local_vec(cols_delta*n);

    if (ProcRank == 0) {
        std::vector<int> tmat(m*n);
        tmat = getTransposeMtx(mat, n, m);
        for (int prcs = 1; prcs < ProcNum; prcs++) {
            MPI_Send(&tmat[cols_left*n] + cols_delta*prcs*n, cols_delta*n, MPI_INT, prcs, 0, MPI_COMM_WORLD);
        }
        local_vec = std::vector<int>(tmat.begin(), tmat.begin() + cols_left*n + cols_delta*n);
    } else {
        MPI_Status status;
        MPI_Recv(&local_vec[0], cols_delta*n, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }

    std::vector<int> res;

    if (ProcRank == 0) {
        MPI_Status status;
        int elem;
        std::vector<int> append(n);
        for (int i = 0; i < cols_delta + cols_left; i++) {
            auto pos = local_vec.begin() + i*n;
            std::vector<int> tmp(pos, local_vec.begin() + (i+1)*n);
            elem = *std::max_element(tmp.begin(), tmp.end());
            res.push_back(elem);
        }

        for (int prcs = 1; prcs < ProcNum; prcs++) {
            MPI_Recv(&append[0], cols_delta, MPI_INT, prcs, 1, MPI_COMM_WORLD, &status);
            res.insert(res.end(), append.begin(), append.begin()+cols_delta);
        }
    } else {
        int elem;
        for (int i = 0; i < cols_delta; i++) {
            auto pos = local_vec.begin() + i*n;
            std::vector<int> tmp(pos, local_vec.begin() + (i+1)*n);
            elem = *std::max_element(tmp.begin(), tmp.end());
            res.push_back(elem);
        }
        MPI_Send(&res[0], cols_delta, MPI_INT, 0, 1, MPI_COMM_WORLD);
    }
    return res;
}

std::vector<int> getSequentialMax(const std::vector<int>& mat, int n, int m) {
    if (m*n != static_cast<int>(mat.size()))
        throw -1;
    std::vector<int> tmat = getTransposeMtx(mat, n, m);
    std::vector<int> maxes;
    int max_el;
    for (int i = 0; i < m; i++) {
        max_el = *std::max_element(tmat.begin() + i*n, tmat.begin() + (i+1)*n);
        maxes.push_back(max_el);
    }
    return maxes;
}
std::vector<int> getTransposeMtx(const std::vector<int>& mat, int n, int m) {
    if (m*n != static_cast<int>(mat.size()))
        throw -1;
    std::vector<int> tmp(m*n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            tmp[i+j*n] = mat[j+i*m];
        }
    }
    return tmp;
}
