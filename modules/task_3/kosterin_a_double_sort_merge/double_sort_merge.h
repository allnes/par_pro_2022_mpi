// Copyright 2022 Kosterin Alexey
#ifndef MODULES_TASK_3_KOSTERIN_A_DOUBLE_SORT_MERGE_DOUBLE_SORT_MERGE_H_
#define MODULES_TASK_3_KOSTERIN_A_DOUBLE_SORT_MERGE_DOUBLE_SORT_MERGE_H_

bool CheckSort(double *source, int size);
void Pass(double *source, double *dest, int size, int offset);
void LastPass(double *source, double *dest, int size, int offset);
void Merge(double *source1, int size1, double *source2, int size2,
           double *dest);
void Sort(double *source, int size);
void ParallelSort(double **source, int size);

#endif  // MODULES_TASK_3_KOSTERIN_A_DOUBLE_SORT_MERGE_DOUBLE_SORT_MERGE_H_
