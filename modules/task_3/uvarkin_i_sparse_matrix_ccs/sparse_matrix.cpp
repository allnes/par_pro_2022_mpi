// Copyright 2022 Uvarkin Ilya
#include "../../../modules/task_3/uvarkin_i_sparse_matrix_ccs/sparse_matrix.h"
#include <mpi.h>
#include <vector>

CCSMatrix getCCSMatrix(const std::vector<std::vector<double>>& matrix) {
    CCSMatrix ccsMatrix;

    ccsMatrix.rows = matrix.size();
    ccsMatrix.columns = matrix[0].size();
    ccsMatrix.non_zero = 0;

    int end = 0;
    ccsMatrix.points.push_back(0);
    for (int i = 0; i < ccsMatrix.columns; ++i) {
        for (int j = 0; j < ccsMatrix.rows; ++j) {
            if (matrix[j][i] == 0) {
                continue;
            }

            ccsMatrix.value.push_back(matrix[j][i]);
            ccsMatrix.rowIndex.push_back(j);
            end++;
            ccsMatrix.non_zero++;
        }

        ccsMatrix.points.push_back(end);
    }

    return ccsMatrix;
}

std::vector<double> Multiply(const CCSMatrix& matrixA, const CCSMatrix& matrixB) {
    std::vector<double> res(matrixA.rows * matrixB.columns, 0);

    for (int columnA = 0; columnA < matrixA.columns; columnA++) {
        for (int columnB = 0; columnB < matrixB.columns; columnB++) {
            for (int i = matrixA.points[columnA]; i <= matrixA.points[columnA + 1] - 1; i++) {
                if (matrixB.points[columnB] - matrixB.points[columnB + 1] != 0) {
                    for (int j = matrixB.points[columnB]; j < matrixB.points[columnB + 1]; j++) {
                        if (matrixB.rowIndex[j] == columnA) {
                            int index = matrixA.rowIndex[i] * matrixB.columns + columnB;
                            res[index] += matrixA.value[i] * matrixB.value[j];
                        }
                    }
                }
            }
        }
    }

    return res;
}

std::vector<double> MultiplyParallel(CCSMatrix matrixA, CCSMatrix matrixB) {
    int ProcRank;
    int ProcNum;

    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);

    if (ProcNum == 1) {
        return Multiply(matrixA, matrixB);
    }

    MPI_Bcast(&matrixA.columns, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&matrixA.rows, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&matrixA.non_zero, 1, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Bcast(&matrixB.columns, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&matrixB.rows, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&matrixB.non_zero, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (matrixA.non_zero == 0 || matrixB.non_zero == 0) {
        if (ProcRank == 0) {
            return Multiply(matrixA, matrixB);
        } else {
            return std::vector<double>();
        }
    }

    if (matrixA.columns < ProcNum) {
        if (ProcRank == 0) {
            return Multiply(matrixA, matrixB);
        } else {
            return std::vector<double>();
        }
    }

    if (ProcRank != 0) {
        growVectorsMatrix(&matrixA);
        growVectorsMatrix(&matrixB);
    }

    SendDataMatrix(&matrixA);
    SendDataMatrix(&matrixB);

    int count = matrixA.columns / ProcNum;

    int leftBound = ProcRank * count;
    int rightBound = (ProcRank + 1) * count;

    if (ProcRank == ProcNum - 1) {
        rightBound = matrixA.columns;
    }

    std::vector<double> localResult(matrixA.rows * matrixB.columns);

    for (int a_col = leftBound; a_col < rightBound; a_col++) {
        for (int b_col = 0; b_col < matrixB.columns; b_col++) {
            for (int i = matrixA.points[a_col]; i <= matrixA.points[a_col + 1] - 1; i++) {
                if (matrixB.points[b_col + 1] - matrixB.points[b_col] != 0) {
                    for (int j = matrixB.points[b_col];
                         j <= matrixB.points[b_col + 1] - 1; j++) {
                        if (matrixB.rowIndex[j] == a_col) {
                            localResult[matrixA.rowIndex[i] * matrixB.columns + b_col]
                                    += matrixA.value[i] * matrixB.value[j];
                        }
                    }
                }
            }
        }
    }

    std::vector<double> resMatrix;
    if (ProcRank == 0) {
        resMatrix.resize(matrixA.rows * matrixB.columns);
    }

    if (ProcRank == 0) {
        MPI_Reduce(&localResult[0], &resMatrix[0], matrixA.rows * matrixB.columns,
                   MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    } else {
        MPI_Reduce(&localResult[0], MPI_IN_PLACE, matrixA.rows * matrixB.columns,
                   MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    }

    return resMatrix;
}

void setMetaToMatrix(CCSMatrix* matrix) {
    int *columnPoint = nullptr, *rowsPoint = nullptr,  *non_zeroPoint = nullptr;

    MPI_Bcast(columnPoint, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(rowsPoint, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(non_zeroPoint, 1, MPI_INT, 0, MPI_COMM_WORLD);

    matrix->columns = *columnPoint;
    matrix->rows = *rowsPoint;
    matrix->non_zero = *non_zeroPoint;
}

void growVectorsMatrix(CCSMatrix* matrix) {
    matrix->value.resize(matrix->non_zero);
    matrix->rowIndex.resize(matrix->non_zero);
    matrix->points.resize(matrix->columns + 1);
}

void SendDataMatrix(CCSMatrix* matrix) {
    MPI_Bcast(&matrix->value[0], matrix->non_zero, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&matrix->rowIndex[0], matrix->non_zero, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&matrix->points[0], matrix->columns + 1, MPI_INT, 0, MPI_COMM_WORLD);
}
