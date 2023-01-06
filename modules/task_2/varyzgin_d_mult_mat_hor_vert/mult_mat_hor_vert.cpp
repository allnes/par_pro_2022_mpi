// Copyright 2023 Varyzgin Dmitriy
#include <../../../modules/task_2/varyzgin_d_mult_mat_hor_vert/mult_mat_hor_vert.h>
#include <mpi.h>

#include <algorithm>
#include <iostream>
#include <random>
#include <string>
#include <vector>

std::vector<int> genRandomMatrix(int m, int n) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::vector<int> matrix(m * n);
    for (int i = 0; i < m * n; i++) matrix[i] = gen() % 10000;
    return matrix;
}
std::vector<int> transpose(const std::vector<int>& B, int k) {
    std::vector<int> BT;
    for (int i = 0; i < k; i++)
        for (int j = i; j < B.size(); j += k) BT.push_back(B[j]);
    return BT;
}
std::vector<int> multMat(const std::vector<int>& A, const std::vector<int>& B,
                         int n) {
    int m = A.size() / n;
    int k = B.size() / n;
    std::vector<int> C(m * k);
    for (int i = 0; i < m; i++)
        for (int j = 0; j < k; j++) {
            int tmp = 0;
            for (int a = 0; a < n; a++) tmp += A[i * n + a] * B[k * a + j];
            C[i * k + j] = tmp;
        }
    return C;
}
std::vector<int> LocToGlob(const std::vector<int>& globRes, int k, int start,
                           const std::vector<int>& locRes, int x, int y) {
    std::vector<int> res = globRes;
    int p = 0;
    for (int i = 0; i < y; i++)
        for (int j = start + i * k; j < start + i * k + x; j++) {
            res[j] = locRes[p];
            p++;
        }
    return res;
}
std::vector<int> multMatParallel(const std::vector<int>& A,
                                 const std::vector<int>& B, int n) {
    int comm_size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int m = A.size() / n;  // число строк в A
    int k = B.size() / n;  // число столбцов в B

    const int processes =
        std::min(std::min(m, k), comm_size);  // число работающих процессов
    const int Atasks = m / processes;  // целое число строк на процесс
    const int Aother = m % processes;  // остаток главному процессу
    const int Btasks = k / processes;  // целое число строк на процесс
    const int Bother = k % processes;  // остаток главному процессу

    std::vector<char> buff(Atasks * n * Btasks * n * 2);

    int pos;
    std::vector<int> receivedAPart;
    std::vector<int> receivedBPart;
    int x;
    int y;
    std::vector<int> locRes;

    std::vector<int> globRes(m * k);

    if (rank == 0) {
        std::vector<int> BT =
            transpose(B, k);  // транспонирование для удобной адресации

        int insPlace;
        std::cout << m << ','<< k << ',' << comm_size << '\n';
        if (processes == 1) {
            // Обработка данных на глав. процессе если он единственный
            receivedAPart =
                std::vector<int>(A.begin(), A.begin() + n * (Atasks + Aother));
            receivedBPart = std::vector<int>(
                BT.begin(), BT.begin() + n * (Btasks + Bother));
            globRes = multMat(receivedAPart, transpose(receivedBPart, n), n);
            std::cout << "Hello\n";
        } else {
            if (Aother > 0) {
                // обработка особой строки результатов (кроме 0й ячейки)
                // 0##
                // 000
                // 000
                for (int p = 1; p < processes; p++) {
                    if (Bother > 0) {
                        pos = 0;
                        MPI_Pack(A.data(), n * Aother, MPI_INT, buff.data(),
                                 buff.size(), &pos, MPI_COMM_WORLD);
                        MPI_Pack(BT.data() + n * (Bother + Btasks * (p - 1)),
                                 n * Btasks, MPI_INT, buff.data(), buff.size(),
                                 &pos, MPI_COMM_WORLD);
                        MPI_Send(buff.data(), pos, MPI_PACKED, p,
                                 Bother + Btasks * (p - 1), MPI_COMM_WORLD);
                    }
                }
                // если есть особый столбец в матрице B - обработаем ячейку на
                // главном процессе, пока работают другие процессы
                // #00
                // 000
                // 000
                if (Bother > 0) {
                    receivedAPart =
                        std::vector<int>(A.begin(), A.begin() + n * Aother);
                    receivedBPart =
                        std::vector<int>(BT.begin(), BT.begin() + n * Bother);
                    locRes =
                        multMat(receivedAPart, transpose(receivedBPart, n), n);
                    insPlace = 0;
                    x = receivedBPart.size() / n;
                    y = receivedAPart.size() / n;
                    globRes = LocToGlob(globRes, k, insPlace, locRes, x, y);
                }
                // получение данных с других процессов
                for (int p = 1; p < processes; p++) {
                    MPI_Status status;
                    MPI_Recv(buff.data(), buff.size(), MPI_PACKED, p,
                             MPI_ANY_TAG, MPI_COMM_WORLD, &status);
                    pos = 0;
                    MPI_Unpack(buff.data(), buff.size(), &pos, locRes.data(),
                               locRes.size(), MPI_INT, MPI_COMM_WORLD);
                    MPI_Unpack(buff.data(), buff.size(), &pos, &x, 1, MPI_INT,
                               MPI_COMM_WORLD);
                    MPI_Unpack(buff.data(), buff.size(), &pos, &y, 1, MPI_INT,
                               MPI_COMM_WORLD);
                    insPlace =
                        status.MPI_TAG;  // тэг = начало вст. в матрице рез-в
                    globRes = LocToGlob(globRes, k, insPlace, locRes, x, y);
                }
            }

            // обработка строк
            for (int step = 1; step < processes; step++) {
                // обработка строк (кроме 0го столбца) на др. процессах
                // 000
                // 0##
                // 0##
                for (int p = 1; p < processes; p++) {
                    pos = 0;
                    MPI_Pack(A.data() + n * (Aother + (step - 1) * Atasks),
                             n * Atasks, MPI_INT, buff.data(), buff.size(),
                             &pos, MPI_COMM_WORLD);

                    MPI_Pack(BT.data() + n * (Bother + (p - 1) * Btasks),
                             n * Btasks, MPI_INT, buff.data(), buff.size(),
                             &pos, MPI_COMM_WORLD);

                    MPI_Send(buff.data(), pos, MPI_PACKED, p,
                             k * (Aother + (step - 1) * Atasks) + Bother +
                                 (p - 1) * Btasks,
                             MPI_COMM_WORLD);
                }
                // если есть особый столбец в матрице B - обработаем его
                // пошагово на главном процессе, пока работают другие процессы
                // 000
                // #00
                // #00
                if (Bother > 0) {
                    receivedAPart = std::vector<int>(
                        A.begin() + n * (Aother + Atasks * (step - 1)),
                        A.begin() + n * (Aother + Atasks * step));
                    receivedBPart =
                        std::vector<int>(BT.begin(), BT.begin() + n * Bother);
                    locRes =
                        multMat(receivedAPart, transpose(receivedBPart, n), n);
                    insPlace = k * (Aother + Atasks * (step - 1));
                    x = receivedBPart.size() / n;
                    y = receivedAPart.size() / n;
                    globRes = LocToGlob(globRes, k, insPlace, locRes, x, y);
                }
                // получение данных с других процессов
                for (int p = 1; p < processes; p++) {
                    MPI_Status status;
                    MPI_Recv(buff.data(), buff.size(), MPI_PACKED, p,
                             MPI_COMM_WORLD, MPI_ANY_TAG, &status);
                    pos = 0;
                    MPI_Unpack(buff.data(), buff.size(), &pos, locRes.data(),
                               locRes.size(), MPI_INT, MPI_COMM_WORLD);
                    MPI_Unpack(buff.data(), buff.size(), &pos, &x, 1, MPI_INT,
                               MPI_COMM_WORLD);
                    MPI_Unpack(buff.data(), buff.size(), &pos, &y, 1, MPI_INT,
                               MPI_COMM_WORLD);
                    insPlace = status.MPI_TAG;
                    globRes = LocToGlob(globRes, k, insPlace, locRes, x, y);
                }
            }
        }
    } else if (rank < processes) {
        // код для других процессов
        MPI_Status status;

        MPI_Recv(buff.data(), buff.size(), MPI_PACKED, 0, MPI_ANY_TAG,
                 MPI_COMM_WORLD, &status);

        pos = 0;
        MPI_Unpack(buff.data(), buff.size(), &pos, receivedAPart.data(),
                   receivedAPart.size(), MPI_INT, MPI_COMM_WORLD);
        MPI_Unpack(buff.data(), buff.size(), &pos, receivedBPart.data(),
                   receivedBPart.size(), MPI_INT, MPI_COMM_WORLD);

        x = receivedBPart.size() / n;
        y = receivedAPart.size() / n;
        locRes = multMat(receivedAPart, transpose(receivedBPart, n), n);

        pos = 0;
        MPI_Pack(locRes.data(), locRes.size(), MPI_INT, buff.data(),
                 buff.size(), &pos, MPI_COMM_WORLD);
        MPI_Pack(&x, 1, MPI_INT, buff.data(), buff.size(), &pos,
                 MPI_COMM_WORLD);
        MPI_Pack(&y, 1, MPI_INT, buff.data(), buff.size(), &pos,
                 MPI_COMM_WORLD);
        MPI_Send(buff.data(), pos, MPI_PACKED, 0, status.MPI_TAG,
                 MPI_COMM_WORLD);
    }
    return globRes;
}
