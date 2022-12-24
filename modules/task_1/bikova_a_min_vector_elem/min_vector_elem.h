// Copyright 2022 Alexandra Bikova
#ifndef MODULES_TASK_1_SASA_1LAB_MIN_VECTOR_ELEM_H_
#define MODULES_TASK_1_SASA_1LAB_MIN_VECTOR_ELEM_H_
#include<vector>
void Smallest_Element(int ProcRank, int ProcSize, std::vector<int>* vec, int* min_element);
void Proccess_Initialization(std::vector<int>* vec, int* n
, std::vector<int>* ProcVec, int ProcRank, int ProcSize, int* NumElem);
void DataDistribution(std::vector<int>* vec, int n, std::vector<int>* ProcVec, int ProcRank, int ProcSize, int NumElem);
void CreateRandomVec(std::vector<int>* vec);
#endif  // MODULES_TASK_1_SASA_1LAB_MIN_VECTOR_ELEM_H_
