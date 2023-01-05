// Copyright 2022 Selivankin Sergey
#include <mpi.h>
#include <vector>
#include <random>
#include <algorithm>
#include "../../../modules/task_3/selivankin_s_strassen/strassen.h"


std::vector<double> getRandomMatrix(int m, int n) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::vector<double> vec(m * n);
    for (int i = 0; i < m * n; i++) { vec[i] = static_cast<double>(gen() % 100 + 1); }
    return vec;
}

int findNewMatrixSize(int current_size) {
    int n = 1;
    while (current_size > n) {
        n *= 2;
    }
    return n;
}

std::vector<double> convertMatrixToRequiredSize(const std::vector<double>& mat, int m, int n, int* new_size) {
    *new_size = findNewMatrixSize(std::max(m, n));
    std::vector<double> result((*new_size) * (*new_size));

    for (int i = 0, j = 0; i < result.size(); ++i) {
        if (i % *new_size < n && j < m * n) {
            result[i] = mat[j++];
        } else {
            result[i] = 0;
        }
    }

    return result;
}

std::vector<double> sumMatrix(bool isSum, const std::vector<double>& mat1, const std::vector<double>& mat2) {
    std::vector<double> result(mat1.size());

    for (int i = 0; i < mat1.size(); ++i) {
        result[i] = isSum ? mat1[i] + mat2[i] : mat1[i] - mat2[i];
    }

    return result;
}

std::vector<std::vector<double>> splitMatrixTo4SubMatrix(const std::vector<double>& mat) {
    std::vector<std::vector<double>> result(4);
    int n = static_cast<int>(sqrt(mat.size()));
    int split_n = n / 2;

    for (int i = 0; i < 4; ++i) {
        std::vector<double> subMat(split_n * split_n);
        result[i] = subMat;
    }

    for (int i = 0; i < mat.size() / 2; ++i) {
        int row = i / n;
        if (i % n >= split_n) {
            result[1][row * split_n + i % split_n] = mat[i];
        } else {
            result[0][row * split_n + i % split_n] = mat[i];
        }
    }
    for (int i = static_cast<int>(mat.size() / 2); i < mat.size(); ++i) {
        int row = i / n - n / 2;
        if (i % n >= split_n) {
            result[3][row * split_n + i % split_n] = mat[i];
        } else {
            result[2][row * split_n + i % split_n] = mat[i];
        }
    }

    return result;
}

