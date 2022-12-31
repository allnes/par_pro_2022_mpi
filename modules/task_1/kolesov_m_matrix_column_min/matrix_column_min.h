// Copyright 2022 Kolesov Maxim
#pragma once

#include <vector>

std::vector<int> generateMatrix(int n, int m);
std::vector<int> transposeMatrix(const std::vector<int> &matrix, int n, int m);

int getMinInSequence(const std::vector<int> &sec);
std::vector<int> getColumnMinParalel(const std::vector<int> &matrix, int n, int m);
