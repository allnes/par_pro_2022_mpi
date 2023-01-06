// Copyright 2022 Makarov Mikhail
#ifndef MODULES_TASK_3_MAKAROV_M_SPARSE_MATRIX_COL_SPARSE_MATRIX_COL_H_
#define MODULES_TASK_3_MAKAROV_M_SPARSE_MATRIX_COL_SPARSE_MATRIX_COL_H_

#include <vector>

struct ComprColStorage {
  int countRow;
  int countCol;
  std::vector<double> value;
  std::vector<int> startPos;
  std::vector<int> numberRow;

  bool operator==(const ComprColStorage &sec) const;
  bool operator!=(const ComprColStorage &sec) const;
};

ComprColStorage GenerateMatrix(int countRow, int countCol, double proc);
ComprColStorage GetSequentialSolution(ComprColStorage *first,
                                      ComprColStorage *second);
ComprColStorage GetParalSolution(ComprColStorage *first,
                                 ComprColStorage *second);
double Sum(const ComprColStorage &first, const ComprColStorage &second,
           int curRow, int curCol, int size);
void PrintMatr(const ComprColStorage &matr);
#endif  // MODULES_TASK_3_MAKAROV_M_SPARSE_MATRIX_COL_SPARSE_MATRIX_COL_H_
