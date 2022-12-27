// Copyright 2022 Zanozin Danila
#include<mpi.h>
#include<vector>
void DoWork(MPI_Op op, std::vector<int>* a, std::vector<int>* b);
void Reduce(int* sendbuf, int* recvbuf, int count, MPI_Datatype type, MPI_Op op, int root, MPI_Comm comm);
#endif  