// Copyright 2022 Mikerin Ilya
#include <mpi.h>
#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include "../../../modules/task_2/Mikerin_I_Contrast/Contrast.h"


std::vector<int> getRandomVector(int  sz) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::vector<int> vec(sz);
    for (int  i = 0; i < sz; i++) { vec[i] = gen() % 100; }
    return vec;
}

std::pair<int, int> getSequentialMaxMin(std::vector<int> vec) {
    const int  sz = vec.size();
    if ( sz > 0 ) {
        int max = vec[0], min = vec[0];
        for ( int i = 1; i < sz; i++ ) {
            if ( vec[i] > max) max = vec[i];
            if ( vec[i] < min) min = vec[i];
        }

        return std::make_pair(min, max);
    } else {
        return std::make_pair(2147483647, 0);
    }
}

int Clamp(int a, int l, int r) {
    if (a > r) return r;
    if (a < l) return l;
    return a;
}

std::vector<int> getSequentialContrast(std::vector<int> vec, const int max, const int min, const int B) { // NOLINT
    for (int i = 0; i < vec.size(); i++) {
        vec[i] = Clamp((vec[i] - min) * 255 / (max - min) + B, 0, 255);
    }
    return vec;
}

std::vector<std::vector<int>> SequentialContrast(std::vector<std::vector<int>> vec, int width, int height) {
    const int B = 50;
    int global_max = 0;
    int global_min = 0;
    std::vector<int> result;
    std::vector<std::vector<int>> realresult;
    std::vector<int> global_vec;

    global_vec = std::vector<int>(width * height);
    result = std::vector<int>(width * height);
    realresult = std::vector<std::vector<int>>(width);

    for ( int i = 0; i < width; i++ ) {
        realresult[i] = std::vector<int>(height);
    }

    for ( int i = 0; i < width * height ; i++ ) {
        global_vec[i] = vec[static_cast<int>(i/height)][i % height];
    }

    std::pair<int, int> min_max = (getSequentialMaxMin(global_vec));

    global_max = min_max.second;
    global_min = min_max.first;

    result = getSequentialContrast(global_vec, global_max, global_min, B);

    for ( int i = 0; i < width; i++ ) {
        for ( int j = 0; j < height; j++ ) {
            realresult[i][j] = result[i * height + j];
        }
    }

    return realresult;
}


std::vector<std::vector<int>> ParallelContrast(std::vector<std::vector<int>> vec, int width, int height) {
    int size, rank;
    const int B = 50;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int delta = static_cast<int>((width * height) / size);
    const int remainder = (width * height) % size;
    std::vector<int> result;
    std::vector<std::vector<int>> realresult;
    int* sendcount = new int[size];
    int* displs = new int[size];

    std::vector<int> global_vec = std::vector<int>(width * height);

    sendcount[0] = delta + static_cast<int>(remainder > 0);
    displs[0] = 0;

    if (remainder > 0) {
        for (int proc =1; proc < remainder ; proc++) {
            sendcount[proc] = delta + 1;
            displs[proc] = proc * delta + proc;
        }
        for (int proc = remainder; proc < size; proc++) {
            if (proc > 0) {
                sendcount[proc] = delta;
                displs[proc] = proc * delta + remainder;
            }
        }
    } else {
        for (int proc = 1; proc < size; proc++) {
            sendcount[proc] = delta;
            displs[proc] = proc * delta;
        }
    }


    result = std::vector<int>(width * height);
    realresult = std::vector<std::vector<int>>(width);

    for ( int i = 0; i < width; i++ ) {
        realresult[i] = std::vector<int>(height);
    }

    if (rank == 0) {
        for ( int i = 0; i < width * height; i++ ) {
            global_vec[i] = vec[static_cast<int>(i/height)][i % height];
        }
    }

    int global_max = 0;
    int global_min = 0;

    // Max and min search

    std::vector<int> local_vec(sendcount[rank]);

    // for (int i = 0; i< delta + remainder; i++) {
    //     local_vec.push_back(global_vec[i]);
    // }
    MPI_Scatterv(global_vec.data() , sendcount , displs , MPI_INT ,
                local_vec.data() , sendcount[rank] , MPI_INT , 0 , MPI_COMM_WORLD);

    std::pair<int, int> min_max = (getSequentialMaxMin(local_vec));

    MPI_Reduce(& (min_max.first), &global_min, 1, MPI_INT, MPI_MIN , 0, MPI_COMM_WORLD);
    MPI_Reduce(& (min_max.second), &global_max, 1, MPI_INT, MPI_MAX , 0, MPI_COMM_WORLD);


    MPI_Bcast(&global_max, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&global_min, 1, MPI_INT, 0, MPI_COMM_WORLD);

    local_vec = getSequentialContrast(local_vec, global_max, global_min, B);

    MPI_Gatherv(local_vec.data(), sendcount[rank], MPI_INT, result.data(),
                     sendcount, displs, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        for ( int i = 0; i < width; i++ ) {
            for ( int j = 0; j < height; j++ ) {
                realresult[i][j] = result[i * height + j];
            }
        }
    }



    return realresult;
}

