// Copyright 2022 Shipilov Artem
#ifndef MODULES_TASK_1_SHIPILOV_A_FIND_MOST_DIFF_FIND_MOST_DIFF_H_
#define MODULES_TASK_1_SHIPILOV_A_FIND_MOST_DIFF_FIND_MOST_DIFF_H_

#include <utility>
#include <vector>

using std::pair;
using std::vector;

vector<int> Random(int size);

pair<int, int> SequenOperation(const vector<int>& vec);
pair<int, int> ParallelOperation(const vector<int>& vec);


void pairCompare(int* in, int* out, int* lenght, MPI_Datatype* type);

#endif  // MODULES_TASK_1_SHIPILOV_A_FIND_MOST_DIFF_FIND_MOST_DIFF_H_
