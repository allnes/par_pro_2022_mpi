// Copyright 2022 Makarov Mikhail
#include "../../modules/task_3/makarov_m_sparse_matrix_col/sparse_matrix_col.h"
#include <mpi.h>
#include <algorithm>
#include <cmath>
#include <random>
#include <iostream>
#include <vector>

ComprColStorage GenerateMatrix(int countRow, int countCol, double proc) {
  int countElem = std::ceil(countRow * countCol * proc / 100);
  ComprColStorage Matr{countRow, countCol, std::vector<double>(countElem, 0),
                       std::vector<int>(countCol + 1, 0),
                       std::vector<int>(countElem, -1)};

  std::random_device dev;
  std::mt19937 ger(dev());
  std::uniform_real_distribution<> realDist(-100, 100);
  std::uniform_int_distribution<> intColDist(0, countCol - 1);
  std::uniform_int_distribution<> intRowDist(0, countRow - 1);

  int curElem = 0;
  while (curElem < countElem) {
    double elem = realDist(ger);
    if (elem != 0) {
      Matr.value[curElem] = elem;
      ++curElem;
    }
  }

  curElem = 0;
  while (curElem < countElem) {
    int col = intColDist(ger);
    if (Matr.startPos[col + 1] - Matr.startPos[col] < countRow) {
      for (int j = col + 1; j < countCol + 1; ++j) {
        ++Matr.startPos[j];
      }
      ++curElem;
    }
  }

  for (int curCol = 0; curCol < countCol; ++curCol) {
    int index = Matr.startPos[curCol];
    while (index < Matr.startPos[curCol + 1]) {
      int row = intRowDist(ger);
      bool F = true;
      for (int curPos = Matr.startPos[curCol]; curPos < index && F; ++curPos) {
        if (Matr.numberRow[curPos] == row) F = false;
      }
      if (F) {
        Matr.numberRow[index] = row;
        ++index;
      }
    }
    std::vector<int>::iterator startIt =
        Matr.numberRow.begin() + Matr.startPos[curCol];
    std::vector<int>::iterator endIt =
        Matr.numberRow.begin() + Matr.startPos[curCol + 1];
    std::sort(startIt, endIt);
  }
  MPI_Bcast(Matr.value.data(), Matr.value.size(), MPI_DOUBLE, 0,
            MPI_COMM_WORLD);
  MPI_Bcast(Matr.numberRow.data(), Matr.numberRow.size(), MPI_INT, 0,
            MPI_COMM_WORLD);
  MPI_Bcast(Matr.startPos.data(), Matr.startPos.size(), MPI_INT, 0,
            MPI_COMM_WORLD);
  return Matr;
}

ComprColStorage GetSequentialSolution(ComprColStorage* first,
                                      ComprColStorage* second) {
  if (first->countCol == 0 || first->countRow == 0 || second->countCol == 0 ||
      second->countRow == 0)
    return ComprColStorage();
  if (first->countCol != second->countRow) return ComprColStorage();
  ComprColStorage resMatr{
      first->countRow, second->countCol, std::vector<double>(),
      std::vector<int>(second->countCol + 1, 0), std::vector<int>()};
  for (int curCol = 0; curCol < resMatr.countCol; ++curCol) {
    for (int curRow = 0; curRow < resMatr.countRow; ++curRow) {
      double curSum = Sum(*first, *second, curRow, curCol, first->countCol);
      if (curSum != 0) {
        resMatr.value.push_back(curSum);
        resMatr.numberRow.push_back(curRow);
        for (size_t i = curCol + 1; i < resMatr.startPos.size(); ++i) {
          ++resMatr.startPos[i];
        }
      }
    }
  }

  return resMatr;
}

