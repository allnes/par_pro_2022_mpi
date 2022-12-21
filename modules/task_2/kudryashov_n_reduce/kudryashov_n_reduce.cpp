  // Copyright 2022 Kudryashov Nikita
#include <mpi.h>
#include <random>
#include <ctime>
#include "../../../modules/task_2/kudryashov_n_reduce/kudryashov_n_reduce.h"

template <class T>
std::vector<T> generateRandomVector(int size) {
    std::mt19937 rnd;
    rnd.seed(std::time(nullptr));
    std::vector<T> vec(size);
    for (int i = 0; i < size; i++) {
        vec[i] = rnd() % 100;
    }
    return vec;
}

template
std::vector<int> generateRandomVector<int>(int size);

template
std::vector<double> generateRandomVector<double>(int size);

template
std::vector<float> generateRandomVector<float>(int size);


int reduce(void* sendbuf, void* recvbuf, int count, MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm) {
    int proc_num, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &proc_num);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == root) {
        if (datatype == MPI_INT) {
            for (int j = 0; j < count; j++) {
                (static_cast<int*>(recvbuf))[j] = (static_cast<int*>(sendbuf))[j];
            }
            int* buf = new int[count];
            for (int i = 1; i < proc_num; i++) {
                MPI_Status status;
                MPI_Recv(buf, count, datatype, MPI_ANY_SOURCE, MPI_ANY_TAG, comm, &status);
                for (int j = 0; j < count; j++) {
                    if (op == MPI_SUM) {
                        (static_cast<int*>(recvbuf))[j] += buf[j];
                    } else if (op == MPI_MAX) {
                        if ((static_cast<int*>(recvbuf))[j] < buf[j]) {
                            (static_cast<int*>(recvbuf))[j] = buf[j];
                        }
                    } else if (op == MPI_MIN) {
                        if ((static_cast<int*>(recvbuf))[j] > buf[j]) {
                            (static_cast<int*>(recvbuf))[j] = buf[j];
                        }
                    } else if (op == MPI_PROD) {
                        (static_cast<int*>(recvbuf))[j] *= buf[j];
                    } else {
                        return -1;
                    }
                }
            }
            delete[] buf;
        } else if (datatype == MPI_DOUBLE) {
            for (int j = 0; j < count; j++) {
                (static_cast<double*>(recvbuf))[j] = (static_cast<double*>(sendbuf))[j];
            }
            double* buf = new double[count];
            for (int i = 1; i < proc_num; i++) {
                MPI_Status status;
                MPI_Recv(buf, count, datatype, MPI_ANY_SOURCE, MPI_ANY_TAG, comm, &status);
                for (int j = 0; j < count; j++) {
                    if (op == MPI_SUM) {
                        (static_cast<double*>(recvbuf))[j] += buf[j];
                    } else if (op == MPI_MAX) {
                        if ((static_cast<double*>(recvbuf))[j] < buf[j]) {
                            (static_cast<double*>(recvbuf))[j] = buf[j];
                        }
                    } else if (op == MPI_MIN) {
                        if ((static_cast<double*>(recvbuf))[j] > buf[j]) {
                            (static_cast<double*>(recvbuf))[j] = buf[j];
                        }
                    } else if (op == MPI_PROD) {
                        (static_cast<double*>(recvbuf))[j] *= buf[j];
                    } else {
                        return -2;
                    }
                }
            }
            delete[] buf;
        } else if (datatype == MPI_FLOAT) {
            for (int j = 0; j < count; j++) {
                (static_cast<float*>(recvbuf))[j] = (static_cast<float*>(sendbuf))[j];
            }
            float* buf = new float[count];
            for (int i = 1; i < proc_num; i++) {
                MPI_Status status;
                MPI_Recv(buf, count, datatype, MPI_ANY_SOURCE, MPI_ANY_TAG, comm, &status);
                for (int j = 0; j < count; j++) {
                    if (op == MPI_SUM) {
                        (static_cast<float*>(recvbuf))[j] += buf[j];
                    } else if (op == MPI_MAX) {
                        if ((static_cast<float*>(recvbuf))[j] < buf[j]) {
                            (static_cast<float*>(recvbuf))[j] = buf[j];
                        }
                    } else if (op == MPI_MIN) {
                        if ((static_cast<float*>(recvbuf))[j] > buf[j]) {
                            (static_cast<float*>(recvbuf))[j] = buf[j];
                        }
                    } else if (op == MPI_PROD) {
                        (static_cast<float*>(recvbuf))[j] *= buf[j];
                    } else {
                        return -3;
                    }
                }
            }
            delete[] buf;
        } else {
            return -4;
        }
    } else {
        MPI_Send(sendbuf, count, datatype, root, 0, comm);
    }

    return 0;
}
