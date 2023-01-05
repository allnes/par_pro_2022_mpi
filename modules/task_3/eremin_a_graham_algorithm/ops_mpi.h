// Copyright 2022 Eremin Aleksandr
#ifndef MODULES_TASK_3_EREMIN_A_GRAHAM_ALGORITHM_OPS_MPI_H_
#define MODULES_TASK_3_EREMIN_A_GRAHAM_ALGORITHM_OPS_MPI_H_

#include <vector>
#include <string>
#include <iostream>

using std::vector;

struct Point {
    int x, y;
    Point() : x(0), y(0) {}
    Point(const int _x, const int _y) : x(_x), y(_y) {}
};

vector<Point> GrahamMethod(vector<Point> vectorOfVertex);
vector<Point> parallelGrahamMethod(vector<Point> vectorOfVertex,
    vector<int>::size_type vectorSize);
vector<Point> random(const vector<int>::size_type Size);

#endif  // MODULES_TASK_3_EREMIN_A_GRAHAM_ALGORITHM_OPS_MPI_H_
