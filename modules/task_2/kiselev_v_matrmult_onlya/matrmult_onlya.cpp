// Copyright 2023 Kiselev Vladislav
#include "../../modules/task_2/kiselev_v_matrmult_onlya/matrmult_onlya.h"

#include <mpi.h>

#include <iostream>
#include <random>
#include <vector>

const int ROOT = 0;
const int ACTIVE = 0;
const int NOT_REQUIRED = 1;

std::vector<int> GenRandMatrix(int m, int n) {
    std::random_device dev;
    std::mt19937 engn(dev());
    std::vector<int> vec(m * n);
    for (int i = 0; i < (m * n); i++) {
        vec[i] = engn() % 200;
    }
    return vec;
}

std::vector<int> MultSequential(const std::vector<int>& MatrixA,
                                const std::vector<int>& MatrixB, int ma, int na,
                                int mb, int nb) {
    if (na != mb) {
        throw std::runtime_error(
            "Dimensions of matrices are not suitable for multiplication!");
    }

    // Usual matrix multiplication algorithm
    std::vector<int> MatrixResult(ma * nb);

    for (int i = 0; i < ma; i++) {
        for (int j = 0; j < nb; j++) {
            int item = 0;
            for (int l = 0; l < na; l++)
                item += MatrixA[l + i * na] * MatrixB[l * nb + j];
            MatrixResult[j + i * nb] = item;
        }
    }

    return MatrixResult;
}

std::vector<int> MultParallel(const std::vector<int>& MatrixA,
                              const std::vector<int>& MatrixB, int ma, int na,
                              int mb, int nb) {
    int TotalPCount, CurrRank;
    MPI_Comm_size(MPI_COMM_WORLD, &TotalPCount);
    MPI_Comm_rank(MPI_COMM_WORLD, &CurrRank);

    const int ActivePCount = std::min(TotalPCount, ma);
    const bool IsPActive = CurrRank < ActivePCount;
    const bool IsPRoot = CurrRank == 0;

    int Type = (CurrRank < ActivePCount) ? ACTIVE : NOT_REQUIRED;
    MPI_Comm CUSTOM_COMM;
    MPI_Comm_split(MPI_COMM_WORLD, Type, CurrRank, &CUSTOM_COMM);
    // Finish not active processes
    if (Type != ACTIVE) return std::vector<int>({-100});

    const int RowsPerPCount = ma / ActivePCount;
    const int AdditionalRowsCount = ma % ActivePCount;

    std::vector<int> MatrixAPartial;
    std::vector<int> SharedMatrixB(mb * nb);
    std::vector<int> MatrixResult(ma * nb);

    if (IsPRoot) {
        SharedMatrixB.assign(MatrixB.begin(), MatrixB.end());
    }

    // Share MatrixB between all processes
    MPI_Bcast(SharedMatrixB.data(), SharedMatrixB.size(), MPI_INT, ROOT,
              CUSTOM_COMM);

    if (IsPRoot) {
        MatrixAPartial = std::vector<int>(
            MatrixA.data(),
            MatrixA.data() + (RowsPerPCount + AdditionalRowsCount) * na);

        // Scatter A parts
        for (size_t OtherRank = 1; OtherRank < ActivePCount; OtherRank++) {
            MPI_Send(MatrixA.data() +
                         (RowsPerPCount + AdditionalRowsCount) * na +
                         (OtherRank - 1) * (RowsPerPCount * na),
                     RowsPerPCount * na, MPI_INT, OtherRank, 0, CUSTOM_COMM);
        }

    } else {
        int msg_size = RowsPerPCount * na;
        MatrixAPartial.resize(msg_size);
        MPI_Status sta;
        MPI_Recv(MatrixAPartial.data(), msg_size, MPI_INT, ROOT, 0, CUSTOM_COMM,
                 &sta);
    }

    int maOfPart =
        IsPRoot ? RowsPerPCount + AdditionalRowsCount : RowsPerPCount;
    int naOfPart = na;

    std::vector<int> PartialMatrixRes = MultSequential(
        MatrixAPartial, SharedMatrixB, maOfPart, naOfPart, mb, nb);

    if (IsPRoot) {
        // Gather results on rank = 0
        for (size_t OtherRank = 1; OtherRank < ActivePCount; OtherRank++) {
            MPI_Status sta;
            MPI_Recv(MatrixResult.data() +
                         (RowsPerPCount + AdditionalRowsCount) * nb +
                         (OtherRank - 1) * (nb * RowsPerPCount),
                     RowsPerPCount * nb, MPI_INT, OtherRank, 0, MPI_COMM_WORLD,
                     &sta);
        }

        for (int i = 0; i < static_cast<int>(PartialMatrixRes.size()); i++)
            MatrixResult[i] = PartialMatrixRes[i];
    } else {
        // Send partial results to rank = 0
        MPI_Send(PartialMatrixRes.data(), PartialMatrixRes.size(), MPI_INT,
                 ROOT, 0, MPI_COMM_WORLD);
    }

    return MatrixResult;
}
