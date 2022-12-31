// Copyright 2022 Pronina Tatiana
#include <mpi.h>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include "../../../modules/task_2/pronina_t_gather/gather.h"

template <class T>
std::vector<T> getRandomVector(int size) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::vector<T> vec(size);
    for (int  i = 0; i < size; i++) { vec[i] = static_cast<T>(gen() % 100); }
    return vec;
}

template
std::vector<int> getRandomVector<int>(int size);

template
std::vector<float> getRandomVector<float>(int size);

template
std::vector<double> getRandomVector<double>(int size);

int Gather(void* sbuf, int scount, MPI_Datatype stype, void* rbuf, int rcount,
    MPI_Datatype rtype, int root, MPI_Comm comm) {
    int my_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    if (my_rank == root) {
        int tasks;
        MPI_Comm_size(MPI_COMM_WORLD, &tasks);
        MPI_Status Status;
        if (stype != rtype) {
            throw "Send and receive types do not match!";
        }
        if (scount != rcount) {
            throw "Send and receive counts do not match!";
        }
        if (rtype == MPI_INT) {
            for (int i = root * rcount; i < rcount * (root + 1); i++) {
                static_cast<int*>(rbuf)[i] = static_cast<int*>(sbuf)[i - root*rcount];
            }
            for (int i = 0; i < tasks; i++) {
                if (i != root) {
                    MPI_Recv(static_cast<int*>(rbuf) + rcount * i, rcount, rtype, MPI_ANY_SOURCE, i, comm, &Status);
                }
            }
        } else if (rtype == MPI_FLOAT) {
            for (int i = root * rcount; i < rcount * (root + 1); i++) {
                static_cast<float*>(rbuf)[i] = static_cast<float*>(sbuf)[i - root * rcount];
            }
            for (int i = 0; i < tasks; i++) {
                if (i != root) {
                    MPI_Recv(static_cast<float*>(rbuf) + rcount * i, rcount, rtype, MPI_ANY_SOURCE, i, comm, &Status);
                }
            }
        } else if (rtype == MPI_DOUBLE) {
            for (int i = root * rcount; i < rcount * (root + 1); i++) {
                static_cast<double*>(rbuf)[i] = static_cast<double*>(sbuf)[i - root * rcount];
            }
            for (int i = 0; i < tasks; i++) {
                if (i != root) {
                    MPI_Recv(static_cast<double*>(rbuf) + rcount * i, rcount, rtype, MPI_ANY_SOURCE, i, comm, &Status);
                }
            }
        } else if (rtype == MPI_CHAR) {
            for (int i = root*rcount; i < rcount*(root+1); i++) {
                static_cast<char*>(rbuf)[i] = static_cast<char*>(sbuf)[i - root * rcount];
            }
            for (int i = 0; i < tasks; i++) {
                if (i != root) {
                    MPI_Recv(static_cast<char*>(rbuf) + rcount * i, rcount, rtype, MPI_ANY_SOURCE, i, comm, &Status);
                }
            }
        } else {
            return -1;
        }
    } else {
        MPI_Send(sbuf, scount, stype, root, my_rank, comm);
    }
    return 0;
}
