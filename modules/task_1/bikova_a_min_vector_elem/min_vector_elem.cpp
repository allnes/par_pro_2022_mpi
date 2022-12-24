// Copyright 2022 Alexandra Bikova
#include"../../../modules/task_1/bikova_a_min_vector_elem/min_vector_elem.h"
#include<mpi.h>
#include<random>
#include<vector>
#include<iostream>
#include<algorithm>
void Proccess_Initialization(std::vector<int>* vec, int* n
, std::vector<int>* ProcVec, int ProcRank, int ProcSize, int* NumElem) {
    if (ProcRank == 0) { *n = vec->size(); }
    MPI_Bcast(n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    int RestElements = *n;
    for (int i = 0; i < ProcRank; i++) {
         RestElements = RestElements - RestElements / (ProcSize - i);
    }
    *NumElem = RestElements / (ProcSize - ProcRank);
    (*ProcVec).resize(*NumElem);
}
void CreateRandomVec(std::vector<int>* vec) {
    std::random_device dev;
    std::mt19937 rand_r(dev());
    int n = rand_r() % 100 + 1;
    for (int i = 0; i < n; i++) {
        (*vec).push_back(rand_r() % 1000 + 1);
    }
}
void DataDistribution(std::vector<int>* vec, int n
, std::vector<int>* ProcVec, int ProcRank, int ProcSize, int NumElem) {
    std::vector<int> pSendNum;
    std::vector<int> pSendInd;
    pSendNum.resize(n);
    pSendInd.resize(n);
    int RestElem = n;
    int NumberofElements = n / ProcSize;
    pSendNum[0] = NumberofElements;
    pSendInd[0] = 0;
    for (int i = 1; i < ProcSize; i++) {
        RestElem -= NumberofElements;
        NumberofElements = RestElem / (ProcSize - i);
        pSendNum[i] = NumberofElements;
        pSendInd[i] = pSendInd[i - 1] + pSendNum[i - 1];
    }
    MPI_Scatterv((*vec).data(), pSendNum.data(), pSendInd.data()
    , MPI_INT, (*ProcVec).data(), pSendNum[ProcRank], MPI_INT, 0, MPI_COMM_WORLD);
}
void Smallest_Element(int ProcRank, int ProcSize, std::vector<int>* vec, int* min_element) {
    int NumElem;
    std::vector<int> ProcVec;
    int n;
    Proccess_Initialization(vec, &n, &ProcVec, ProcRank, ProcSize, &NumElem);
    DataDistribution(vec, n, &ProcVec, ProcRank, ProcSize, NumElem);
    int mini_element, total_min;
    mini_element = *std::min_element(ProcVec.begin(), ProcVec.end());
    MPI_Reduce(&mini_element, &total_min, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);
    *min_element = total_min;
}
