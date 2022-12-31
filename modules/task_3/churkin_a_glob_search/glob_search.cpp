// Copyright 2022 Churkin Alexander
#include "../../modules/task_3/churkin_a_glob_search/glob_search.h"

#include <mpi.h>

#include <algorithm>
#include <limits>
#include <vector>

auto idxCompareRule = [](const std::pair<double, int>& p1,
                         const std::pair<double, int>& p2) {
    return p1.first > p2.first;
};

double compute_M(const std::vector<double>& y,
                 std::function<double(double)> f) {
    double M = DOUBLE_MIN;
    for (size_t i = 1; i < y.size(); i++) {
        double z = f(y[i]), zPrev = f(y[i - 1]);
        double curM = std::abs((z - zPrev) / (y[i] - y[i - 1]));
        M = std::max(curM, M);
    }
    return M;
}

std::vector<std::pair<double, int>> compute_R(const std::vector<double>& y,
                                              std::function<double(double)> f,
                                              double m) {
    std::vector<std::pair<double, int>> R;
    for (size_t i = 1; i < y.size(); i++) {
        double z = f(y[i]);
        double zPrev = f(y[i - 1]);
        double dist = (y[i] - y[i - 1]);
        double curR =
            m * dist + (z - zPrev) * (z - zPrev) / (m * dist) - 2 * (z + zPrev);
        R.push_back(std::make_pair(curR, i));
    }
    return R;
}

double compute_x(double y0, double y1, std::function<double(double)> f,
                 double m) {
    double z = f(y0);
    double zPrev = f(y1);
    return y0 + (y1 - y0) / 2.0 + (z - zPrev) / (2.0 * m);
}

double globalSearchOneDimSequential(std::function<double(double)> f, double a,
                                    double b, double epsilon) {
    std::vector<double> x;
    x.push_back(a);
    x.push_back(b);

    while (x.size() < 65000) {
        double M = compute_M(x, f);
        double m = (M == 0) ? 1 : r * M;
        std::vector<std::pair<double, int>> R = compute_R(x, f, m);

        std::sort(R.begin(), R.end(), idxCompareRule);

        size_t idxOfMaxR = R.front().second;

        double newX = compute_x(x[idxOfMaxR - 1], x[idxOfMaxR], f, m);

        if (std::abs(newX - x[idxOfMaxR - 1]) < epsilon) return f(newX);

        x.insert(std::upper_bound(x.begin(), x.end(), newX), newX);
    }
    return DOUBLE_MAX;
}

double globalSearchOneDimParallel(std::function<double(double)> f, double a,
                                  double b, double epsilon) {
    int rank, proc_count;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &proc_count);

    std::vector<double> x;
    x.push_back(a);
    x.push_back(b);

    int iterCounter = 0;
    while (iterCounter < 65000) {
        int activeProcsCount = proc_count;
        std::vector<double> receivedData(3);

        iterCounter += 1;

        if (rank == 0) {
            double M = compute_M(x, f);
            double m = (M == 0) ? 1 : r * M;
            std::vector<std::pair<double, int>> R = compute_R(x, f, m);

            auto criteria = [](const std::pair<double, int>& p1,
                               const std::pair<double, int>& p2) {
                return p1.first > p2.first;
            };
            std::sort(R.begin(), R.end(), criteria);

            int idxOfMaxR = R.front().second;
            receivedData = {x[idxOfMaxR - 1], x[idxOfMaxR], m};

            activeProcsCount =
                std::min(static_cast<size_t>(proc_count), R.size());
            for (int i = 1; i < activeProcsCount; i++) {
                std::pair<double, int> RR = R[i];
                std::vector<double> sendData = {x[RR.second - 1], x[RR.second],
                                                m};
                MPI_Send(sendData.data(), 3, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
            }
        }

        MPI_Bcast(&activeProcsCount, 1, MPI_INT, 0, MPI_COMM_WORLD);

        if (rank != 0 && rank < activeProcsCount) {
            MPI_Status status;
            MPI_Recv(receivedData.data(), 3, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD,
                     &status);
        }

        bool hasResultFound = false;
        double result = 0.0;
        if (rank < activeProcsCount) {
            double newX =
                compute_x(receivedData[0], receivedData[1], f, receivedData[2]);

            int state = NOT_FOUND;
            if (std::abs(newX - receivedData[0]) < epsilon) state = FOUND;

            if (rank != 0) {
                MPI_Send(&newX, 1, MPI_DOUBLE, 0, state, MPI_COMM_WORLD);
            } else {
                for (int i = 1; i < activeProcsCount; i++) {
                    double receivedNewX;
                    MPI_Status status;
                    MPI_Recv(&receivedNewX, 1, MPI_DOUBLE, i, MPI_ANY_TAG,
                             MPI_COMM_WORLD, &status);

                    if (status.MPI_TAG == FOUND) {
                        result = receivedNewX;
                        hasResultFound = true;
                    }

                    x.insert(std::upper_bound(x.begin(), x.end(), receivedNewX),
                             receivedNewX);
                }

                if (state == FOUND) {
                    result = newX;
                    hasResultFound = true;
                }

                x.insert(std::upper_bound(x.begin(), x.end(), newX), newX);
            }
        }

        MPI_Bcast(&hasResultFound, 1, MPI_BYTE, 0, MPI_COMM_WORLD);
        if (hasResultFound) return f(result);
    }
    return DOUBLE_MAX;
}
