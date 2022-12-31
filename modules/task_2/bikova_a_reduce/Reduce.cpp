// Copyright 2022 Alexandra Bikova
#include<mpi.h>
#include<vector>
#include<algorithm>
#include<numeric>
#include<iostream>
#include<random>
#include"../../../modules/task_2/bikova_a_reduce/Reduce.h"
#define MAXSIZE 100
int ProcRank, ProcSize;
void DoWork(MPI_Op op, std::vector<int>* a, std::vector<int>* b) {
    std::random_device dev;
    std::mt19937 rand_r(dev());
    int count;
    if (ProcRank == 0) {
        count = rand_r() % 10 + 1;
    }
    MPI_Bcast(&count, 1, MPI_INT, 0, MPI_COMM_WORLD);
    int sendbuf[MAXSIZE];
    int myreduce[MAXSIZE];
    int mpi_reduce[MAXSIZE];
    for (int i = 0; i < count; i++) {
        sendbuf[i] = rand_r() % 50;
    }
    MPI_Reduce(sendbuf, mpi_reduce, count, MPI_INT, op, 0, MPI_COMM_WORLD);
    Reduce(sendbuf, myreduce, count, MPI_INT, op, 0, MPI_COMM_WORLD);
    if (ProcRank == 0) {
        for (int i = 0; i < count; i++) {
            (*a).push_back(mpi_reduce[i]);
            (*b).push_back(myreduce[i]);
        }
    }
}
void Reduce(int* sendbuf, int* recvbuf, int count, MPI_Datatype type, MPI_Op op, int root, MPI_Comm comm) {
    if (ProcRank != root) {
        for (int i = 1; i < ProcSize; i++) {
            if (i == ProcRank)MPI_Send(sendbuf, count, type, root, 0, MPI_COMM_WORLD);
            MPI_Barrier(MPI_COMM_WORLD);
        }
    } else {
        std::vector<int>all_elements;
        all_elements.resize(ProcSize * count);
        MPI_Status status;
        std::vector<int>receive_buff;
        receive_buff.resize(count);
        for (int i = 0; i < count; i++) {
            all_elements[i * ProcSize] = sendbuf[i];
        }
        for (int i = 1; i < ProcSize; i++) {
            MPI_Recv(receive_buff.data(), count, type, i, 0, comm, &status);
            for (int j = 0; j < count; j++) {
                all_elements[j * ProcSize + i] = receive_buff[j];
            }
            MPI_Barrier(MPI_COMM_WORLD);
        }
		/*for (auto elem : all_elements) { std::cout << elem << " "; }
		std::cout << std::endl;*/
        switch (op) {
            case MPI_MAX: {
                for (int i = 0; i < count; i++) {
                    recvbuf[i] = *std::max_element(all_elements.begin()+i*ProcSize
                    , all_elements.begin()+((i+1)*ProcSize));
                }
                break;
            }
            case MPI_MIN: {
                for (int i = 0; i < count; i++) {
                    recvbuf[i] = *std::min_element(all_elements.begin() + i * ProcSize
                    , all_elements.begin() + ((i + 1) * ProcSize));
                }
                break;
            }
            case MPI_SUM: {
                for (int i = 0; i < count; i++) {
                    recvbuf[i] = std::accumulate(all_elements.begin() + i * ProcSize
                    , all_elements.begin() + ((i + 1) * ProcSize), 0);
                }
                break;
            }
        }
    }
}