ComprColStorage GetParalSolution(ComprColStorage* first,
                                 ComprColStorage* second) {
  if (first->countCol == 0 || first->countRow == 0 || second->countCol == 0 ||
      second->countRow == 0)
    return ComprColStorage();
  if (first->countCol != second->countRow) return ComprColStorage();
  ComprColStorage resMatr{
      first->countRow, second->countCol, std::vector<double>(),
      std::vector<int>(second->countCol + 1, 0), std::vector<int>()};

  int ProcCount;
  int ProcRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  MPI_Comm_size(MPI_COMM_WORLD, &ProcCount);
  int offset = resMatr.countCol / ProcCount;
  std::vector<int> countColInProc(ProcCount, offset);

  for (int i = 0; i < resMatr.countCol % ProcCount; ++i) ++countColInProc[i];
  if (resMatr.countCol % ProcCount) ++offset;
  std::vector<int> globCol(ProcCount, 0);
  for (int i = 1; i < ProcCount; ++i) {
    globCol[i] = globCol[i - 1] + countColInProc[i - 1];
  }
  ComprColStorage locMatr{
      resMatr.countRow, countColInProc[ProcRank], std::vector<double>(),
      std::vector<int>(locMatr.countCol + 1, 0), std::vector<int>()};
  int count = 0;
  for (int curCol = 0; curCol < locMatr.countCol; ++curCol) {
    for (int curRow = 0; curRow < locMatr.countRow; ++curRow) {
      double curSum = Sum(*first, *second, curRow, globCol[ProcRank] + curCol,
                          first->countCol);
      if (curSum != 0) {
        ++count;
        locMatr.value.push_back(curSum);
        locMatr.numberRow.push_back(curRow);
        for (size_t i = curCol + 1; i < locMatr.startPos.size(); ++i)
          ++locMatr.startPos[i];
      }
    }
  }
  int locElem = locMatr.value.size();
  std::vector<int> countElem(ProcCount, 0);
  int totalCountElem;

  MPI_Allgather(&locElem, 1, MPI_INT, &countElem[0], 1, MPI_INT,
                MPI_COMM_WORLD);
  MPI_Allreduce(&locElem, &totalCountElem, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

  resMatr.numberRow = std::vector<int>(totalCountElem, 0);
  resMatr.value = std::vector<double>(totalCountElem, 0);
  std::vector<int> offsetValueAndNumRow(ProcCount, 0);
  std::vector<int> offsetStartPos(ProcCount, 0);
  for (int i = 1; i < ProcCount; ++i) {
    offsetValueAndNumRow[i] = offsetValueAndNumRow[i - 1] + countElem[i - 1];
    offsetStartPos[i] = offsetStartPos[i - 1] + countColInProc[i - 1];
  }

  MPI_Gatherv(locMatr.startPos.data(), countColInProc[ProcRank], MPI_INT,
              resMatr.startPos.data(), &countColInProc[0], &offsetStartPos[0],
              MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Gatherv(locMatr.numberRow.data(), countElem[ProcRank], MPI_INT,
              resMatr.numberRow.data(), &countElem[0], &offsetValueAndNumRow[0],
              MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Gatherv(locMatr.value.data(), countElem[ProcRank], MPI_DOUBLE,
              resMatr.value.data(), &countElem[0], &offsetValueAndNumRow[0],
              MPI_DOUBLE, 0, MPI_COMM_WORLD);

  if (ProcRank == 0) {
    int curStartPos = locMatr.startPos[countColInProc[0]];
    int curCol = countColInProc[0];
    for (int i = 1; i < ProcCount; ++i) {
      for (int j = 0; j < countColInProc[i]; ++j) {
        resMatr.startPos[curCol++] += curStartPos;
      }
      curStartPos = curStartPos + countElem[i];
    }
    resMatr.startPos[resMatr.countCol] = totalCountElem;
  }

  return resMatr;
}

double Sum(const ComprColStorage& first, const ComprColStorage& second,
           int curRow, int curCol, int size) {
  int distCol = 0;
  int startPosCol = second.startPos[curCol];
  int endPosCol = second.startPos[curCol + 1];
  double sum = 0;
  double firstV = 0;
  double secondV = 0;

  for (int i = 0; i < size; ++i) {
    if ((startPosCol + distCol < endPosCol) &&
        (second.numberRow[startPosCol + distCol] == i)) {
      secondV = second.value[startPosCol + distCol++];
    }

    for (int j = first.startPos[i]; j < first.startPos[i + 1] && firstV == 0;
         ++j) {
      if (first.numberRow[j] == curRow) {
        firstV = first.value[j];
      }
    }
    sum += firstV * secondV;
    firstV = 0;
    secondV = 0;
  }
  return sum;
}

bool ComprColStorage::operator==(const ComprColStorage& sec) const {
  if (this->countRow != sec.countRow || this->countCol != sec.countCol)
    return false;
  if (this->value.size() != sec.value.size() ||
      this->startPos.size() != sec.startPos.size() ||
      this->numberRow.size() != sec.numberRow.size())
    return false;
  for (size_t i = 0; i < this->startPos.size(); ++i) {
    if (this->startPos[i] != sec.startPos[i]) return false;
  }
  for (size_t i = 0; i < this->value.size(); ++i) {
    if (this->value[i] != sec.value[i] ||
        this->numberRow[i] != sec.numberRow[i])
      return false;
  }

  return true;
}

bool ComprColStorage::operator!=(const ComprColStorage& sec) const {
  return !(*this == sec);
}
