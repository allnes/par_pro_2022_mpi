// Copyright 2022 Elistratov Vladimir
#include "../../../modules/task_3/elistratov_v_betcher_sort/betcher_sort.h"

#include <mpi.h>

#include <algorithm>
#include <random>
#include <string>
#include <utility>
#include <vector>

std::vector<int> OddSpliter(std::vector<int> vec1, std::vector<int> vec2,
                            std::vector<int> res) {
  int size1 = static_cast<int>(vec1.size());
  int size2 = static_cast<int>(vec2.size());
  res.resize(size1 + size2);
  int a = 1;
  int b = 1;
  int i = 1;
  while ((a < size1) && (b < size2)) {
    if (vec1[a] <= vec2[b]) {
      res[i] = vec1[a];
      a += 2;
    } else {
      res[i] = vec2[b];
      b += 2;
    }
    i += 2;
  }
  if (a >= size1) {
    for (int j = b; j < size2; j += 2, i += 2) {
      res[i] = vec2[j];
    }
  }
  if (b >= size2) {
    for (int j = a; j < size1; j += 2, i += 2) {
      res[i] = vec1[j];
    }
  }
  return res;
}

std::vector<int> EvenSpliter(std::vector<int> vec1, std::vector<int> vec2,
                             std::vector<int> res) {
  int size1 = static_cast<int>(vec1.size());
  int size2 = static_cast<int>(vec2.size());
  res.resize(size1 + size2);
  int flag = 0;
  if ((size1 == 1) && (size2 == 1)) {
    if (vec1[0] < vec2[0]) {
      res[0] = vec1[0];
      res[1] = vec2[0];
    } else {
      res[0] = vec2[0];
      res[1] = vec1[0];
    }
    flag = 1;
  }
  if (flag == 0) {
    int a = 0;
    int b = 0;
    int i = 0;
    while ((a < size1) && (b < size2)) {
      if (vec1[a] <= vec2[b]) {
        res[i] = vec1[a];
        a += 2;
      } else {
        res[i] = vec2[b];
        b += 2;
      }
      i += 2;
    }
    if (a >= size1) {
      for (int j = b; j < size2; j += 2, i += 2) {
        if (i >= static_cast<int>(res.size())) {
          res[i - 1] = vec2[j];
        } else {
          res[i] = vec2[j];
        }
      }
    }
    if (b >= size2) {
      for (int j = a; j < size1; i += 2, j += 2) {
        if (i >= static_cast<int>(res.size())) {
          res[i - 1] = vec1[j];
        } else {
          res[i] = vec1[j];
        }
      }
    }
  }
  return res;
}

std::vector<int> SimpleComparator(std::vector<int> res, std::vector<int> even,
                                  std::vector<int> odd) {
  int size = static_cast<int>(even.size());
  res = std::vector<int>(size);
  for (int i = 0; i < static_cast<int>(odd.size()); i++) {
    if ((i % 2 != 0) && (odd[i] != 0)) {
      res[i] = odd[i];
    } else {
      res[i] = even[i];
    }
  }
  for (int i = 1; i < static_cast<int>(res.size()); i++) {
    if (res[i] < res[i - 1]) {
      std::swap(res[i], res[i - 1]);
    }
  }
  return res;
}

std::vector<int> genRand(int n) {
  std::mt19937 gen;
  gen.seed(static_cast<unsigned int>(time(0)));
  std::vector<int> vec(n);
  std::vector<int> vec1(n);
  for (int i = 0; i < n; i++) vec[i] = gen() % 10000;
  for (int i = 0; i < n; i++) vec1[i] = gen() % 100;
  return vec;
}

bool degree_2(int n) {
  int k = 1;
  while (k < n) {
    k *= 2;
  }
  if (k == n) {
    return true;
  } else {
    return false;
  }
}

std::vector<int> SequentialRadixSort(std::vector<int> vec) {
  int max = 0, pos = 1;
  std::vector<int> res(vec.size());
  if ((vec.size() == 1) || (vec.size() == 0)) return vec;
  for (size_t i = 1; i < vec.size(); i++)
    if (max < vec[i]) max = vec[i];
  while (max / pos > 0) {
    int digitCount[10] = {0};
    for (size_t i = 0; i < vec.size(); i++) digitCount[vec[i] / pos % 10]++;
    for (int i = 1; i < 10; i++) digitCount[i] += digitCount[i - 1];
    for (int i = vec.size() - 1; i >= 0; i--)
      res[--digitCount[vec[i] / pos % 10]] = vec[i];
    for (size_t i = 0; i < vec.size(); i++) vec[i] = res[i];
    pos *= 10;
  }
  return vec;
}

