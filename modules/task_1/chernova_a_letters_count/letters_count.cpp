// Copyright 2022 Anna Chernova
#include <mpi.h>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <numeric>
#include <iostream>
#include "../../../modules/task_1/chernova_a_letters_count/letters_count.h"

int countLetters(string str) {
  int counter = 0;
  for (char c : str) {
    if (isalpha(c)) {
      counter++;
    }
  }
  return counter;
}

string randomStr(int size) {
  string result = "";
  std::random_device rd;
  std::mt19937 RNG(rd());
  std::uniform_int_distribution<int> dist(33, 126);

  for (int i = 0; i < size; i++) {
    result += dist(RNG);
  }

  return result;
}

int countLettersMPI(string str, int size) {
  int procNum, procRank;
  int result = 0;

  MPI_Comm_size(MPI_COMM_WORLD, &procNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &procRank);

  int sizeOfBuff = size / procNum;
  if (procRank == 0) {
    for (int i = 1; i < procNum; i++) {
      string temp = str.substr(i * sizeOfBuff, sizeOfBuff);
      MPI_Send(temp.c_str(), temp.size() + 1, MPI_CHAR, i, 0, MPI_COMM_WORLD);
    }
  }

  string local;
  if (procRank == 0) {
    local = str.substr(0, sizeOfBuff) +
            str.substr(procNum * sizeOfBuff, size % procNum + 1);
  } else {
    char* temp = new char[sizeOfBuff + 1];
    MPI_Status status;
    MPI_Recv(temp, sizeOfBuff + 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
    local = temp;
  }

  int count = countLetters(local);

  MPI_Reduce(&count, &result, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  return result;
}
