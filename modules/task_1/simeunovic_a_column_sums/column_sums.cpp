// Copyright 2022 Aleksandar Simeunovic
#include"../../../modules/task_1/simeunovic_a_column_sums/column_sums.h"
#include<stdlib.h>
#include<mpi.h>
#include<iostream>
#include<vector>
#include<random>
#include<utility>
void ProcessInitialization(int ProcRank, int ProcSize, std::vector<int>* matrix, std::vector<int>* results
, std::vector<int>* pProcColumns, std::vector<int>* pProcResults, int* row_num, int* column_num, int* ColumnNum) {
    std::pair<int, int>par;
    std::random_device dev;
    std::mt19937 rand_r(dev());
    if (ProcRank == 0) {
        *row_num = rand_r() % 10 + 1;
        *column_num = rand_r() % 10 + ProcSize;
        par.first = *row_num;
        par.second = *column_num;
    }
    MPI_Bcast(&par, 1, MPI_2INT, 0, MPI_COMM_WORLD);
    if (ProcRank != 0) { *row_num = par.first; *column_num = par.second; }
    int RestColumns = *column_num;
    for (int i = 0; i < ProcRank; i++) {
         RestColumns = RestColumns - RestColumns / (ProcSize - i);
     }
    *ColumnNum = RestColumns / (ProcSize - ProcRank);
    (*pProcColumns).resize(*ColumnNum * *row_num);
    (*pProcResults).resize(*ColumnNum);
    if (ProcRank == 0) {
        (*matrix).resize(*row_num * *column_num);
        (*results).resize(*column_num);
        CreateRandomMatrix(matrix, *row_num, *column_num);
        // PrintMatrix(*matrix, *row_num, *column_num);
    }
}
void PrintMatrix(const std::vector<int>& matrix, int row_num, int column_num) {
    std::cout << "Row number:" << row_num << std::endl;
    std::cout << "Column number:" << column_num << std::endl;
    std::cout << "Matrix:" << std::endl;
    for (int i = 0; i < row_num; i++) {
        for (int j = 0; j < column_num; j++) {
             std::cout << matrix[i * column_num + j] << " ";
        }
        std::cout << std::endl;
    }
}
void PrintVector(const std::vector<int>& matrix, int size) {
    std::cout << "Result Vector:" << std::endl;
    for (int i = 0; i < size; i++) {
        std::cout << matrix[i] << " ";
    }
    std::cout << std::endl;
}
void DataDistribution(int ProcSize, int ProcRank, std::vector<int>* pSendInd, std::vector<int>* pSendNum
, const std::vector<int>& matrix, std::vector<int>* pProcColumns, int row_num, int column_num) {
    (*pSendInd).resize(ProcSize);
    (*pSendNum).resize(ProcSize);
    int RestColumns = column_num;
    int ColumnNum = column_num / ProcSize;
    (*pSendNum)[0] = ColumnNum * row_num;
    (*pSendInd)[0] = 0;
    for (int i = 1; i < ProcSize; i++) {
        RestColumns -= ColumnNum;
        ColumnNum = RestColumns / (ProcSize - i);
        (*pSendNum)[i] = ColumnNum * row_num;
        (*pSendInd)[i] = (*pSendInd)[i - 1] + (*pSendNum)[i - 1];
    }
    std::vector<int>matrix_transport;
    if (ProcRank == 0) {
        for (int i = 0; i < column_num; i++) {
            for (int j = 0; j < row_num; j++) {
                 matrix_transport.push_back(matrix[i + j * column_num]);
            }
        }
        // PrintVector(matrix_transport, row_num * column_num);
    }
    MPI_Scatterv(matrix_transport.data(), (*pSendNum).data(), (*pSendInd).data()
    , MPI_INT, (*pProcColumns).data(), (*pSendNum)[ProcRank], MPI_INT, 0, MPI_COMM_WORLD);
}
void CreateRandomMatrix(std::vector<int>* matrix, int row_num, int column_num) {
    std::random_device dev;
    std::mt19937 rand_r(dev());
    for (int i = 0; i < row_num; i++) {
        for (int j = 0; j < column_num; j++) {
           (*matrix)[i * column_num + j] = rand_r() % 10;
        }
    }
}
void ColumnSumsSequenceally(int ProcRank, int ProcSize, const std::vector<int>* pProcColumns
, std::vector<int>* pProcResults, int row_num, int ColumnNum) {
    for (int i = 0; i < ColumnNum; i++) {
        int sum = 0;
        for (int j = 0; j < row_num; j++) {
            sum += (*pProcColumns)[i * row_num + j];
        }
        (*pProcResults)[i] = sum;
    }
}
void ColumnSumsParallel(int ProcRank, int ProcSize, std::vector<int>* pSendInd, std::vector<int>* pSendNum
, std::vector<int>* result, const std::vector<int>& pProcColumns
, std::vector<int>* pProcResults, int row_num, int column_num, int ColumnNum) {
    ColumnSumsSequenceally(ProcRank, ProcSize, &pProcColumns, pProcResults, row_num, ColumnNum);
    int RestColumns = column_num;
    int SendColumns = column_num / ProcSize;
    (*pSendNum)[0] = SendColumns;
    (*pSendInd)[0] = 0;
    for (int i = 1; i < ProcSize; i++) {
        RestColumns -= SendColumns;
        SendColumns = RestColumns / (ProcSize - i);
        (*pSendNum)[i] = SendColumns;
        (*pSendInd)[i] = (*pSendInd)[i - 1] + (*pSendNum)[i - 1];
    }
    MPI_Gatherv((*pProcResults).data(), (*pSendNum)[ProcRank], MPI_INT, (*result).data(), (*pSendNum).data()
    , (*pSendInd).data(), MPI_INT, 0, MPI_COMM_WORLD);
}
std::vector<int>SequencallSum(std::vector<int>* matrix, int row_num, int column_num) {
    std::vector<int>result;
    for (int i = 0; i < column_num; i++) {
        int sum = 0;
        for (int j = 0; j < row_num; j++) {
             sum +=  (*matrix)[j * column_num + i];
        }
        result.push_back(sum);
    }
    return result;
}
void DoWork(std::vector<int>* a, std::vector<int>* b) {
    int ProcRank, ProcSize, row_num, column_num, ColumnNum;
    std::vector<int>matrix, result, pProcColumns, pProcResults, pSendNum, pSendInd;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcSize);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    ProcessInitialization(ProcRank, ProcSize, &matrix, &result
    , &pProcColumns, &pProcResults, &row_num, &column_num, &ColumnNum);
    DataDistribution(ProcSize, ProcRank, &pSendInd, &pSendNum, matrix, &pProcColumns, row_num, column_num);
    ColumnSumsParallel(ProcRank, ProcSize, &pSendInd
    , &pSendNum, &result, pProcColumns, &pProcResults, row_num, column_num, ColumnNum);
    if (ProcRank == 0) {
        // PrintVector(result, column_num);
        std::vector<int> sequental = SequencallSum(&matrix, row_num, column_num);
        *a = sequental;
        *b = result;
    }
}
