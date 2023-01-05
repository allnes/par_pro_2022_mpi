// Copyright 2022 Nikitin Alexander
#include "../../../modules/task_1/nikitin_a_vector_min_value/vector_min_value.h"

#include <mpi.h>

#include <random>
#include <vector>

std::vector<int> Random(const int vecSize) {
  if (vecSize > 0) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::vector<int> info(vecSize);
    for (int i = 0; i < vecSize; i++) {
      info[i] = -10000 + gen() % 100000;
    }
    return info;
  }
  throw "Error: uncorrect size";
}

int Min(const std::vector<int>& vectr) {
  if (!vectr.empty()) {
    int min = vectr[0];
    for (int i = 1; i < static_cast<int>(vectr.size()); i++) {
      if (min > vectr[i]) min = vectr[i];
    }
    return min;
  }
  return 0;
}

int MinParallel(const std::vector<int>& globVec, const int vecSize) {
  int cntProc, rnkProc;
  MPI_Comm_size(MPI_COMM_WORLD, &cntProc);
  MPI_Comm_rank(MPI_COMM_WORLD, &rnkProc);

  int sizeBlock = vecSize / cntProc;
  int elementsRemaining = vecSize % cntProc;

  int res = 0;
  int localMin;

  if (rnkProc < elementsRemaining) sizeBlock++;

  if (vecSize / cntProc == 0) {
    localMin = Min(globVec);
    return localMin;
  }

  std::vector<int> countSends;
  std::vector<int> tmp;
  std::vector<int> dataReceived(sizeBlock);

  if (rnkProc == 0) {
    countSends.assign(cntProc, globVec.size() / cntProc);
    tmp.resize(cntProc);

    for (int i = 0; i < elementsRemaining; i++) countSends[i]++;
    for (int i = 0; i < cntProc - 1; i++) tmp[i + 1] = tmp[i] + countSends[i];
  }
  MPI_Scatterv(reinterpret_cast<const void*>(globVec.data()), countSends.data(),
               tmp.data(), MPI_INT, dataReceived.data(), sizeBlock, MPI_INT, 0,
               MPI_COMM_WORLD);

  localMin = Min(dataReceived);

  MPI_Reduce(reinterpret_cast<void*>(&localMin), reinterpret_cast<void*>(&res),
             1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);

  return res;
}
