// Copyright 2022 Strogantsev Anton
#include <mpi.h>
#include <vector>
#include <algorithm>
#include "../../../modules/task_3/strogantsev_a_global_search/global_search.h"

const int maxIterationCount = 50000;

double getMiddle(double a, double b) {
    return (a + b) / 2.0;
}

double getParameterM(const std::vector<double>& points, std::function<double(double)> fun) {
    double M = std::numeric_limits<double>::min();
    for (size_t i = 1; i < points.size(); i++) {
        double z1 = fun(points[i]);
        double z0 = fun(points[i - 1]);
        double currentM = std::abs((z1 - z0) / (points[i] - points[i - 1]));
        M = std::max(currentM, M);
    }
    return M;
}

double getParameterLipschitz(double parameterM, double parameterR) {
    return (parameterM == 0) ? 1 : parameterR * parameterM;
}

std::vector<RAndIndex> getParametersR(
    const std::vector<double>& points,
    std::function<double(double)> fun,
    double parameter_m
) {
    std::vector<RAndIndex> characteristicR;
    for (size_t i = 1; i < points.size(); i++) {
        double z1 = fun(points[i]);
        double z0 = fun(points[i - 1]);
        double pointsDiff = (points[i] - points[i - 1]);
        double currentR = parameter_m * pointsDiff +
            (z1 - z0) * (z1 - z0) / (parameter_m * pointsDiff) -
            2 * (z1 + z0);
        characteristicR.push_back(std::make_pair(currentR, i));
    }
    return characteristicR;
}

double getNextX(
    double point0,
    double point1,
    std::function<double(double)> fun,
    double parameter_m
) {
    double z1 = fun(point0);
    double z0 = fun(point1);
    return point0 + (point1 - point0) / 2.0 +
        (z1 - z0) / (2.0 * parameter_m);
}

double getNextX(
    const std::vector<double>& points,
    std::function<double(double)> fun,
    int indexOfMaxR,
    double parameter_m
) {
    return getNextX(points[indexOfMaxR - 1], points[indexOfMaxR], fun, parameter_m);
}

double globalSearchParallel(std::function<double(double)> fun, double a, double b, double epsilone) {
    int rank, count;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &count);

    std::vector<double> sortedX = { a, b };

    int iterationCount = 0;
    while (iterationCount++ < maxIterationCount) {
        std::vector<double> inputBuffer(3);
        int workedProcessCount = count;

        if (rank == 0) {
            double M = getParameterM(sortedX, fun);
            double m = getParameterLipschitz(M, parameterR);
            auto characteristicR = getParametersR(sortedX, fun, m);

            std::sort(characteristicR.begin(), characteristicR.end(), [](const RAndIndex& p1, const RAndIndex& p2) {
                return p1.first > p2.first;
            });

            int indexOfMaxR = characteristicR.front().second;
            inputBuffer = { sortedX[indexOfMaxR - 1], sortedX[indexOfMaxR], m };

            workedProcessCount = std::min(static_cast<size_t>(count), characteristicR.size());
            for (int i = 1; i < workedProcessCount; i++) {
                RAndIndex r = characteristicR[i];
                std::vector<double> outputBuffer = { sortedX[r.second - 1], sortedX[r.second],  m };
                MPI_Send(outputBuffer.data(), 3, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
            }
        }

        MPI_Bcast(&workedProcessCount, 1, MPI_INT, 0, MPI_COMM_WORLD);

        if (rank != 0 && rank < workedProcessCount) {
            MPI_Status status;
            MPI_Recv(inputBuffer.data(), 3, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
        }

        bool isResultFinded = false;
        double result = 0.0;
        if (rank < workedProcessCount) {
            double nextX = getNextX(inputBuffer[0], inputBuffer[1], fun, inputBuffer[2]);

            int tag = TAG::UNFINDED;
            if (std::abs(nextX - inputBuffer[0]) < epsilone) {
                tag = TAG::FINDED;
            }

            if (rank != 0) {
                MPI_Send(&nextX, 1, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD);
            } else {
                for (int i = 1; i < workedProcessCount; i++) {
                    double inputNextX;
                    MPI_Status status;
                    MPI_Recv(&inputNextX, 1, MPI_DOUBLE, i, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

                    if (status.MPI_TAG == TAG::FINDED) {
                        result = inputNextX;
                        isResultFinded = true;
                    }

                    auto upperBound = std::upper_bound(sortedX.begin(), sortedX.end(), inputNextX);
                    sortedX.insert(upperBound, inputNextX);
                }

                if (tag == TAG::FINDED) {
                    result = nextX;
                    isResultFinded = true;
                }

                auto upperBound = std::upper_bound(sortedX.begin(), sortedX.end(), nextX);
                sortedX.insert(upperBound, nextX);
            }
        }

        MPI_Bcast(&isResultFinded, 1, MPI_BYTE, 0, MPI_COMM_WORLD);
        if (isResultFinded) return fun(result);
    }
    return std::numeric_limits<double>::max();
}

double globalSearchSequentially(std::function<double(double)> fun, double a, double b, double epsilone) {
    std::vector<double> sortedX = { a, b };

    while (sortedX.size() < maxIterationCount) {
        double M = getParameterM(sortedX, fun);
        double m = getParameterLipschitz(M, parameterR);
        auto characteristicR = getParametersR(sortedX, fun, m);

        std::sort(characteristicR.begin(), characteristicR.end(), [](const RAndIndex& p1, const RAndIndex& p2) {
            return p1.first > p2.first;
        });

        size_t indexOfMaxR = characteristicR.front().second;

        double nextX = getNextX(sortedX, fun, indexOfMaxR, m);

        if (std::abs(nextX - sortedX[indexOfMaxR - 1]) < epsilone)
            return fun(nextX);

        auto upperBound = std::upper_bound(sortedX.begin(), sortedX.end(), nextX);

        sortedX.insert(upperBound, nextX);
    }
    return std::numeric_limits<double>::max();
}
