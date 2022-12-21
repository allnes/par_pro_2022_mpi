#include"min_vector_elem.h"
#include<vector>
#include<iostream>
#include<algorithm>
#include<stdafx.h>
#include<mpi.h>
void Proccess_Initialization(std::vector<int>&vec,int& n, std::vector<int>& ProcVec, int ProcRank, int ProcSize,int& NumElem)
{
	if (ProcRank == 0)
	{
		do
		{
			std::cout << "Enter size of vector(not smaller than number of processes!)" << std::endl;
			std::cin >> n;
		} while (n<ProcSize);
	}
	MPI_Bcast(&n, 1,MPI_INT,0,MPI_COMM_WORLD);
	int RestElements = n;
	for (int i = 0; i < ProcRank; i++)
	{
		RestElements = RestElements - RestElements / (ProcSize - i);
	}
	NumElem = RestElements / (ProcSize - ProcRank);
	ProcVec.resize(NumElem);
	if (ProcRank == 0)
	{
		vec.resize(n);
		std::cout << "Enter elements:" << std::endl;
		for (int i = 0; i < n; i++)
		{
			std::cin >> vec[i];
		}
	}
}
void DataDistribution(std::vector<int>& vec, int n, std::vector<int>& ProcVec, int ProcRank, int ProcSize,int NumElem)
{
	std::vector<int> pSendNum;
	std::vector<int> pSendInd;
	pSendNum.resize(n);
	pSendInd.resize(n);
	int RestElem = n;
	int NumberofElements = n / ProcSize;
	pSendNum[0] = NumberofElements;
	pSendInd[0] = 0;
	for (int i = 1; i < ProcSize; i++)
	{
		RestElem -= NumberofElements;
		NumberofElements = RestElem / (ProcSize - i);
		pSendNum[i] = NumberofElements;
		pSendInd[i] = pSendInd[i - 1] + pSendNum[i - 1];
	}
	MPI_Scatterv(vec.data(), pSendNum.data(), pSendInd.data(), MPI_INT, ProcVec.data(), pSendNum[ProcRank], MPI_INT, 0, MPI_COMM_WORLD);
}
int Smallest_Element(int ProcRank, int ProcSize)
{
	int NumElem;
	std::vector<int> ProcVec;
	std::vector<int> vec;
	int n;
	Proccess_Initialization(vec,n, ProcVec, ProcRank, ProcSize,NumElem);
	DataDistribution(vec,n, ProcVec, ProcRank, ProcSize,NumElem);
	int min_element, total_min;
	min_element = *std::min_element(ProcVec.begin(), ProcVec.end());
	MPI_Reduce(&min_element, &total_min, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);
	return total_min;
}