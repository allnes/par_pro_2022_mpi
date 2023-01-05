// Copyright 2022 Tarasova Anastasia

#include "../../../modules/task_2/tarasova_a_simple_iterations/simple_iterations.h"

std::vector<double> CreateVector(const int size, const int min, const int max) {
    std::random_device rand_r;
    std::mt19937 random(rand_r());
    std::uniform_real_distribution<double> dist(min, max);
    std::vector<double> Vector(size);
    for (int i = 0; i < size; i++) {
        Vector.at(i) = dist(random);
    }
    return Vector;
}

std::vector<std::vector<double>> CreateMatrix(const int size) {
    std::vector<std::vector<double>> Matrix(size);
    std::vector<double> numbers = CreateVector(size, 10 * size + 1, 10 * size + 20);
    for (int i = 0; i < size; i++) {
        Matrix.at(i) = CreateVector(size, -10, 10);
        Matrix.at(i).at(i) = numbers.at(i);
    }
    return Matrix;
}

std::vector<double> GetSimpleIter(const std::vector<std::vector<double>>& a, const std::vector<double>& b,
    const int size, const double eps) {
    std::vector<double> answ(size), B = b;
    std::vector<std::vector<double>> A = a;
    for (int i = 0; i < size; i++) {
        double divid = A.at(i).at(i);
        if (divid < 0)
            B.at(i) /= -divid;
        else
            B.at(i) /= divid;
        for (int j = 0; j < size; j++) {
            if (i != j)
                A.at(i).at(j) /= (divid < 0) ? divid : -divid;
            else
                A.at(i).at(j) = 0;
        }
    }
    std::vector<double> pansw = B;
    bool flag = false;
    while (!flag) {
        answ = pansw;
        for (int i = 0; i < size; i++) {
            pansw.at(i) = 0;
        }
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                pansw.at(i) += A.at(i).at(j) * answ.at(j);
            }
            pansw.at(i) += B.at(i);
        }

        flag = true;
        for (int i = 0; i < size - 1; i++) {
            if (std::abs(answ.at(i) - pansw.at(i)) > eps) {
                flag = false;
            }
        }
    }
    return pansw;
}


std::vector<double> GetSimpleIterParallel(const std::vector<std::vector<double>>& a, const std::vector<double>& b,
    const int size, const double eps) {
    int ProcCount, ProcId;
    std::vector<std::vector<double>> A = a;
    std::vector<double> B = b;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcCount);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcId);
    int np = (size / ProcCount) * size + size / ProcCount;
    if (ProcId < size % ProcCount) {
        np += size + 1;
    }

    std::vector<double> Adata, Procdata(np);
    std::vector<int> Counts(ProcCount, (size / ProcCount) * size + size / ProcCount), Offsets(ProcCount, 0);
    if (ProcId == 0) {
        int Asize = size * size + size;
        Adata.reserve(Asize);
        for (int i = 0; i < size; i++) {
            A.at(i).push_back(B.at(i));
            Adata.insert(Adata.end(), A.at(i).begin(), A.at(i).end());
        }

        for (int i = 0; i < size % ProcCount; i++)
            Counts.at(i) += size + 1;
        for (int i = 1; i < ProcCount; i++)
            Offsets.at(i) = Offsets.at(i - 1) + Counts.at(i - 1);
    }
    MPI_Scatterv(Adata.data(), Counts.data(), Offsets.data(), MPI_DOUBLE, Procdata.data(),
        np, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    int Index = (size / ProcCount) * ProcId + std::min(ProcId, size % ProcCount);
    int Size = size + 1;
    for (int i = 0; i < np; i += Size) {
        double divid = Procdata.at(Index + i);
        int BIndex = i + Size - 1;
        if (divid < 0)
            Procdata.at(BIndex) /= -divid;
        else
            Procdata.at(BIndex) /= divid;
        for (int j = i; j < i + Size - 1; j++) {
            if (j != Index + i)
                Procdata.at(j) /= (divid < 0) ? divid : -divid;
            else
                Procdata.at(j) = 0;
        }
        Index++;
    }
    std::vector<double> answ(size), pransw(size), psansw;

    for (int i = 0; i < np; i += Size) {
        int Bindex = i + Size - 1;
        psansw.push_back(Procdata.at(Bindex));
    }

    Counts.assign(ProcCount, size / ProcCount);
    for (int i = 0; i < size % ProcCount; i++)
        Counts.at(i)++;
    Offsets.assign(ProcCount, 0);
    for (int i = 1; i < ProcCount; i++)
        Offsets.at(i) = Offsets.at(i - 1) + Counts.at(i - 1);


    MPI_Allgatherv(psansw.data(), psansw.size(), MPI_DOUBLE,
        answ.data(), Counts.data(), Offsets.data(), MPI_DOUBLE, MPI_COMM_WORLD);
    pransw = answ;
    bool flag = false;
    int step = 0;
    while (!flag) {
        if (step > 0) {
            MPI_Allgatherv(psansw.data(), psansw.size(), MPI_DOUBLE,
                answ.data(), Counts.data(), Offsets.data(), MPI_DOUBLE, MPI_COMM_WORLD);
            flag = true;
            for (int i = 0; i < size - 1; i++) {
                if (std::abs(pransw.at(i) - answ.at(i)) > eps) {
                    flag = false;
                }
            }
            pransw = answ;
        }

        if (!flag) {
            psansw.assign(Counts.at(ProcId), 0);
            for (int i = 0; i < Counts.at(ProcId); i++) {
                for (int j = i * Size, Index = 0; j < i * Size + Size - 1; j++, Index++) {
                    psansw.at(i) += Procdata.at(j) * answ.at(Index);
                }
                psansw.at(i) += Procdata.at(i * Size + Size - 1);
            }
        }
        step++;
    }
    return answ;
}
