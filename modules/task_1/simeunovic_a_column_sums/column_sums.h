// Copyright 2022 Aleksandar Simeunovic
#ifndef MODULES_TASK_1_SIMEUNOVIC_A_COLUMN_SUMS_COLUMN_SUMS_H_
#define MODULES_TASK_1_SIMEUNOVIC_A_COLUMN_SUMS_COLUMN_SUMS_H_
#include<vector>
void DoWork(std::vector<int>* a, std::vector<int>* b);
void CreateRandomMatrix(std::vector<int>* matrix, int row_num, int column_num);
std::vector<int>SequencallSum(std::vector<int>* matrix, int row_num, int column_num);
void ColumnSumsParallel(int ProcRank, int ProcSize, std::vector<int>* pSendInd, std::vector<int>* pSendNum
, std::vector<int>* result, const std::vector<int>& pProcColumns
, std::vector<int>* pProcResults, int row_num, int column_num, int ColumnNum);
void ColumnSumsSequenceally(int ProcRank, int ProcSize, const std::vector<int>* pProcColumns
, std::vector<int>* pProcResults, int row_num, int ColumnNum);
void ProcessInitialization(int ProcRank, int ProcSize, std::vector<int>* matrix, std::vector<int>* results
, std::vector<int>* pProcColumns, std::vector<int>* pProcResults, int* row_num, int* column_num, int* ColumnNum);
void DataDistribution(int ProcSize, int ProcRank, std::vector<int>* pSendInd, std::vector<int>* pSendNum
, const std::vector<int>& matrix, std::vector<int>* pProcColumns, int row_num, int column_num);
void PrintMatrix(const std::vector<int>& matrix, int row_num, int column_num);
#endif  //  MODULES_TASK_1_SIMEUNOVIC_A_COLUMN_SUMS_COLUMN_SUMS_H_