std::vector<int> BetcherMerge(std::vector<int> vec, int n) {
  int ProcRank;
  int ProcNum;
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  if ((ProcNum == 1) || (ProcNum > static_cast<int>(vec.size()))) {
    vec = SequentialRadixSort(vec);
    return vec;
  }
  int Delta = n / ProcNum;
  int ost = n % ProcNum;
  std::vector<int> localvector(Delta);
  if (ProcRank == 0) {
    localvector.resize(Delta + ost);
    localvector = std::vector<int>(vec.begin(), vec.begin() + ost + Delta);
    for (int i = 1; i < ProcNum; i++) {
      MPI_Send(vec.data() + Delta * i + ost, Delta, MPI_INT, i, 0,
               MPI_COMM_WORLD);
    }
  } else {
    MPI_Status status;
    MPI_Recv(localvector.data(), Delta, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
  }
  localvector = SequentialRadixSort(localvector);
  if (ProcRank % 2 != 0) {
    std::vector<int> result;
    MPI_Send(localvector.data(), Delta, MPI_INT, ProcRank - 1, 1,
             MPI_COMM_WORLD);
    MPI_Status status;
    int count;
    (ProcRank - 1 == 0) ? count = Delta + ost : count = Delta;
    std::vector<int> getvec(count);
    MPI_Recv(getvec.data(), count, MPI_INT, ProcRank - 1, 1, MPI_COMM_WORLD,
             &status);
    result = OddSpliter(getvec, localvector, result);
    MPI_Send(result.data(), static_cast<int>(result.size()), MPI_INT,
             ProcRank - 1, 1, MPI_COMM_WORLD);
  }
  if ((ProcRank % 2 == 0) && (ProcRank + 1 < ProcNum)) {
    std::vector<int> result;
    std::vector<int> getvec(Delta);
    MPI_Status status;
    MPI_Recv(getvec.data(), Delta, MPI_INT, ProcRank + 1, 1, MPI_COMM_WORLD,
             &status);
    int count;
    (ProcRank == 0) ? count = Delta + ost : count = Delta;
    MPI_Send(localvector.data(), count, MPI_INT, ProcRank + 1, 1,
             MPI_COMM_WORLD);
    result = EvenSpliter(getvec, localvector, result);
    MPI_Status status2;
    std::vector<int> res(static_cast<int>(result.size()));
    MPI_Recv(res.data(), static_cast<int>(result.size()), MPI_INT, ProcRank + 1,
             1, MPI_COMM_WORLD, &status2);
    localvector = SimpleComparator(localvector, result, res);
  }
  if ((ProcRank % 2 == 0) && (degree_2(ProcRank) == false) && (ProcRank != 0)) {
    int off = ProcRank;
    int shift = 0;
    while (degree_2(off) == false) {
      off--;
      shift++;
    }
    int dim = static_cast<int>(localvector.size());
    int dim2;
    MPI_Send(&dim, 1, MPI_INT, ProcRank - shift, 3, MPI_COMM_WORLD);
    MPI_Status status1;
    MPI_Recv(&dim2, 1, MPI_INT, ProcRank - shift, 3, MPI_COMM_WORLD, &status1);
    std::vector<int> getvec(dim2);
    MPI_Send(localvector.data(), dim, MPI_INT, ProcRank - shift, 3,
             MPI_COMM_WORLD);
    MPI_Status status;
    MPI_Recv(getvec.data(), dim2, MPI_INT, ProcRank - shift, 3, MPI_COMM_WORLD,
             &status);
    std::vector<int> result(dim + dim2);
    result = OddSpliter(getvec, localvector, result);
    MPI_Send(result.data(), dim + dim2, MPI_INT, ProcRank - shift, 3,
             MPI_COMM_WORLD);
  }
  if ((degree_2(ProcRank) == true) && (ProcRank + 2 < ProcNum) &&
      (degree_2(ProcRank + 2) == false) && (ProcRank % 2 == 0)) {
    int off = ProcRank;
    int shift = 2;
    while ((degree_2(off + shift) == false) && ((off + shift) < ProcNum)) {
      MPI_Status status;
      int dim2;
      MPI_Recv(&dim2, 1, MPI_INT, ProcRank + shift, 3, MPI_COMM_WORLD, &status);
      std::vector<int> getvec(dim2);
      int dim = static_cast<int>(localvector.size());
      MPI_Send(&dim, 1, MPI_INT, ProcRank + shift, 3, MPI_COMM_WORLD);
      MPI_Status status1;
      MPI_Recv(getvec.data(), dim2, MPI_INT, ProcRank + shift, 3,
               MPI_COMM_WORLD, &status1);
      std::vector<int> result(dim + dim2);
      result = EvenSpliter(getvec, localvector, result);
      MPI_Send(localvector.data(), dim, MPI_INT, ProcRank + shift, 3,
               MPI_COMM_WORLD);
      std::vector<int> res(dim + dim2);
      MPI_Status status3;
      MPI_Recv(res.data(), dim + dim2, MPI_INT, ProcRank + shift, 3,
               MPI_COMM_WORLD, &status3);
      localvector = SimpleComparator(localvector, result, res);
      shift += 2;
    }
  }
  if ((degree_2(ProcRank) == true) && (ProcRank != 0) && (ProcRank % 2 == 0)) {
    int dim = static_cast<int>(localvector.size());
    MPI_Send(&dim, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
    MPI_Send(localvector.data(), dim, MPI_INT, 0, 1, MPI_COMM_WORLD);
  }
  if (ProcRank == 0) {
    int count = 2;
    while (count < ProcNum) {
      MPI_Status status1;
      int dim2;
      MPI_Recv(&dim2, 1, MPI_INT, count, 1, MPI_COMM_WORLD, &status1);
      std::vector<int> getvec(dim2);
      MPI_Status status;
      MPI_Recv(getvec.data(), dim2, MPI_INT, count, 1, MPI_COMM_WORLD, &status);
      std::vector<int> result(dim2 + static_cast<int>(localvector.size()));
      result = EvenSpliter(getvec, localvector, result);
      result = OddSpliter(getvec, localvector, result);
      int dim = static_cast<int>(localvector.size());
      localvector.resize(dim + dim2);
      localvector = result;
      localvector = SimpleComparator(localvector, result, result);
      count *= 2;
    }
  }
  return localvector;
}
