// Copyright 2022 Voronov Alexander
#include <mpi.h>
#include <random>
#include <algorithm>
#include <vector>
#include "../../../modules/task_1/voronov_a_min_column_matrix/min_column_matrix.h"

std::vector<int> GetRandomMatrix(int rows, int columns) {
    if (rows < 1 || columns < 1)
        throw - 1;

    std::vector <int> matrix;

    matrix.resize(rows*columns);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution <int> dist(-100, 100);

    for (int i = 0; i < rows*columns; i++)
            matrix[i] = dist(gen);

    return matrix;
}

std::vector <int> GetTransposeMatrix(std::vector <int> Matrix, int rows, int columns) {
    if (rows < 1 || columns < 1)
        throw - 1;

    if (rows*columns != static_cast<int>(Matrix.size()))
        throw - 1;

    std::vector <int> tr_matrix(rows*columns);

    int k = -1;
    for (int i = 0, j = 0; i < columns*rows; i++, j = (j+1) % columns) {
        if (i % columns == 0)
            k++;
        tr_matrix[i] = Matrix[(j*columns) + k];
    }

    return tr_matrix;
}

std::vector <int> GetSequentialMinValueColumn(std::vector <int> Matrix, int rows, int columns) {
    if (rows < 1 || columns < 1)
        throw - 1;

    if (rows*columns != static_cast<int>(Matrix.size()))
        throw - 1;

    std::vector <int> result(columns);

    std::vector <int> tr_matrix = GetTransposeMatrix(Matrix, rows, columns);

    for (int i = 0; i < columns; i++)
        result[i] = *min_element(tr_matrix.begin() + i * rows, tr_matrix.begin() + (i + 1) * rows);

    return result;
}

std::vector <int> GetParallelMinValueColumn(std::vector <int> Matrix, int rows, int columns) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int Error = 0;
    if (rank == 0) {
        if (rows*columns != static_cast<int>(Matrix.size())) {
            Error = 1;
        }
        for (int i = 1; i < size; i++)
            MPI_Send(&Error, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
    } else {
        MPI_Status status;
        MPI_Recv(&Error, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
    }

    if (Error == 1)
        throw - 1;

    const int delta = columns / size;
    const int delta_rem = columns % size;

    std::vector <int> result;

    std::vector <int> local_columns(delta*rows);
    std::vector <int> local_columns0;

    if (rank == 0) {
        std::vector<int> tr_matrix;
        tr_matrix = GetTransposeMatrix(Matrix, rows, columns);

        if (delta > 0) {
            for (int i = 0; i < size - 1; i++)
                MPI_Send(&tr_matrix[(delta+delta_rem)*rows] + delta * rows * i,
                                    delta*rows, MPI_INT, i + 1, 2, MPI_COMM_WORLD);
        }
        local_columns0 = std::vector<int>(tr_matrix.begin(), tr_matrix.begin() + (delta + delta_rem)*rows);
    } else {
        MPI_Status status;
        if (delta > 0)
            MPI_Recv(&local_columns[0], delta*rows, MPI_INT, 0, 2, MPI_COMM_WORLD, &status);
    }

    if (rank == 0) {
        for (int i = 0; i < delta_rem + delta; i++)
            result.push_back(*min_element(local_columns0.begin() + i * rows,
                                        local_columns0.begin() + (i + 1) * rows));

        if (delta > 0) {
            std::vector <int> local_result(delta);
            for (int i = 1; i < size; i++) {
                MPI_Status status;
                MPI_Recv(&local_result[0], delta, MPI_INT, i, 3, MPI_COMM_WORLD, &status);
                result.insert(result.end(), local_result.begin(), local_result.end());
            }
        }
    } else {
        if (delta > 0) {
            std::vector <int> local_result(delta);
            for (int i = 0; i < delta; i++) {
                local_result[i] = *min_element(local_columns.begin() + i * rows,
                    local_columns.begin() + (i + 1) * rows);
            }
            MPI_Send(&local_result[0], delta, MPI_INT, 0, 3, MPI_COMM_WORLD);
        }
    }

    return result;
}


