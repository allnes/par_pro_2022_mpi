  // Copyright 2022 Prokofev Denis
#include "../../../modules/task_1/prokofev_d_min_val_in_rows/min_val_in_rows.h"

std::vector<int> genMatr(int rows, int cols) {
    if ( rows < 1|| cols < 1 ) {
        throw - 1;
    } else {
    std::mt19937 gen;
    gen.seed((unsigned)(time(0)));
    std::vector<int> mat(rows * cols);
    for (int i = 0; i < rows * cols; i++) {
        mat[i] = gen() % 10;
    }
    return mat;
    }
}

std::vector<int> minValRows(const std::vector<int>& mat,
        const size_t rows, const size_t cols) {
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
    if (myComm == NULL) return std::vector<int>{};
    int chapter = rows / size;
    std::vector<int>mmap(chapter * cols);
    std::vector<int>mres(chapter);
    std::vector<int>res(rows);
    MPI_Scatter(&mat[0], chapter * cols, MPI_INT, &mmap[0],
        chapter * cols, MPI_INT, 0, myComm);

    for (int i = 0; i < chapter; i++) {
        mres[i] = mmap[i * cols];
        for (int j = 1; j < cols; j++) {
            if (mres[i] > mmap[j + i * cols]) mres[i] = mmap[j + i * cols];
        }
    }
    MPI_Barrier(myComm);
    MPI_Gather(&mres[0], chapter, MPI_INT, &res[0], chapter,
        MPI_INT, 0, myComm);
    return res;
}
