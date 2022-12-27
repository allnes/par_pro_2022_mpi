// Copyright 2022 Shipilov Artem
#ifndef MODULES_TASK_3_SHIPILOV_A_JARVIS_JARVIS_H_
#define MODULES_TASK_3_SHIPILOV_A_JARVIS_JARVIS_H_

#include <mpi.h>
#include <vector>

using std::vector;

struct Point2d {
    int x;
    int y;

    Point2d() : x(0), y(0) {}
    Point2d(const int _x, const int _y) : x(_x), y(_y) {}
};

vector<Point2d> Random(const vector<int>::size_type vecSize);

vector<Point2d> SequenJarvis(const vector<Point2d>& vecVertex);
vector<Point2d> ParallelJarvis(const vector<Point2d>& vecVertex, vector<int>::size_type vecSize);

#endif  // MODULES_TASK_3_SHIPILOV_A_JARVIS_JARVIS_H_
