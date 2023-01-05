// Copyright 2022 Uvarkin Ilya
#pragma once
#include <vector>

struct CCSMatrix {
    std::vector<double> value;
    std::vector<int> points;
    std::vector<int> rowIndex;
    int columns = 0, rows = 0, non_zero = 0;
};

struct Vector{
    std::vector<double> value;
    std::vector<int> indexes;
};

CCSMatrix getCCSMatrix(const std::vector<std::vector<double>>& matrix);
std::vector<double> Multiply(const CCSMatrix& matrixA, const CCSMatrix& matrixB);

CCSMatrix CCS(const std::vector<double>& _newMatrix,
              const int _newColumns, const int _newRows);

std::vector<double> MultiplyParallel(CCSMatrix matrixA, CCSMatrix matrixB);
void setMetaToMatrix(CCSMatrix* matrix);
void growVectorsMatrix(CCSMatrix* matrix);
void SendDataMatrix(CCSMatrix* matrix);
