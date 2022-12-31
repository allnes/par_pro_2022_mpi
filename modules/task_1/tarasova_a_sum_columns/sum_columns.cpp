// Copyright 2022 Tarasova Anastasia

#include "../../../modules/task_1/tarasova_a_sum_columns/sum_columns.h"

std::vector<int> CreateMatrix(const int rows, const int cols) {
    std::random_device rand_r;
    std::mt19937 random(rand_r());
    std::vector<int> Matrix(rows * cols);

    for (int i = 0; i < rows * cols; i++)
        Matrix.at(i) = random() % 100;

    return Matrix;
}

std::vector<int> GetSumCols(const std::vector<int>& matrix, const int rows, const int cols) {
    std::vector<int> Sum(cols);
    for (int i = 0; i < cols; i++)
        Sum.at(i) = 0;

    for (int j = 0; j < rows * cols; j = j + cols)
        for (int i = 0; i < cols; i++)
            Sum.at(i) += matrix.at(i + j);

    return Sum;
}

std::vector<int> GetSumColsParallel(const std::vector<int>& matrix, const int rows, const int cols) {
    int Rows = 0, Cols = 0, ProcCount, ProcId;
    std::vector<int> Matrix;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcCount);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcId);

    if (ProcId == 0) {
        int np = 0;
        if (rows % ProcCount != 0)
            np = (rows / ProcCount + 1) * ProcCount - rows;

        Matrix = std::vector<int>((rows + np) * cols);

        for (int i = 0; i < rows * cols; i++)
            Matrix.at(i) = matrix.at(i);
        for (int i = rows * cols; i < (rows + np) * cols; i++)
            Matrix.at(i) = 0;

        Rows = rows + np;
        Cols = cols;
    }
    MPI_Bcast(&Rows, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&Cols, 1, MPI_INT, 0, MPI_COMM_WORLD);

    Rows = Rows / ProcCount;
    std::vector<int> Buffer = std::vector<int>(Rows * Cols);

    MPI_Scatter(Matrix.data(), Rows * Cols, MPI_INT, Buffer.data(), Rows * Cols, MPI_INT, 0, MPI_COMM_WORLD);
    std::vector<int>Sum(Cols);
    std::vector<int> tmpSum = GetSumCols(Buffer, Rows, Cols);
    MPI_Reduce(tmpSum.data(), Sum.data(), Cols, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    return Sum;
}
