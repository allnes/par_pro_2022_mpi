// Copyright 2022 Tuzhilkina Polina
#include "../../../modules/task_3/tuzhilkina_p_shellsort/shellsort.h"
#include <mpi.h>
#include <algorithm>
#include <iostream>
#include <random>
#include <string>
#include <vector>

std::vector<int> getUnsortedVec(int size) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::vector<int> vec(size);
    for (int i = 0; i < size; i++) {
        vec[i] = gen() % 500;
    }
    return vec;
}

void shellsortSeq(std::vector<int>* vec) {
    int n = static_cast<int>(vec->size());

    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i += 1) {
            int temp = vec->at(i);
            int j;
            for (j = i; j >= gap && vec->at(j - gap) > temp; j -= gap) {
                (*vec)[j] = (*vec)[j - gap];
            }
            (*vec)[j] = temp;
        }
    }
}

void shellsortPar(std::vector<int>* globVec, int glob_vec_size) {
    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int maxActiveProcesses = glob_vec_size / 2;

    bool isRootProcess = (world_rank == 0);

    // Only first activeProcesses processes will be executing the task
    int activeProcesses = std::min(world_size, maxActiveProcesses);

    int color = (world_rank < activeProcesses) ? 0 : 1;
    MPI_Comm smallerComm;
    MPI_Comm_split(MPI_COMM_WORLD, color, world_rank, &smallerComm);
    if (color != 0) return;

    const int dQ = glob_vec_size / activeProcesses;
    const int rem = glob_vec_size % activeProcesses;

    // Sending vectors (parts of globVec) to all (active!) processes
    if (isRootProcess) {
        for (int pRank = 1; pRank < activeProcesses; pRank++) {
            MPI_Send(globVec->data() + rem + pRank * dQ, dQ, MPI_INT, pRank, 0,
                     smallerComm);
        }
    }

    // Receiving them
    std::vector<int> localVec(dQ);
    if (isRootProcess) {
        localVec =
            std::vector<int>(globVec->begin(), globVec->begin() + dQ + rem);
    } else {
        MPI_Status stat;
        MPI_Recv(localVec.data(), dQ, MPI_INT, 0, 0, smallerComm, &stat);
    }

    // Sort our local parts
    shellsortSeq(&localVec);

    // And then gather them all on rank 0 to globVecCpy
    std::vector<int> globVecCpy(glob_vec_size);
    MPI_Gather(localVec.data(), dQ, MPI_INT, globVecCpy.data(), dQ, MPI_INT, 0,
               smallerComm);

    if (isRootProcess) {
        // Adding remained elements from localVec of the root process to the end
        for (int i = 0; i < rem; i++) {
            globVecCpy[dQ * activeProcesses + i] = localVec[dQ + i];
        }

        // Merging sorted parts to globVec
        std::vector<int> positions(activeProcesses + 1);
        if (rem == 0) {
            positions[activeProcesses] = -1;
        }

        for (int i = 0; i < glob_vec_size; i++) {
            // We find the smallest element from all elements
            // on which we are watching via positions
            int minPartPos = -1;

            // We find the first unspent piece (it's index)
            for (int j = 0; j < activeProcesses + 1; j++) {
                if (positions[j] >= 0) {
                    minPartPos = j;
                    break;
                }
            }

            // Then find index of unspent piece that has minimum element now
            for (int j = minPartPos + 1; j < activeProcesses + 1; j++) {
                if (positions[j] >= 0 &&
                    globVecCpy[dQ * j + positions[j]] <
                        globVecCpy[dQ * minPartPos + positions[minPartPos]]) {
                    minPartPos = j;
                }
            }

            if (minPartPos < 0) break;

            // In minPartPos we have this index
            (*globVec)[i] = globVecCpy[dQ * minPartPos + positions[minPartPos]];

            positions[minPartPos]++;
            if (minPartPos != activeProcesses &&
                positions[minPartPos] == dQ) {
                positions[minPartPos] = -1;
            } else if (minPartPos == activeProcesses &&
                       positions[minPartPos] == rem) {
                positions[minPartPos] = -1;
            }
        }
    }
}

void printVecElements(const std::vector<int>& vec) {
    int size = static_cast<int>(vec.size());
    for (int i = 0; i < size - 1; i++) {
        std::cout << vec[i] << ", ";
    }
    std::cout << vec[size - 1];
}
