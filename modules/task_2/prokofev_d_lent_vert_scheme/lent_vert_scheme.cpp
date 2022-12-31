  // Copyright 2022 Prokofev Denis
#include "../../../modules/task_2/prokofev_d_lent_vert_scheme/lent_vert_scheme.h"

std::vector<int> genMatr(int rows, int cols) {
    if (rows < 1 || cols < 1) {
        throw - 1;
    }
    std::mt19937 gen;
    gen.seed((unsigned)(time(0)));
    std::vector<int> mat(rows * cols);
    for (int i = 0; i < rows * cols; i++) {
        mat[i] = gen() % 10;
    }
    return mat;
}

std::vector<int> genVect(int rows) {
    if (rows < 1) {
        throw - 1;
    }
    std::mt19937 gen;
    gen.seed((unsigned)(time(0)));
    std::vector<int> mat(rows);
    for (int i = 0; i < rows; i++) {
        mat[i] = gen() % 10;
    }
    return mat;
}


std::vector<int> lentVertScheme(const std::vector<int>& mat,
    const std::vector<int>& vect, const size_t rows, const size_t cols) {

    int size, id, tsize;
    MPI_Comm_size(MPI_COMM_WORLD, &tsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm myComm = NULL;
    MPI_Comm comm2;
    if (tsize > rows || rows % tsize != 0) {
        int color;
        if (id < rows && tsize>rows || id < rows / tsize) {
            color = 1;
            MPI_Comm_split(MPI_COMM_WORLD, color, id, &myComm);
            MPI_Comm_size(myComm, &size);
            MPI_Comm_rank(myComm, &id);
        } else {
            color = 2;
            MPI_Comm_split(MPI_COMM_WORLD, color, id, &comm2);
            MPI_Comm_size(comm2, &size);
            MPI_Comm_rank(comm2, &id);
        }
    } else {
        size = tsize;
        myComm = MPI_COMM_WORLD;
    }
    if (myComm == NULL) return std::vector<int>{0};
    int chapter = rows / size;
    int vchapter = cols / size;
    std::vector<int>mmap(chapter * cols);
    std::vector<int>mvec(vchapter);
    std::vector<int>vres(chapter * cols);
    std::vector<int>tres(rows * cols);
    std::vector<int>nvec(rows * cols);
    int in = 0;
    for (std::size_t i = 0; i < cols; i++) {
        for (std::size_t j = 0; j < rows; j++) {
            nvec[in++] = mat[j * cols + i];
        }
    }
    MPI_Scatter(&nvec[0], chapter * cols, MPI_INT, &mmap[0],
        chapter * cols, MPI_INT, 0, myComm);
    MPI_Scatter(&vect[0], vchapter, MPI_INT, &mvec[0],
        vchapter, MPI_INT, 0, myComm);
    for (std::size_t i = 0; i < vchapter; i++) {
        for (std::size_t j = 0; j < rows; j++) {
            vres[i * rows + j] = mmap[i * rows + j] * mvec[i];
        }
    }

    MPI_Gather(&vres[0], chapter * cols, MPI_INT, &tres[0], chapter * cols,
        MPI_INT, 0, myComm);


    std::vector<int> res(rows);

    if (id == 0) {
        for (int i = 0; i < rows; i++) {
            res[i] = 0;
            for (int j = 0; j < cols; j++) {
                res[i] += tres[j * rows + i];
            }
        }
        return res;
    }
    return res;
}
