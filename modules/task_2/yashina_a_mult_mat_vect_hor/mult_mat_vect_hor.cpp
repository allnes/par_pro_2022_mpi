// Copyright 2022 Yashina Anastasia
#include <../../../modules/task_2/yashina_a_mult_mat_vect_hor/mult_mat_vect_hor.h>
#include <algorithm>
#include <vector>
#include <ctime>

std::vector<int> Vector(int n) {
    std::mt19937 gen;
    std::vector<int> vec(n);
    std::default_random_engine rand_val;
    gen.seed(static_cast<unsigned int>(time(0)));
    for (int i = 0; i < n; i++) {
        vec[i] = gen() % 10;
    }
    return vec;
}

std::vector<int> Matrix(int row, int col) {
    if (row <= 0) {
        throw - 111;
    }
    if (col <= 0) {
        throw - 111;
    }
    std::mt19937 gen;
    std::vector<int> M(row*col);
    std::default_random_engine rand_val;
    gen.seed(static_cast<unsigned int>(time(0)));
    for (int i = 0; i < row*col; i++) {
        M[i] = gen() % 10;
    }
    return M;
}

std::vector<int>ConstVector(int n, int c) {
    std::vector <int> vec(n);
    for (int i = 0; i < n; i++) {
        vec[i] = c;
    }
    return vec;
}

std::vector <int> Multip(const std::vector <int> &matr, int row, int col, const std::vector <int> &vect) {
    int size;
    int rank;
    int ost;
    int delta;
    int flag = 1;
    int S = vect.size();
    if ((S != col) || (row <= 0) || (col <= 0))
        throw - 111;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    ost = row % size;
    if (row < size) {
        size = 1;
        ost = 0;
        flag = 0;
    }
    delta = row / size;

    if (rank == 0) {
        int num;
        for (int i = col * (delta + ost); i <= col * (row - delta); i += col * delta) {
            num = (i / col - ost) / delta;
            MPI_Send(&matr[i], col * delta, MPI_INT, num, 0, MPI_COMM_WORLD);
        }
    }

    MPI_Status status;
    std::vector <int> mat(col * delta, 0);
    std::vector <int> tmp(delta);
    std::vector <int> res(row);

    if (rank == 0) {
        tmp.resize(delta + ost);
        for (int i = 0; i < col * (delta + ost); i += col) {
            for (int j = 0; j < col; j++)
                tmp[i / col] += matr[i + j] * vect[j];
        }
    } else {
        if (flag == 1) {
            int n = col * delta;
            MPI_Recv(&mat[0], n, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
            for (int i = 0; i < col * delta; i += col) {
                for (int j = 0; j < col; j++)
                    tmp[i / col] += mat[i + j] * vect[j];
            }
        }
    }

    if (flag == 1) {
        MPI_Gather(&tmp[0], delta, MPI_INT, &res[ost], delta, MPI_INT, 0, MPI_COMM_WORLD);
        if (rank == 0) {
            for (int i = 0; i < delta + ost; i++)
                res[i] = tmp[i];
        }
        return res;
    } else {
        if (rank == 0) {
            return tmp;
        } else {
            std::vector <int> err = { -111 };
            return err;
        }
    }
}
