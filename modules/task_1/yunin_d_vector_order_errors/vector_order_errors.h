// Copyright 2022 Yunin D.
#pragma once
#include <vector>
#include <random>

using std::vector;
using std::mt19937;

vector<int> CreateRandomVector(int v_size, int right_border, int offset);
void UpdateRandNumbers(mt19937 *gen);
int CountErrorsOrderNeigboringElementsVector(const vector<int> &my_vector);
int CountErrorsOrderNeigboringElementsVectorParallel(const vector<int> &my_vector);
