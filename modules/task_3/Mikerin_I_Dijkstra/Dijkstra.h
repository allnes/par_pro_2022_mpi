// Copyright 2022 Mikerin Ilya
#ifndef MODULES_TASK_3_MIKERIN_I_DIJKSTRA_DIJKSTRA_H_
#define MODULES_TASK_3_MIKERIN_I_DIJKSTRA_DIJKSTRA_H_


#include <vector>
#include <string>

int* getRandomVector(int  sz);
int* ParallelDijkstra(int** vec, const int count, const int a);
int* SequentialDijkstra(int** vec, const int count, const int a);

#endif  // MODULES_TASK_3_MIKERIN_I_DIJKSTRA_DIJKSTRA_H_
