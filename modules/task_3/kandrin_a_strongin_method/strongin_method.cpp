// Copyright 2022 Kandrin Alexey
#include "../../../modules/task_3/kandrin_a_strongin_method/strongin_method.h"

#include <mpi.h>

#include <algorithm>
#include <cassert>
#include <cfloat>
#include <cmath>
#include <utility>

//=============================================================================
// Function : WorkSplitter::WorkSplitter
// Purpose  : Constructor.
//=============================================================================
WorkSplitter::WorkSplitter(size_t work, size_t workerCount)
    : m_workDistribution(workerCount, 0) {
  if (work <= workerCount) {
    for (size_t currentWorker = 0; currentWorker < work; ++currentWorker) {
      m_workDistribution[currentWorker] = 1;
    }
  } else {
    for (size_t currentWorker = 0; work != 0; ++currentWorker) {
      size_t workForCurrentWorker = work / workerCount;
      m_workDistribution[currentWorker] = work / workerCount;
      work -= workForCurrentWorker;
      workerCount -= 1;
    }
  }
}

//=============================================================================
// Function : GetPartWork
// Purpose  : Determining how much work a worker should do.
//=============================================================================
size_t WorkSplitter::GetPartWork(size_t workerNumber) const {
  return m_workDistribution[workerNumber];
}

//=============================================================================
// Function : GetPrevPartWork
// Purpose  : Determines how much work will be done by workers from 0 to
//            workerNumber - 1.
//=============================================================================
size_t WorkSplitter::GetPrevPartWork(size_t workerNumber) const {
  size_t work = 0;
  for (size_t i = 0; i < workerNumber; ++i) {
    work += m_workDistribution.at(i);
  }
  return work;
}

//=============================================================================
// Struct  : Sequential
// Purpose : For sequential execution strategy
//=============================================================================
struct Sequential {};

//=============================================================================
// Struct  : Parallel
// Purpose : For parallel execution strategy
//=============================================================================
struct Parallel {};