std::vector<double> getStrassenSequence(const std::vector<double>& matA, const std::vector<double>& matB) {
    if (matA.size() == 4) {
        double P1 = (matA[0] + matA[3]) * (matB[0] + matB[3]);
        double P2 = (matA[2] + matA[3]) * (matB[0]);
        double P3 = (matA[0]) * (matB[1] - matB[3]);
        double P4 = (matA[3]) * (matB[2] - matB[0]);
        double P5 = (matA[0] + matA[1]) * (matB[3]);
        double P6 = (matA[2] - matA[0]) * (matB[0] + matB[1]);
        double P7 = (matA[1] - matA[3]) * (matB[2] + matB[3]);

        std::vector<double> matC(4);
        matC[0] = P1 + P4 - P5 + P7;
        matC[1] = P3 + P5;
        matC[2] = P2 + P4;
        matC[3] = P1 - P2 + P3 + P6;

        return matC;
    } else {
        std::vector<std::vector<double>> subMatsA(4);
        std::vector<std::vector<double>> subMatsB(4);

        subMatsA = splitMatrixTo4SubMatrix(matA);
        subMatsB = splitMatrixTo4SubMatrix(matB);

        std::vector<double> P1 = getStrassenSequence(sumMatrix(true, subMatsA[0], subMatsA[3]),
            sumMatrix(true, subMatsB[0], subMatsB[3]) );
        std::vector<double> P2 = getStrassenSequence(sumMatrix(true, subMatsA[2], subMatsA[3]),
            subMatsB[0]);
        std::vector<double> P3 = getStrassenSequence(subMatsA[0],
            sumMatrix(false, subMatsB[1], subMatsB[3]));
        std::vector<double> P4 = getStrassenSequence(subMatsA[3], sumMatrix(false, subMatsB[2],
            subMatsB[0]));
        std::vector<double> P5 = getStrassenSequence(sumMatrix(true, subMatsA[0], subMatsA[1]),
            subMatsB[3]);
        std::vector<double> P6 = getStrassenSequence(sumMatrix(false, subMatsA[2], subMatsA[0]),
            sumMatrix(true, subMatsB[0], subMatsB[1]));
        std::vector<double> P7 = getStrassenSequence(sumMatrix(false, subMatsA[1], subMatsA[3]),
            sumMatrix(true, subMatsB[2], subMatsB[3]));

        std::vector<double> C11(P1.size());
        std::vector<double> C12(P1.size());
        std::vector<double> C21(P1.size());
        std::vector<double> C22(P1.size());

        for (int i = 0; i < P1.size(); ++i) {
            C11[i] += P1[i] + P4[i] - P5[i] + P7[i];
            C12[i] += P3[i] + P5[i];
            C21[i] += P2[i] + P4[i];
            C22[i] += P1[i] - P2[i] + P3[i] + P6[i];
        }

        std::vector<double> matC(matA.size());

        int n = static_cast<int>(sqrt(matC.size()));
        int split_n = n / 2;

        for (int i = 0; i < matC.size() / 2; ++i) {
            int row = i / n;
            if (i % n >= split_n) {
                matC[i] = C12[row * split_n + i % split_n];
            } else {
                matC[i] = C11[row * split_n + i % split_n];
            }
        }
        for (int i = static_cast<int>(matC.size() / 2); i < matC.size(); ++i) {
            int row = i / n - n / 2;
            if (i % n >= split_n) {
                matC[i] = C22[row * split_n + i % split_n];
            } else {
                matC[i] = C21[row * split_n + i % split_n];
            }
        }

        return matC;
    }
}

