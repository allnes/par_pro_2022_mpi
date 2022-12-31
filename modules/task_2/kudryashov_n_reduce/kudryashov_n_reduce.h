  // Copyright 2022 Kudryashov Nikita
#pragma once
#include <vector>

int reduce(void* sendbuf, void* recvbuf, int count, MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm);
template <class T>
std::vector<T> generateRandomVector(int size);
