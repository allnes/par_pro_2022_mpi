  // Copyright 2022 Mironov Nikita
#include "../../../modules/task_2/mironov_n_gaussian_method_vertical/gaussian_method_vertical.h"

static int offset = 0;
static const double tolerance = 1e-8;

bool checkSolution(const std::vector <double> &coefs, size_t rows,
                        size_t columns, const std::vector <double> &xAns) {
    if (rows * columns != coefs.size()) {
        throw "Matrix sizes does not match";
    }
    if (rows + 1 != columns) {
        throw "Incorrect amount of rows and columns";
    }

    std::vector <double> result(rows, 0);
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < rows; ++j) {
            result[i] += coefs[i * columns + j] * xAns[j];
        }
    }
    for (size_t i = 0; i < rows; ++i) {
        if (result[i] - coefs[i * columns + rows] > tolerance) {
            return false;
        }
    }
    return true;
}

bool checkEqualOfMatrix(const std::vector <double> &firstMatrix,
                                const std::vector <double> &secondMatrix) {
    if (firstMatrix.size() != secondMatrix.size()) {
        return false;
    }
    for (size_t i = 0; i < firstMatrix.size(); ++i) {
        if (std::abs(firstMatrix[i] - secondMatrix[i]) > tolerance) {
            return false;
        }
    }
    return true;
}

std::vector <double> nonParSolution(const std::vector <double> &coefs,
                                    size_t rows, size_t columns) {
    if (rows * columns != coefs.size()) {
        throw "Matrix sizes does not match";
    }
    if (rows + 1 != columns) {
        throw "Incorrect amount of rows and columns";
    }

    std::vector <double> result(rows);
    std::vector <double> b(coefs);
    for (size_t k = 0; k < rows; ++k) {
        for (size_t i = k; i < rows; ++i) {
            double temp = b[i * columns + k];
            for (size_t j = 0; j < columns; ++j)
                b[i * columns + j] /= temp;
            if (i != k) {
                for (size_t j = 0; j < columns; ++j) {
                    b[i * columns + j] -= b[k * columns + j];
                }
            }
        }
    }

    for (int k = static_cast<int>(rows) - 1; k >= 0; --k) {
        result[k] = b[k * columns + columns - 1];
        for (int i = 0; i < k; i++) {
            b[i * columns + columns - 1] -= b[i * columns + k] * result[k];
        }
    }
    return result;
}

std::vector <double> ParSolution(const std::vector <double> &coefs,
                                        size_t rows, size_t columns) {
    int procCount, procId;
    MPI_Comm_size(MPI_COMM_WORLD, &procCount);
    MPI_Comm_rank(MPI_COMM_WORLD, &procId);
    const int oneProcWorkAmount = columns / procCount;
    const int nonDistWork = columns % procCount;

    int errorCode = 0;

    if (rows * columns != coefs.size()) {
        errorCode = 1;
    }
    MPI_Bcast(&errorCode, 1, MPI_INT, 0, MPI_COMM_WORLD);
    if (errorCode != 0) {
        throw "errorCode = 1 : Incorrect matrix sizes";
    }

    if (rows + 1 != columns) {
        errorCode = 2;
    }
    MPI_Bcast(&errorCode, 1, MPI_INT, 0, MPI_COMM_WORLD);
    if (errorCode != 0) {
        throw "errorCode = 2 : Incorrect amount of rows and columns";
    }

    std::vector <double> oneProcVect((oneProcWorkAmount +
                        (procId < nonDistWork ? 1 : 0)) * rows);

    if (procId == 0) {
        for (int proc = procCount - 1; proc >= 0; --proc) {
            int index = 0;
            for (size_t j = proc; j < columns; j += procCount) {
                for (size_t i = 0; i < rows; ++i) {
                    oneProcVect[index++] = coefs[i * columns + j];
                }
            }
            if (proc > 0) {
                MPI_Send(oneProcVect.data(), index, MPI_DOUBLE,
                        proc, 1, MPI_COMM_WORLD);
            }
        }
    } else {
        MPI_Status status;
        MPI_Recv(oneProcVect.data(), oneProcVect.size(),
                MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, &status);
    }

    std::vector <double> pivotColumn(rows);
    for (size_t row = 0; row < rows; ++row) {
        if (static_cast<int>(row) % procCount == procId) {
            int index = 0;
            for (size_t i = rows * (row / procCount);
                i < rows * (row / procCount + 1); ++i) {
                pivotColumn[index++] = oneProcVect[i];
            }
        }
        MPI_Bcast(pivotColumn.data(), rows, MPI_DOUBLE,
                    row % procCount, MPI_COMM_WORLD);
        double pivotRow = pivotColumn[row];
        for (int j = row / procCount; j < (oneProcWorkAmount +
            (procId < nonDistWork ? 1 : 0)); ++j) {
            double pivotC = oneProcVect[j * rows + row];
            for (size_t k = 0; k < rows; ++k) {
                if (k == row) {
                    oneProcVect[j * rows + k] /= pivotRow;
                } else {
                    oneProcVect[j * rows + k] -=
                        pivotC * pivotColumn[k] / pivotRow;
                }
            }
        }
    }

    if ((columns - 1) % procCount ==
        static_cast<size_t>(procId)) {
        MPI_Request request;
        MPI_Isend(oneProcVect.data() + ((columns - 1) / procCount) *
                rows, rows, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD, &request);
    }
    if (procId == 0) {
        oneProcVect.resize(rows);
        MPI_Status status;
        MPI_Recv(oneProcVect.data(), rows, MPI_DOUBLE,
                (columns - 1) % procCount, 2, MPI_COMM_WORLD, &status);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    return oneProcVect;
}