std::vector<double> getStrassenParallel(const std::vector<double>& matA,
    const std::vector<double>& matB, int m, int n) {
    int rank, size;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int delta = 1;
    std::vector<double> matC(findNewMatrixSize(std::max(m, n)) * findNewMatrixSize(std::max(m, n)));

    if (rank == 0) {
        int new_size;

        std::vector<double> convertedMatA = convertMatrixToRequiredSize(matA, m, n, &new_size);
        std::vector<double> convertedMatB = convertMatrixToRequiredSize(matB, m, n, &new_size);

        std::vector<std::vector<double>> subMatsA(4);
        std::vector<std::vector<double>> subMatsB(4);

        subMatsA = splitMatrixTo4SubMatrix(convertedMatA);
        subMatsB = splitMatrixTo4SubMatrix(convertedMatB);

        std::vector<std::vector<double>> matrixForCalcP(14);

        matrixForCalcP[0] = sumMatrix(true, subMatsA[0], subMatsA[3]);
        matrixForCalcP[1] = sumMatrix(true, subMatsB[0], subMatsB[3]);

        matrixForCalcP[2] = sumMatrix(true, subMatsA[2], subMatsA[3]);
        matrixForCalcP[3] = subMatsB[0];

        matrixForCalcP[4] = subMatsA[0];
        matrixForCalcP[5] = sumMatrix(false, subMatsB[1], subMatsB[3]);

        matrixForCalcP[6] = subMatsA[3];
        matrixForCalcP[7] = sumMatrix(false, subMatsB[2], subMatsB[0]);

        matrixForCalcP[8] = sumMatrix(true, subMatsA[0], subMatsA[1]);
        matrixForCalcP[9] = subMatsB[3];

        matrixForCalcP[10] = sumMatrix(false, subMatsA[2], subMatsA[0]);
        matrixForCalcP[11] = sumMatrix(true, subMatsB[0], subMatsB[1]);

        matrixForCalcP[12] = sumMatrix(false, subMatsA[1], subMatsA[3]);
        matrixForCalcP[13] = sumMatrix(true, subMatsB[2], subMatsB[3]);

        int sub_size = static_cast<int>(subMatsA[0].size());
        std::vector<std::vector<double>> recvP(7);
        std::vector<int> sendTasks(7, 0);
        int currentTask = 0;

        for (int proc = 1; proc < (size > 7 ? 7 : size); ++proc) {
            MPI_Send(&sub_size,
                1, MPI_INT, proc, 0, MPI_COMM_WORLD);
            for (int i = 0; i < delta; ++i) {
                sendTasks[currentTask] = proc;
                MPI_Send(matrixForCalcP[currentTask * 2].data(),
                    sub_size, MPI_DOUBLE, proc, 1, MPI_COMM_WORLD);
                MPI_Send(matrixForCalcP[currentTask * 2 + 1].data(),
                    sub_size, MPI_DOUBLE, proc, 2, MPI_COMM_WORLD);

                currentTask++;
            }
        }

        for (int i = 0; i < 7; ++i) {
            if (sendTasks[i] == 0) {
                std::vector<double> proc_result;
                proc_result = getStrassenSequence(matrixForCalcP[i * 2], matrixForCalcP[i * 2 + 1]);

                recvP[i] = proc_result;
            } else {
                MPI_Status status;
                std::vector<double> proc_result(sub_size);
                MPI_Recv(proc_result.data(), static_cast<int>(proc_result.size()),
                    MPI_DOUBLE, sendTasks[i], 0, MPI_COMM_WORLD, &status);

                recvP[i] = proc_result;
            }
        }

        std::vector<double> C11(recvP[0].size());
        std::vector<double> C12(recvP[0].size());
        std::vector<double> C21(recvP[0].size());
        std::vector<double> C22(recvP[0].size());

        for (int i = 0; i < recvP[0].size(); ++i) {
            C11[i] += recvP[0][i] + recvP[3][i] - recvP[4][i] + recvP[6][i];
            C12[i] += recvP[2][i] + recvP[4][i];
            C21[i] += recvP[1][i] + recvP[3][i];
            C22[i] += recvP[0][i] - recvP[1][i] + recvP[2][i] + recvP[5][i];
        }

        int matC_n = new_size;
        int split_n = matC_n / 2;

        for (int i = 0; i < matC.size() / 2; ++i) {
            int row = i / matC_n;
            if (i % matC_n >= split_n) {
                matC[i] = C12[row * split_n + i % split_n];
            } else {
                matC[i] = C11[row * split_n + i % split_n];
            }
        }
        for (int i = static_cast<int>(matC.size() / 2); i < matC.size(); ++i) {
            int row = i / matC_n - matC_n / 2;
            if (i % matC_n >= split_n) {
                matC[i] = C22[row * split_n + i % split_n];
            } else {
                matC[i] = C21[row * split_n + i % split_n];
            }
        }
    } else if (rank < (size > 7 ? 7 : size)) {
        int countData;
        MPI_Status status;
        MPI_Status status1;
        MPI_Status status2;

        MPI_Recv(&countData, 1,
            MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

        std::vector<double> subMatA(countData);
        std::vector<double> subMatB(countData);

        MPI_Recv(subMatA.data(), static_cast<int>(subMatA.size()),
            MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, &status1);

        MPI_Recv(subMatB.data(), static_cast<int>(subMatB.size()),
            MPI_DOUBLE, 0, 2, MPI_COMM_WORLD, &status2);

        std::vector<double> proc_result;
        proc_result = getStrassenSequence(subMatA, subMatB);

        MPI_Send(proc_result.data(), static_cast<int>(proc_result.size()),
            MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }

    return matC;
}
