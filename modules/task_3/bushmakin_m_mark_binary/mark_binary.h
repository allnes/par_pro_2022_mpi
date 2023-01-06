// Copyright 2022 Bushmakin Maxim
#ifndef MODULES_TASK_3_BUSHMAKIN_M_MARK_BINARY_MARK_BINARY_H_
#define MODULES_TASK_3_BUSHMAKIN_M_MARK_BINARY_MARK_BINARY_H_

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <ctime>
#include <iostream>
#include <random>
#include <vector>
#include <set>
#include <utility>
using std::cout;
using std::pair;
using std::set;
using std::vector;

class MarkImage {
 private:
    vector<int> elements;
    int w;
    int h;

 public:
    MarkImage(int _w, int _h);
    MarkImage(const MarkImage &img);

    int W() const;
    int H() const;

    bool HasElem(int x, int y) const;
    void Print() const;
    void Random();

    int Elem(int x, int y) const;
    void SetElem(int x, int y, int val);

    bool operator==(const MarkImage &img) const;
};

MarkImage FirstMark(const MarkImage &original, int beginMark);

MarkImage SecondMark(const MarkImage &img);

MarkImage MarkParal(const MarkImage &original);

bool CheckMark(const MarkImage &checkable);

#endif  // MODULES_TASK_3_BUSHMAKIN_M_MARK_BINARY_MARK_BINARY_H_
