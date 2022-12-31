// Copyright 2022 Artemiev Aleksey
#include "../../../modules/task_3/artemiev_a_graham_alg/graham_alg.h"

#include <mpi.h>

#include <algorithm>
#include <cmath>
#include <functional>
#include <iostream>
#include <random>
#include <string>
#include <vector>

// Returns random array of n points: [x1, y1, x2, y2, ..., xn, yn]
std::vector<double> getRandomPoints(int count, double min, double max) {
    std::default_random_engine rnd{std::random_device {} ()};
    std::uniform_real_distribution<double> dist(min, max);
    std::vector<double> vec(2 * count);
    for (int i = 0; i < 2 * count; i++) vec[i] = dist(rnd);
    return vec;
}

void printPoints(const std::vector<double>& points) {
    int size = static_cast<int>(points.size());
    std::cout << "[ ";
    for (int i = 0; i < size - 2; i += 2) {
        std::cout << '(' << points[i] << ", " << points[i + 1] << "), ";
    }
    std::cout << '(' << points[size - 2] << ", " << points[size - 1] << ") ";
    std::cout << "]";
}

void sortPoints(std::vector<double>* points,
                std::function<bool(double, double, double, double)> less) {
    int n = static_cast<int>(points->size()) / 2;
    int i, j;
    for (i = 0; i < n - 1; i++)
        for (j = 0; j < n - i - 1; j++)
            if (!less((*points)[2 * j], (*points)[2 * j + 1],
                      (*points)[2 * j + 2], (*points)[2 * j + 3])) {
                std::swap((*points)[2 * j], (*points)[2 * j + 2]);
                std::swap((*points)[2 * j + 1], (*points)[2 * j + 3]);
            }
}

double rotate(double x1, double y1, double x2, double y2, double x3,
              double y3) {
    return (x2 - x1) * (y3 - y2) - (y2 - y1) * (x3 - x2);
}

std::vector<double> grahamSequential(std::vector<double> points) {
    std::vector<double> result;
    int count = static_cast<int>(points.size()) / 2;

    if (count < 2) throw "Count was < 2";

    double xMin = points[0];
    double yMin = points[1];
    int minId = 0;

    // At first, we search (xMin, yMin) - point with minimum x and minimum y
    for (int i = 2; i < 2 * count; i += 2)
        if (points[i] < xMin || (points[i] == xMin && points[i + 1] < yMin)) {
            xMin = points[i], yMin = points[i + 1];
            minId = i;
        }

    // std::cout << "\nxMin = " << xMin << ", yMin = " << yMin << '\n';

    // Exclude (xMin, yMin) from our points
    std::swap(points[minId], points[2 * count - 2]);
    std::swap(points[minId + 1], points[2 * count - 1]);
    points.pop_back(), points.pop_back();

    auto lessFunc = [xMin, yMin](double x1, double y1, double x2, double y2) {
        // Vector v1 coordinates
        double v1x = x1 - xMin;
        double v1y = y1 - yMin;
        // Vector v2 coordinates
        double v2x = x2 - xMin;
        double v2y = y2 - yMin;

        // Performing comparison by det
        double det = v1x * v2y - v1y * v2x;

        if (det > 0) {
            // v1 < v2 by det
            return true;
        } else if (det == 0) {
            // v1 == v2 by det. Let's compare by length of vectors
            double len_v1 = sqrt(pow(v1x, 2) + pow(v1y, 2));
            double len_v2 = sqrt(pow(v2x, 2) + pow(v2y, 2));

            return len_v1 < len_v2;
        } else {
            // v1 > v2 by det
            return false;
        }
    };

    // And sort other points in counter-clockwise order
    sortPoints(&points, lessFunc);

    // Then we perform Graham scan
    // (xMin, yMin) and (points[0], points[1])
    // are the first points of our convex hull
    result.push_back(xMin), result.push_back(yMin);
    result.push_back(points[0]), result.push_back(points[1]);

    for (int i = 2; i < 2 * count - 2; i += 2) {
        int sz = static_cast<int>(result.size());

        // p1
        double x1 = result[sz - 4];
        double y1 = result[sz - 3];
        // p2
        double x2 = result[sz - 2];
        double y2 = result[sz - 1];
        // p3
        double x3 = points[i];
        double y3 = points[i + 1];

        double rot = rotate(x1, y1, x2, y2, x3, y3);
        // p1, p2, p3 on one line -> replacing p2 with p3
        if (rot == 0) {
            result[sz - 2] = x3;
            result[sz - 1] = y3;
        } else if (rot < 0) {
            // While p1, p2, p3 form right turn -> excluding p2
            while (rotate(result[static_cast<int>(result.size()) - 4],
                          result[static_cast<int>(result.size()) - 3],
                          result[static_cast<int>(result.size()) - 2],
                          result[static_cast<int>(result.size()) - 1], x3,
                          y3) < 0)
                result.pop_back(), result.pop_back();
            result.push_back(x3);
            result.push_back(y3);
        } else {
            // left turns are ok - just adding p3 to our result
            result.push_back(x3);
            result.push_back(y3);
        }
    }

    return result;
}

std::vector<double> grahamParallel(std::vector<double> points, int count) {
    int comm_size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Every acting process processes at least 4 points
    const int actingProcessesCount = std::min(comm_size, count / 4);
    if (rank >= actingProcessesCount) return std::vector<double>(0);

    const int q = count / actingProcessesCount;
    const int rem = count % actingProcessesCount;

    std::vector<double> localPoints(2 * q);
    std::vector<double> localResult;

    if (rank == 0) {
        localPoints =
            std::vector<double>(points.begin(), points.begin() + 2 * (q + rem));

        // Distributing data to other active processes
        for (int i = 1; i < actingProcessesCount; i++) {
            MPI_Send(points.data() + 2 * (q + rem) + 2 * q * (i - 1), 2 * q, MPI_DOUBLE,
                     i, 0, MPI_COMM_WORLD);
        }
    } else {
        MPI_Status stat;
        MPI_Recv(localPoints.data(), 2 * q, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD,
                 &stat);
    }

    localResult = grahamSequential(localPoints);

    // Gathering all points on the root process
    if (rank != 0) {
        MPI_Send(localResult.data(), static_cast<int>(localResult.size()),
                 MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    } else {
        std::vector<double> processedPoints(2 * count);
        int receivedCount = 0;

        for (size_t i = 0; i < localResult.size(); i++)
            processedPoints[i] = localResult[i];
        receivedCount += localResult.size();

        for (int i = 1; i < actingProcessesCount; i++) {
            MPI_Status stat;
            MPI_Recv(processedPoints.data() + receivedCount, 2 * q, MPI_DOUBLE, i,
                     0, MPI_COMM_WORLD, &stat);
            int cnt = 0;
            MPI_Get_count(&stat, MPI_DOUBLE, &cnt);
            receivedCount += cnt;
        }

        processedPoints.resize(receivedCount);

        return grahamSequential(processedPoints);
    }

    return std::vector<double>(0);
}
