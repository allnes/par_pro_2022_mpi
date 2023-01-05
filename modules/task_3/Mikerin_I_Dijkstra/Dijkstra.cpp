// Copyright 2022 Mikerin Ilya
#include <mpi.h>
#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include "../../../modules/task_3/Mikerin_I_Dijkstra/Dijkstra.h"


int* getRandomVector(int  sz) {
    std::random_device dev;
    std::mt19937 gen(dev());
    int* vec = new int[sz];
    for (int  i = 0; i < sz; i++) { vec[i] = gen() % 100; }
    return vec;
}

void SequentialMinPathRecalculation(const int* vec, int* result, const int num, const int count, const bool* versh) {
    for ( int i = 0; i < count; i++ ) {
        if ( i != num && !(versh[i]) && vec[num*count + i] >= 0)
            result[i] = std::min(result[i], result[num] + vec[num*count + i]);
    }
}

int* SequentialDijkstra(int** vec, const int count, const int a) {
    int* result;
    int* global_vec;
    bool* versh;
    int min = 2147483647;
    int next = 0;

    global_vec = new int[count * count];
    result = new int[count];
    versh = new bool[count];

    for ( int i = 0; i < count * count; i++ ) {
        global_vec[i] = vec[static_cast<int>(i/count)][i% count];
    }



    for ( int i =0; i < count; i++ ) {
        result[i] = 2147483647;
        versh[i] = false;
    }
    result[a] = 0;

    SequentialMinPathRecalculation(global_vec, result, a, count, versh);

    versh[a] = true;
    for (int i = 0; i < count; i++) {
        if ( !(versh[i]) ) {
            if ( result[i] < min ) {
                min = result[i];
                next = i;
            }
        }
    }
    min = 2147483647;

    for ( int i = 0; i < count- 2; i++ ) {
        SequentialMinPathRecalculation(global_vec, result, next, count, versh);
        versh[next] = true;

        for ( int j = 0; j < count; j++ ) {
            if ( !(versh[j]) ) {
                if ( result[j] < min ) {
                    min = result[j];
                    next = j;
                }
            }
        }
        min = 2147483647;
    }


    return result;
}

void ParallelMinPathRecalculation(int* vec, int* result, const int num, const int count, const bool* versh,
                                    const int rank, const int delta, const int rem, const int size) {
    int* localres;
    int* localvec;
    int* sendcount = new int[size];
    int* displs = new int[size];
    int longToNext;

    if ( rank == 0 ) {
        longToNext = result[num];
    }

    MPI_Bcast(&longToNext, 1, MPI_INT, 0, MPI_COMM_WORLD);

    sendcount[0] = delta + static_cast<int>(rem > 0);
    displs[0] = 0;

    for ( int i = 1; i < rem ; i++ ) {
        sendcount[i] = delta + 1;
        displs[i] = delta * i + i;
    }

    for ( int i = rem; i < size; i++ ) {
        if (i != 0) {
            sendcount[i] = delta;
            displs[i] = delta * i + rem;
        }
    }

    localres = new int[sendcount[rank]];
    localvec = new int[sendcount[rank]];


    MPI_Scatterv(result , sendcount , displs , MPI_INT ,
                    localres , sendcount[rank] , MPI_INT , 0 , MPI_COMM_WORLD);

    MPI_Scatterv(vec + num * count , sendcount , displs , MPI_INT ,
                    localvec , sendcount[rank] , MPI_INT , 0 , MPI_COMM_WORLD);

    for ( int i = 0; i < sendcount[rank]; i++ ) {
        int index = (i + displs[rank]);
        if ( index != num && !(versh[index]) && localvec[i] >= 0 )
             localres[i] = std::min(localres[i], longToNext + localvec[i]);
    }

    MPI_Gatherv(localres, sendcount[rank], MPI_INT, result, sendcount, displs, MPI_INT, 0, MPI_COMM_WORLD);
}

int ParallelNext(int* result, const int rank, const bool* versh, const int size, const int delta, const int rem) {
    int* localres;
    int localmin = 2147483647;

    int* sendcount = new int[size];
    int* displs = new int[size];


    int* mins = new int[size];
    int* nexts = new int[size];
    int min = 0;
    int next = 0;

    sendcount[0] = delta + static_cast<int>(rem > 0);
    displs[0] = 0;

    for ( int i = 1; i < rem ; i++ ) {
        sendcount[i] = delta + 1;
        displs[i] = delta * i + i;
    }
    for ( int i = rem ; i < size; i++ ) {
        if ( i != 0 ) {
            sendcount[i] = delta;
            displs[i] = delta * i + rem;
        }
    }

    localres = new int[sendcount[rank]];

    MPI_Scatterv(result , sendcount , displs , MPI_INT ,
                    localres , sendcount[rank] , MPI_INT , 0 , MPI_COMM_WORLD);

    for (int i = 0; i < sendcount[rank]; i++) {
        if ( !(versh[i + displs[rank]]) ) {
            if ( localres[i] < localmin ) {
                localmin = localres[i];
                next = i + displs[rank];
            }
        }
    }

    MPI_Gather(&localmin, 1, MPI_INT, mins, 1, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Reduce(&localmin, &min, 1, MPI_INT, MPI_MIN , 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Gather(&next, 1, MPI_INT, nexts, 1, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);

    if ( rank == 0 ) {
        for ( int i = 0; i < size; i++ ) {
            if ( mins[i] == min ) {
                next = nexts[i];
                break;
            }
        }
    }

     MPI_Bcast(&next, 1, MPI_INT, 0, MPI_COMM_WORLD);

     return next;
}


int* ParallelDijkstra(int** vec, const int count, const int a) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int delta = static_cast<int>(count / size);
    const int remainder = count % size;
    int* result = new int[count];
    bool* versh = new bool[count];
    int next;

    int* global_vec = new int[count * count];

    if (rank == 0) {
        for ( int i = 0; i < count * count; i++ ) {
            global_vec[i] = vec[static_cast<int>(i/count)][i% count];
        }
    }

    if ( rank == 0 ) {
        for ( int i =0; i < count; i++ ) {
            result[i] = 2147483647;
        }
        result[a] = 0;
    }

    for ( int i =0; i < count; i++ ) {
        versh[i] = false;
    }

    ParallelMinPathRecalculation(global_vec, result, a, count, versh, rank, delta, remainder, size);
    versh[a] = true;

    next = ParallelNext(result, rank, versh, size, delta, remainder);

    for ( int i = 0; i < count - 2; i++ ) {
        ParallelMinPathRecalculation(global_vec, result, next, count, versh, rank, delta, remainder, size);
        versh[next] = true;

        next = ParallelNext(result, rank, versh, size, delta, remainder);
    }

    return result;
}

