// Copyright 2022 Makarov Danila
#include "../../../modules/task_3/makarov_d_marking_on_binary_image/marking_on_binary_image.h"

#include <mpi.h>

#include <iomanip>
#include <iostream>
#include <list>
#include <queue>
#include <set>
#include <vector>

std::vector<int> preset1() {
  const int rows = 13, colls = 15;
  int matr[rows][colls] = {{1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1},
                           {0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1},
                           {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1},
                           {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1},
                           {1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1},
                           {1, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 1},
                           {1, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1},
                           {1, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1},
                           {1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0},
                           {0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1},
                           {1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 1},
                           {1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1},
                           {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0}};

  std::vector<int> vec(rows * colls);
  for (int i = 0; i < rows; i++)
    for (int j = 0; j < colls; j++) {
      vec[i * colls + j] = matr[i][j];
    }
  vec.push_back(rows);
  vec.push_back(colls);
  return vec;
}

std::vector<int> preset2() {
  const int rows = 15, colls = 22;
  int matr2[rows][colls] = {
      {0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1},
      {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
      {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
      {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
      {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
      {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
      {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
      {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
      {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
      {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
      {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
      {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
      {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
      {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
      {0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1}};

  std::vector<int> vec(rows * colls);
  for (int i = 0; i < rows; i++)
    for (int j = 0; j < colls; j++) {
      vec[i * colls + j] = matr2[i][j];
    }
  vec.push_back(rows);
  vec.push_back(colls);
  return vec;
}

std::vector<int> preset3() {
  const int rows = 50, colls = 70;
  int matr2[rows][colls] = {0};

  for (int i = 1; i < 15; i++)
    for (int j = 1; j < 15; j++) matr2[i][j] = 1;

  for (int i = 1; i < 12; i++)
    for (int j = 30; j < 48; j++) matr2[i][j] = 1;

  for (int i = 25; i < 45; i++)
    for (int j = 3; j < 48; j++) matr2[i][j] = 1;

  for (int i = 30; i < 49; i++)
    for (int j = 50; j < 69; j++) matr2[i][j] = 1;

  std::vector<int> vec(rows * colls);
  for (int i = 0; i < rows; i++)
    for (int j = 0; j < colls; j++) {
      vec[i * colls + j] = matr2[i][j];
    }
  vec.push_back(rows);
  vec.push_back(colls);
  return vec;
}

std::vector<int> preset4() {
  const int rows = 50, colls = 50;
  int matr2[rows][colls];

  for (int i = 0; i < rows; i++)
    for (int j = 0; j < colls; j++) matr2[i][j] = 1;

  std::vector<int> vec(rows * colls);
  for (int i = 0; i < rows; i++)
    for (int j = 0; j < colls; j++) {
      vec[i * colls + j] = matr2[i][j];
    }
  vec.push_back(rows);
  vec.push_back(colls);
  return vec;
}

std::vector<int> preset5() {
  const int rows = 50, colls = 50;
  int matr2[rows][colls] = {0};

  std::vector<int> vec(rows * colls);
  for (int i = 0; i < rows; i++)
    for (int j = 0; j < colls; j++) {
      vec[i * colls + j] = matr2[i][j];
    }
  vec.push_back(rows);
  vec.push_back(colls);
  return vec;
}

std::vector<int> marking(const std::vector<int> &vecInput, int rows, int colls) {
  std::vector<int> vec = vecInput;
  int mark = 1;

  for (int i = 0; i < rows * colls; i++) {
    if (i % colls != 0 && vec[i - 1] != 0 && vec[i] == 1) {
      vec[i] = vec[i - 1];
    } else if (i - colls > 0 && vec[i - colls] != 0 && vec[i] == 1) {
      vec[i] = vec[i - colls];
    } else if (vec[i] == 1) {
      vec[i] = mark;
      mark++;
    }
  }

  std::vector<std::vector<int>> pairs;

  for (int i = 0; i < rows * colls; i++) {
    if (i - colls > 0 && vec[i - colls] != 0 && vec[i] != 0 &&
        vec[i] != vec[i - colls])
      pairs.push_back({vec[i], vec[i - colls]});
  }

  std::vector<std::set<int>> totalMarks;

  for (int i = 0; i < pairs.size(); i++) {
    int coinc = -1;

    for (int j = 0; j < totalMarks.size(); j++) {
      if (totalMarks[j].count(pairs[i][0]) >= 1 ||
          totalMarks[j].count(pairs[i][1]) >= 1) {
        if (coinc != -1) {
          totalMarks[j].insert(totalMarks[coinc].begin(),
                               totalMarks[coinc].end());
          totalMarks[coinc].erase(totalMarks[coinc].begin(),
                                  totalMarks[coinc].end());
          coinc = j;
        }

        totalMarks[j].insert({pairs[i][0], pairs[i][1]});
        coinc = j;
      }
    }

    if (coinc == -1) {
      totalMarks.push_back({pairs[i][0], pairs[i][1]});
    }
  }

  std::vector<int> vec2 = vec;

  for (int i = 0; i < totalMarks.size(); i++) {
    for (int j = 0; j < rows * colls; j++) {
      if (totalMarks[i].count(vec[j])) {
        vec2[j] = *totalMarks[i].begin();
      }
    }
  }

  std::set<int> elems;
  for (int i = 0; i < rows * colls; i++) {
    elems.insert(vec2[i]);
  }
  if (elems.count(0) >= 1) elems.erase(elems.begin());

  std::set<int>::iterator iter = elems.begin();
  for (int i = 1; i < elems.size() + 1; i++) {
    for (int j = 0; j < rows * colls; j++) {
      if (*iter == vec2[j]) vec2[j] = i;
    }
    ++iter;
  }

  return vec2;
}

std::vector<int> rowsDistrib(std::vector<int> vecInput, int rows, int colls) {
  int ProcRank, ProcNum;

  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

  int localrows = rows / (ProcNum - 1);

  if (ProcRank == ProcNum - 1 && rows % (ProcNum - 1) != 0)
    localrows += rows % (ProcNum - 1);

  std::vector<int> localvec;

  int localstart, localend;
  if (ProcRank == ProcNum - 1 && rows % (ProcNum - 1) != 0)
    localstart = (localrows - rows % (ProcNum - 1)) * colls * (ProcRank - 1);
  else if (ProcRank != 0)
    localstart = localrows * colls * (ProcRank - 1);

  if (ProcRank != 0) localend = localstart + localrows * colls;

  if (ProcRank != 0) {
    for (int i = localstart; i < localend; i++) {
      localvec.push_back(vecInput[i]);
    }
  }

  int temp = localrows * colls;
  int temp2 = temp + (rows % (ProcNum - 1)) * colls;
  std::vector<int> tempvec;
  MPI_Status status;
  std::vector<int> res(temp);
  std::vector<int> res2(temp2);

  int mark = (ProcRank - 1) * (colls * localrows / 2) + 1;

  if (ProcRank != 0) {
    for (int i = 0; i < localvec.size(); i++) {
      if (i % colls != 0 && localvec[i - 1] != 0 && localvec[i] == 1) {
        localvec[i] = localvec[i - 1];
      } else if (i - colls >= 0 && localvec[i - colls] != 0 &&
                 localvec[i] == 1) {
        localvec[i] = localvec[i - colls];
      } else if (localvec[i] == 1) {
        localvec[i] = mark;
        mark++;
      }
    }
  }

  if (ProcRank != 0) {
    MPI_Send(&localvec.front(), localvec.size(), MPI_INT, 0, 0, MPI_COMM_WORLD);
  }

  if (ProcRank == 0) {
    if (rows % (ProcNum - 1) == 0) {
      for (int i = 1; i < ProcNum; i++) {
        MPI_Recv(&res.front(), temp, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
        tempvec.insert(tempvec.end(), res.begin(), res.end());
      }
    } else if (rows % (ProcNum - 1) != 0) {
      for (int i = 1; i < ProcNum - 1; i++) {
        MPI_Recv(&res.front(), temp, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
        tempvec.insert(tempvec.end(), res.begin(), res.end());
      }
      temp += (rows % (ProcNum - 1)) * colls;
      MPI_Recv(&res2.front(), temp2, MPI_INT, ProcNum - 1, 0, MPI_COMM_WORLD,
               &status);
      tempvec.insert(tempvec.end(), res2.begin(), res2.end());
    }
  }

  if (ProcRank == 0) {
    return markingMPI(tempvec, localrows, rows, colls, ProcRank);
  }

  return {0};
}

std::vector<int> markingMPI(const std::vector<int> &vecInput, int locrows,
                            int rows,
                            int colls, int rank) {
  std::vector<int> vec = vecInput;

  std::vector<std::vector<int>> pairs;

  for (int i = 0; i < rows * colls; i++) {
    if (i - colls > 0 && vec[i - colls] != 0 && vec[i] != 0 &&
        vec[i] != vec[i - colls])
      pairs.push_back({vec[i], vec[i - colls]});
  }

  std::vector<std::set<int>> totalMarks;

  for (int i = 0; i < pairs.size(); i++) {
    int coinc = -1;

    for (int j = 0; j < totalMarks.size(); j++) {
      if (totalMarks[j].count(pairs[i][0]) >= 1 ||
          totalMarks[j].count(pairs[i][1]) >= 1) {
        if (coinc != -1) {
          totalMarks[j].insert(totalMarks[coinc].begin(),
                               totalMarks[coinc].end());
          totalMarks[coinc].erase(totalMarks[coinc].begin(),
                                  totalMarks[coinc].end());
          coinc = j;
        }
        totalMarks[j].insert({pairs[i][0], pairs[i][1]});
        coinc = j;
      }
    }

    if (coinc == -1) totalMarks.push_back({pairs[i][0], pairs[i][1]});
  }

  std::vector<int> vec2 = vec;
  for (int i = 0; i < totalMarks.size(); i++) {
    for (int j = 0; j < rows * colls; j++) {
      if (totalMarks[i].count(vec[j])) vec2[j] = *totalMarks[i].begin();
    }
  }

  std::set<int> elems;
  for (int i = 0; i < rows * colls; i++) {
    elems.insert(vec2[i]);
  }
  if (elems.count(0) >= 1) elems.erase(elems.begin());

  std::set<int>::iterator iter = elems.begin();
  for (int i = 1; i < elems.size() + 1; i++) {
    for (int j = 0; j < rows * colls; j++) {
      if (*iter == vec2[j]) vec2[j] = i;
    }
    ++iter;
  }

  return vec2;
}