namespace {
//=============================================================================
// Function : Calculate_M
// Purpose  : Calculate the "M" estimate of the Lipschitz constant:
//  M = max{1 <= i <= n}(abs((Z_{i}.end - Z_{i}.begin) / (Y_{i}.end -
//  Y_{i}.begin))
//=============================================================================
template <class ExectionPolicy>
double Calculate_M(Function&& f, const std::vector<Segment>& y);

//=============================================================================
// Function : Calculate_M
// Purpose  : Calculate_M - sequential version
//=============================================================================
template <>
double Calculate_M<Sequential>(Function&& f, const std::vector<Segment>& y) {
  double M = 0;

  for (int i = 0; i < y.size(); ++i) {
    const double y_begin = y.at(i).begin;
    const double y_end = y.at(i).end;

    const double zDif = f(y_end) - f(y_begin);
    const double yDif = y_end - y_begin;
    const double currentMax = std::abs(zDif / yDif);
    if (currentMax > M) {
      M = currentMax;
    }
  }

  return M;
}

//=============================================================================
// Function : Calculate_M
// Purpose  : Calculate_M - parallel version
//=============================================================================
template <>
double Calculate_M<Parallel>(Function&& f, const std::vector<Segment>& y) {
  int procCount;
  MPI_Comm_size(MPI_COMM_WORLD, &procCount);

  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  WorkSplitter workSplitter(y.size(), procCount);

  size_t workForThisProc = workSplitter.GetPartWork(rank);
  std::vector<Segment> localY;

  if (rank == 0) {
    for (int procNum = 1; procNum < procCount; ++procNum) {
      size_t workForProc = workSplitter.GetPartWork(procNum);
      if (workForProc != 0) {
        size_t workForPrevProc = workSplitter.GetPrevPartWork(procNum);
        MPI_Send(&y.at(workForPrevProc),
                 static_cast<int>(workForProc * sizeof(Segment)), MPI_CHAR,
                 procNum, 0, MPI_COMM_WORLD);
      }
    }

    localY = std::vector<Segment>(y.begin(), y.begin() + workForThisProc);
  } else {
    if (workForThisProc != 0) {
      localY = std::vector<Segment>(workForThisProc);
      MPI_Recv(localY.data(),
               static_cast<int>(workForThisProc * sizeof(Segment)), MPI_CHAR, 0,
               0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
  }

  double M = Calculate_M<Sequential>(std::forward<Function>(f), localY);

  double result;

  MPI_Allreduce(&M, &result, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
  // after this call, the value "res" of the variable will be synchronized in
  // all processes, so calling "MPI_Bcast" is not required

  return result;
}

//=============================================================================
// Function : Calculate_m
// Purpose  : Calculate the "m" estimate of the Lipschitz constant:
//  m = { 1 if M = 0, r * M if M > 0}, where r > 1 - parameter,
//  Z_{i}.begin = f(Y_{i}.begin),
//  Z_{i}.end = f(Y_{i}.end),
//  M = Calculate_M(...) (see above)
//=============================================================================
double Calculate_m(const double M, const double r) {
  assert(M >= 0);
  assert(r > 1);
  return (M == 0 ? 1 : r * M);
}

//=============================================================================
// Function : CalculateIndexOfMaxR
// Purpose  : Calculate all of characteristic "R".
// R(i) = m(Y_{i} - Y_{i - 1}) + sqr(Z_{i} - Z_{i - 1}) / m(Y_{i} - Y{i - 1}) -
// 2(Z_{i} + Z_{i-1}), where m = Calculate_m(...) (see above),
// Z_{i} = f(Y_{i}).
// Then return maximum of the characteristics and its index.
//=============================================================================
template <class ExecutionPolicy>
std::pair<double, int> CalculateIndexOfMaxR(Function&& f,
                                            const std::vector<Segment>& y,
                                            const double m);

//=============================================================================
// Function : CalculateIndexOfMaxR
// Purpose  : CalculateIndexOfMaxR - sequential version
//=============================================================================
template <>
std::pair<double, int> CalculateIndexOfMaxR<Sequential>(
    Function&& f, const std::vector<Segment>& y, const double m) {
  std::pair<double, int> maxR_Index(-DBL_MAX, -1);

  for (int i = 0; i < y.size(); ++i) {
    const double y_begin = y.at(i).begin;
    const double y_end = y.at(i).end;

    const double yDif = y_end - y_begin;
    const double zDif = f(y_end) - f(y_begin);
    const double zSum = f(y_end) + f(y_begin);
    const double R = m * yDif + zDif * zDif / (m * yDif) - 2 * zSum;

    double& maxR = maxR_Index.first;
    int& maxIndex = maxR_Index.second;

    if (R > maxR) {
      maxR = R;
      maxIndex = i;
    }
  }

  return maxR_Index;
}

//=============================================================================
// Function : CalculateIndexOfMaxR
// Purpose  : CalculateIndexOfMaxR - parallel version
//=============================================================================
template <>
std::pair<double, int> CalculateIndexOfMaxR<Parallel>(
    Function&& f, const std::vector<Segment>& y, const double m) {
  std::pair<double, int> maxR_Index(-DBL_MAX, -1);

  int procCount;
  MPI_Comm_size(MPI_COMM_WORLD, &procCount);

  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  WorkSplitter workSplitter(y.size(), procCount);

  size_t workForThisProc = workSplitter.GetPartWork(rank);
  std::vector<Segment> localY;

  if (rank == 0) {
    for (int procNum = 1; procNum < procCount; ++procNum) {
      size_t workForProc = workSplitter.GetPartWork(procNum);
      if (workForProc != 0) {
        size_t workForPrevProc = workSplitter.GetPrevPartWork(procNum);
        MPI_Send(&y.at(workForPrevProc),
                 static_cast<int>(workForProc * sizeof(Segment)), MPI_CHAR,
                 procNum, 0, MPI_COMM_WORLD);
      }
    }

    localY = std::vector<Segment>(y.begin(), y.begin() + workForThisProc);
  } else {
    if (workForThisProc != 0) {
      localY = std::vector<Segment>(workForThisProc);
      MPI_Recv(localY.data(),
               static_cast<int>(workForThisProc * sizeof(Segment)), MPI_CHAR, 0,
               0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
    }
  }

  auto indexOfMaxR =
      CalculateIndexOfMaxR<Sequential>(std::forward<Function>(f), localY, m);

  // the sequential version finds the index of the maximum R relative to the
  // beginning of the "localY", so we add an offset:
  indexOfMaxR.second += static_cast<int>(workSplitter.GetPrevPartWork(rank));

  std::pair<double, int> res;

  // this function will find the maximum R ("indexOfMaxR.first") of all
  // processes and write it, as well as its index ("indexOfMaxR.second"), to
  // "res"
  MPI_Allreduce(&indexOfMaxR, &res, 1, MPI_DOUBLE_INT, MPI_MAXLOC,
                MPI_COMM_WORLD);

  // after this call, the value "res" of the variable will be synchronized in
  // all processes, so calling "MPI_Bcast" is not required

  return res;
}

//=============================================================================
// Function : GetMin
// Purpose  : Get minimum of function f in [a; b]
//=============================================================================
template <class ExecutionPolicy>
double GetMin(Function&& f, double a, double b, double epsilon) {
  std::vector<Segment> y = {Segment{a, b}};

  const double r = 2.0;
  const size_t maxIterationCount = 100000;

  for (size_t iterationIndex = 0; iterationIndex < maxIterationCount;
       ++iterationIndex) {
    double _M = Calculate_M<ExecutionPolicy>(std::forward<Function>(f), y);
    const double m = Calculate_m(_M, r);
    auto indexOfMaxR =
        CalculateIndexOfMaxR<ExecutionPolicy>(std::forward<Function>(f), y, m);
    const auto& currentSegment = y.at(indexOfMaxR.second);
    const double y_begin = currentSegment.begin;
    const double y_end = currentSegment.end;
    if (y_end - y_begin < epsilon) {
      return f(y_end);
    }
    double yn =
        y_begin + (y_end - y_begin) / 2 + (f(y_end) - f(y_begin)) / (2 * m);
    y.push_back(Segment{y_begin, yn});
    y.at(indexOfMaxR.second).begin = yn;
  }

  // calculation error
  return NAN;
}
}  // namespace

//=============================================================================
// Function : GetMinSequential
// Purpose  : Get minimum of function f in [a; b] - sequential version
//=============================================================================
double GetMinSequential(Function&& f, double a, double b, double epsilon) {
  return GetMin<Sequential>(std::forward<Function>(f), a, b, epsilon);
}

//=============================================================================
// Function : GetMinParallel
// Purpose  : Get minimum of function f in [a; b] - parallel version
//=============================================================================
double GetMinParallel(Function&& f, double a, double b, double epsilon) {
  return GetMin<Parallel>(std::forward<Function>(f), a, b, epsilon);
}
