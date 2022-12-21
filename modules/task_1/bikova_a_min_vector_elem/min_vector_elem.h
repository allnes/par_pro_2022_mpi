#ifndef MIN_VEC_ELEM
#define MIN_VEC_ELEM
#include<vector>
int Smallest_Element(int ProcRank, int ProcSize);
void Proccess_Initialization(std::vector<int>& vec,int& n,std::vector<int>& ProcVec,int ProcRank, int ProcSize,int& NumElem);
void DataDistribution(std::vector<int> &vec,int n,std::vector<int>& ProcVec, int ProcRank, int ProcSize,int NumElem);
#endif