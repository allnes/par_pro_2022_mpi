// Copyright 2022 Kandrin Alexey
#ifndef MODULES_TASK_3_KANDRIN_A_STRONGIN_METHOD_STRONGIN_METHOD_H_
#define MODULES_TASK_3_KANDRIN_A_STRONGIN_METHOD_STRONGIN_METHOD_H_

#ifndef NDEBUG
#define NDEBUG
#endif

#include <vector>

//=============================================================================
// Class   : WorkSplitter
// Purpose : Calculating how much work the worker needs to do.
//=============================================================================
class WorkSplitter {
  std::vector<size_t> m_workDistribution;

 public:
  // Constructor.
  // \param work Total amount of work.
  // \param workerCount Total number of workers who can do the work.
  WorkSplitter(size_t work, size_t workerCount);

  // Determining how much work a worker should do.
  // \param workerNumber The worker for whom we calculate how much work he has
  // to do.
  // \return What part of the work should worker do. (If there are fewer
  // work than workerCount, then some workers will do nothing. If work is
  // proportional to the workerCount, then all workers will do the same amount
  // of work)
  size_t GetPartWork(size_t workerNumber) const;

  // Determines how much work will be done by workers from 0 to workerNumber
  // - 1. \param workerNumber you need to sum up all the work up to this number
  // \return What work will be done by workers [0..workerNumber - 1]
  size_t GetPrevPartWork(size_t workerNumber) const;
};

// The type of function that can be evaluated
using Function = double (*)(double);

//=============================================================================
// Struct  : Segment
// Purpose : Separate segment for which the characteristic "R" will be
//           calculated
//=============================================================================
struct Segment {
  double begin;
  double end;
};

// Get minimum of function f in [a; b] (sequential version)
double GetMinSequential(Function&& f, double a, double b, double epsilon);

// Get minimum of function f in [a; b] (parallel version)
double GetMinParallel(Function&& f, double a, double b, double epsilon);

#endif  // MODULES_TASK_3_KANDRIN_A_STRONGIN_METHOD_STRONGIN_METHOD_H_
