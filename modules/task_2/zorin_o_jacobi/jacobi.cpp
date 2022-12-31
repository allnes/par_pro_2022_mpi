// Copyright 2022 Zorin Oleg
#include "../../modules/task_2/zorin_o_jacobi/jacobi.h"
#include <mpi.h>
#include <algorithm>
#include <vector>

Vector operator-(const Vector &X, const Vector &Y) {
    Vector res = X;
    for (std::size_t i = 0; i < res.size(); i++) {
        res[i] -= Y[i];
    }
    return res;
}

double normVector(const Vector &X) {
    double norm = 0.0;
    for (const double &x : X) {
        norm = std::max(norm, std::abs(x));
    }
    return norm;
}

Vector JacobiSequential(const Matrix& A, const Vector& B) {
    std::size_t dim = A.size();
    Vector X(dim);
    double norm = 0.0;

    do {
        Vector nextX = B;
        for (std::size_t i = 0; i < dim; i++) {
            const Vector &row = A[i];
            for (std::size_t j = 0; j < i; j++) {
                nextX[i] -= row[j] * X[j];
            }
            for (std::size_t j = i + 1; j < dim; j++) {
                nextX[i] -= row[j] * X[j];
            }
            nextX[i] /= row[i];
        }
        Vector diff = X - nextX;
        norm = normVector(diff);
        X = nextX;
    } while (norm > 1e-16);

    return X;
}

Vector expandVector(const Vector &V, std::size_t shift) {
    Vector res = V;
    res.insert(res.end(), shift, 0.0);
    return res;
}

Matrix expandMatrix(const Matrix &M, std::size_t shift) {
    Matrix res = M;

    for (Vector &row : res) {
        row.insert(row.end(), shift, 0.0);
    }
    res.insert(res.end(), shift, Vector(M.size() + shift));
    for (std::size_t i = M.size(); i < res.size(); i++) {
        res[i][i] = 1.0;
    }

    return res;
}

Vector JacobiParallel(Matrix A, Vector B) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    const std::size_t dim = A.size();
    Vector X(dim);
    std::size_t delta = dim / size;
    std::size_t shift = 0;
    if (dim % size != 0) {
        shift = size * (++delta) - dim;
        X = expandVector(X, shift);
        B = expandVector(B, shift);
        A = expandMatrix(A, shift);
    }
    Vector localX(delta);
    std::size_t pos = rank * delta;
    double norm = 0.0;
    int l = 0;
    do {
        l++;
        Vector localNextX(delta);
        for (std::size_t i = 0; i < delta; i++) {
            const Vector &row = A[i + pos];
            localNextX[i] = B[i + pos];
            for (std::size_t j = 0; j < i + pos; j++) {
                localNextX[i] -= row[j] * X[j];
            }
            for (std::size_t j = pos + i + 1; j < dim + shift; j++) {
                localNextX[i] -= row[j] * X[j];
            }
            localNextX[i] /= row[i + pos];
        }
        Vector localDiff = localX - localNextX;
        double localNorm = normVector(localDiff);
        MPI_Allreduce(&localNorm, &norm, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
        localX = localNextX;
        MPI_Allgather(localX.data(), delta, MPI_DOUBLE, X.data(), delta, MPI_DOUBLE, MPI_COMM_WORLD);
    } while (norm > 1e-16);

    X.erase(X.end() - shift, X.end());
    return X;
}
