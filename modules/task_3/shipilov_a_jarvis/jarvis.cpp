// Copyright 2022 Shipilov Artem
#include <mpi.h>
#include <stddef.h>
#include <random>
#include <algorithm>
#include <numeric>
#include "../../../modules/task_3/shipilov_a_jarvis/jarvis.h"

int FindRotate(const Point2d& first, const Point2d& second, const Point2d& third) {
    return (second.x - first.x) * (third.y - second.y)
        - (second.y - first.y) * (third.x - second.x);
}

void StPoint2d(MPI_Datatype* stPoint2d) {
    int count = 2;
    int array_blocklengths[] = { 1, 1 };
    MPI_Datatype array_types[] = { MPI_INT, MPI_INT };
    MPI_Datatype tmp;
    MPI_Aint array_displacements[] = { offsetof(Point2d, x), offsetof(Point2d, y) };
    MPI_Aint l;
    MPI_Aint ex;

    MPI_Type_create_struct(count, array_blocklengths, array_displacements, array_types, &tmp);
    MPI_Type_get_extent(tmp, &l, &ex);
    MPI_Type_create_resized(tmp, l, ex, stPoint2d);
    MPI_Type_commit(stPoint2d);
}

vector<Point2d> Random(const vector<int>::size_type vecSize) {
    std::mt19937 gen(100);

    vector<Point2d> vecVertex(vecSize);
    for (vector<int>::size_type i = 0; i < vecSize; i++) {
        vecVertex[i] = Point2d(gen() % 10000, gen() % 10000);
    }

    return vecVertex;
}

vector<Point2d> SequenJarvis(const vector<Point2d>& vecVertex) {
    vector<int> vecIndex(vecVertex.size());
    std::iota(vecIndex.begin(), vecIndex.end(), 0);

    for (vector<int>::size_type i = 1; i < vecVertex.size(); ++i) {
        if (vecVertex[vecIndex[i]].x < vecVertex[vecIndex[0]].x) {
            std::swap(vecIndex[i], vecIndex[0]);
        }
    }

    vector<int> H = { vecIndex[0] };
    vecIndex.erase(vecIndex.begin());
    vecIndex.push_back(H[0]);

    while (true) {
        int right = 0;
        for (vector<int>::size_type i = 1; i < vecIndex.size(); i++) {
            if (FindRotate(vecVertex[H[H.size() - 1]],
                vecVertex[vecIndex[right]],
                vecVertex[vecIndex[i]]) < 0) {
                right = i;
            }
        }
        if (vecIndex[right] == H[0]) {
            break;
        } else {
            H.push_back(vecIndex[right]);
            vecIndex.erase(vecIndex.begin() + right);
        }
    }

    vector<Point2d> curveVertex(H.size());
    for (vector<int>::size_type i = 0; i < H.size(); i++) {
        curveVertex[i] = vecVertex[H[i]];
    }

    return curveVertex;
}
vector<Point2d> ParallelJarvis(const vector<Point2d>& vecVertex, vector<int>::size_type vecSize) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Datatype stPoint2d;
    StPoint2d(&stPoint2d);

    int size_new = vecSize / size;

    vector<Point2d> localVecVertex(size_new);
    vector<Point2d> localJarvis, globalJarvis;

    MPI_Scatter(vecVertex.data(), size_new, stPoint2d, localVecVertex.data(), size_new,
        stPoint2d, 0, MPI_COMM_WORLD);
    localJarvis = SequenJarvis(localVecVertex);

        if (rank != 0) {
            MPI_Send(localJarvis.data(), localJarvis.size(), stPoint2d, 0, 0, MPI_COMM_WORLD);
        } else { 
            for (int i = 1; i < size; i++) {
                MPI_Status status;
                int elements = 0;
                MPI_Probe(i, 0, MPI_COMM_WORLD, &status);
                MPI_Get_count(&status, stPoint2d, &elements);

                vector<int>::size_type previousSize = localJarvis.size();
                localJarvis.resize(previousSize + elements);
                MPI_Recv(localJarvis.data() + previousSize, elements,
                    stPoint2d, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
            if (size != 1) {
                int tail = vecSize - size * size_new;
                if (tail) {
                    localVecVertex = vector<Point2d>(vecVertex.begin() +
                        (vecSize - tail), vecVertex.end());
                    globalJarvis = SequenJarvis(localVecVertex);

                    vector<int>::size_type previousSize = localJarvis.size();
                    localJarvis.resize(previousSize + globalJarvis.size());
                    for (vector<int>::size_type i = previousSize, j = 0; i < localJarvis.size(); i++, j++) {
                        localJarvis[i] = globalJarvis[j];
                    }
                }
            }
            globalJarvis = SequenJarvis(localJarvis);
        }

    return globalJarvis;
}
