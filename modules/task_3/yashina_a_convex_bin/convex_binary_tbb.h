// Copyright 2020 Silenko Dmitrii
#ifndef MODULES_TASK_3_SILENKO_D_CONVEX_BINARY_TBB_CONVEX_BINARY_TBB_H_
#define MODULES_TASK_3_SILENKO_D_CONVEX_BINARY_TBB_CONVEX_BINARY_TBB_H_

#include <tbb/tbb.h>
#include <algorithm>
#include <vector>

int** getRandomMas(int n, int m);

std::vector<int*> getComponent(int** mas, int n, int m, int** *convex_mas, int *num);

double length(int* p1, int* p2);
double cosvec(int* p1, int* p2, int* p3);
std::vector<int*> Jarvis(int** convex_mas, int n);
std::vector<int*> Inside(std::vector<int*> hull, int** convex_mas, int n);

std::vector<int*> getComponent_TBB(int** mas, int n, int m, int** *convex_mas, int *num, int num_thr);

#endif  // MODULES_TASK_3_SILENKO_D_CONVEX_BINARY_TBB_CONVEX_BINARY_TBB_H_
