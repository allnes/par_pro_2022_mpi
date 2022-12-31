// Copyright 2022 MUKHIN VADIM
#include "../../../modules/task_3/mukhin_v_graham_algorithm/ops_mpi.h"
#include <mpi.h>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <numeric>

bool cmp(Point a, Point b) { return a.x < b.x || a.x == b.x && a.y < b.y; }

bool cw(Point a, Point b, Point c) {
    return a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y) < 0;
}

bool ccw(Point a, Point b, Point c) {
    return a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y) > 0;
}

vector<Point> random(const vector<int>::size_type Size) {
    std::mt19937 gen(10);
    vector<Point> VertexVector(Size);
    for (vector<int>::size_type i = 0; i < Size; i++) {
        VertexVector[i] = Point(gen() % 10, gen() % 20);
    }
    return VertexVector;
}

void StructPoint(MPI_Datatype* structPoint) {
    int block[] = { 1, 1 };
    MPI_Aint displacements[] = { offsetof(Point, x), offsetof(Point, y) };
    MPI_Datatype Datatype[] = { MPI_INT, MPI_INT };
    MPI_Datatype type;
    MPI_Aint lb, extent;

    MPI_Type_create_struct(2, block, displacements, Datatype, &type);
    MPI_Type_get_extent(type, &lb, &extent);
    MPI_Type_create_resized(type, lb, extent, structPoint);
    MPI_Type_commit(structPoint);
}

vector<Point> GrahamMethod(vector<Point> VertexVector) {
    if (VertexVector.size() == 1) return VertexVector;
    sort(VertexVector.begin(), VertexVector.end(), &cmp);
    Point p1 = VertexVector[0], p2 = VertexVector.back();

    vector<Point> up, down;
    up.push_back(p1);
    down.push_back(p1);

    for (size_t i = 1; i < VertexVector.size(); ++i) {
        if (i == VertexVector.size() - 1 || cw(p1, VertexVector[i], p2)) {
            while (up.size() >= 2 &&
                !cw(up[up.size() - 2], up[up.size() - 1], VertexVector[i]))
                up.pop_back();
            up.push_back(VertexVector[i]);
        }
        if (i == VertexVector.size() - 1 || ccw(p1, VertexVector[i], p2)) {
            while (
                down.size() >= 2 &&
                !ccw(down[down.size() - 2], down[down.size() - 1], VertexVector[i]))
                down.pop_back();
            down.push_back(VertexVector[i]);
        }
    }
    vector<Point> Vertex;
    for (size_t i = 0; i < up.size(); ++i) Vertex.push_back(up[i]);
    for (size_t i = down.size() - 2; i > 0; --i) Vertex.push_back(down[i]);
    return Vertex;
}

vector<Point> parallelGrahamMethod(vector<Point> VertexVector,
    vector<int>::size_type vectorSize) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Datatype structPoint;
    StructPoint(&structPoint);

    if (rank == 0) {
        Point x = VertexVector[vectorSize - 1];
        if (vectorSize < size) {
            for (int i = vectorSize; i < size; i++) {
                VertexVector.push_back(x);
                vectorSize++;
            }
        }
    }

    int delta = vectorSize / size;
    if (delta == 0) delta = 1;

    vector<Point> localVectorOfVertex(delta);
    vector<Point> localGrahamMethod, globalGrahamMethod;

    MPI_Scatter(VertexVector.data(), delta, structPoint,
        localVectorOfVertex.data(), delta, structPoint, 0,
        MPI_COMM_WORLD);
    localGrahamMethod = GrahamMethod(localVectorOfVertex);

    if (rank != 0) {
        MPI_Send(localGrahamMethod.data(), localGrahamMethod.size(), structPoint, 0,
            0, MPI_COMM_WORLD);
    } else {
        for (int i = 1; i < size; i++) {
            MPI_Status status;
            int sendElements = 0;
            MPI_Probe(i, 0, MPI_COMM_WORLD, &status);
            MPI_Get_count(&status, structPoint, &sendElements);

            vector<int>::size_type oldSize = localGrahamMethod.size();
            localGrahamMethod.resize(oldSize + sendElements);
            MPI_Recv(localGrahamMethod.data() + oldSize, sendElements, structPoint, i,
                0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        if (size != 1) {
            int tail = vectorSize - size * delta;
            if (tail) {
                localVectorOfVertex = vector<Point>(
                    VertexVector.begin() + (vectorSize - tail), VertexVector.end());
                globalGrahamMethod = GrahamMethod(localVectorOfVertex);

                vector<int>::size_type oldSize = localGrahamMethod.size();
                localGrahamMethod.resize(oldSize + globalGrahamMethod.size());
                for (vector<int>::size_type i = oldSize, j = 0;
                    i < localGrahamMethod.size(); i++, j++) {
                    localGrahamMethod[i] = globalGrahamMethod[j];
                }
            }
        }
        globalGrahamMethod = GrahamMethod(localGrahamMethod);
    }

    return globalGrahamMethod;
}
