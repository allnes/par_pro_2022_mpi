// Copyright 2023 Pankov Anatoliy
#include "../../../modules/task_3/pankov_a_jarvis/jarvis.h"

#include <mpi.h>

#include <algorithm>
#include <cmath>
#include <functional>
#include <iostream>
#include <random>
#include <string>
#include <vector>

void printDoubleVec(const std::vector<double>& vec) {
    int size = static_cast<int>(vec.size());
    std::cout << "{ ";
    for (int i = 0; i < size - 2; i += 2)
        std::cout << '(' << vec[i] << "; " << vec[i + 1] << "), ";
    std::cout << '(' << vec[size - 2] << "; " << vec[size - 1] << ") }";
}

std::vector<double> createRandomPoints(double xyMin, double xyMax,
                                       int p_count) {
    std::random_device dev;
    std::mt19937 rnd(dev());
    std::uniform_real_distribution<double> dist(xyMin, xyMax);
    std::vector<double> vec(2 * p_count);
    for (int i = 0; i < 2 * p_count; i++) vec[i] = dist(rnd);
    return vec;
}

std::vector<double> jarvisSequential(std::vector<double> points) {
    std::vector<double> result;

    // 1. We search (x_t, y_t) - point with minimum x and minimum y
    int t_id = 0;
    double x_t = points[0];
    double y_t = points[1];

    for (size_t i = 2; i < points.size(); i += 2) {
        if (points[i] < x_t || (points[i] == x_t && points[i + 1] < y_t)) {
            t_id = i;
            x_t = points[i];
            y_t = points[i + 1];
        }
    }

    double x_first = x_t;
    double y_first = y_t;

    // std::cout << "x_t = " << x_t << ", y_t = " << y_t << '\n';

    // Then, we exclude t = (x_t, y_t) from points
    std::swap(points[t_id], points[points.size() - 2]);
    std::swap(points[t_id + 1], points[points.size() - 1]);
    points.pop_back(), points.pop_back();

    // There Jarvis algorithm starts
    // (x_t, y_t) is the first in our hull
    result.push_back(x_first);
    result.push_back(y_first);

    auto isLess = [&x_t, &y_t](double x1, double y1, double x2, double y2) {
        double vx = x1 - x_t;
        double wx = x2 - x_t;

        double vy = y1 - y_t;
        double wy = y2 - y_t;

        double determ = (vx * wy) - (vy * wx);

        if (determ > 0) {
            return true;
        } else if (determ == 0) {
            return sqrt(pow(vx, 2) + pow(vy, 2)) >
                   sqrt(pow(wx, 2) + pow(wy, 2));
        }
        return false;
    };

    auto jarvisStep = [&x_t, &y_t, &points, &isLess]() {
        int next_point_pos = 0;

        for (int i = 2; i < points.size(); i += 2) {
            if (isLess(points[i], points[i + 1], points[next_point_pos],
                       points[next_point_pos + 1])) {
                next_point_pos = i;
            }
        }
        return next_point_pos;
    };

    // First Jarvis step
    int point_pos = jarvisStep();
    double x = points[point_pos];
    double y = points[point_pos + 1];

    // returning t back so the Jarvis won't cycle
    points.push_back(x_first);
    points.push_back(y_first);

    while (!(x == x_first && y == y_first)) {
        // Adding an already found point to the result
        result.push_back(x);
        result.push_back(y);

        // Exclude it from points
        std::swap(points[point_pos], points[points.size() - 2]);
        std::swap(points[point_pos + 1], points[points.size() - 1]);
        points.pop_back(), points.pop_back();

        // Moving t = (x_t, y_t) to the next point
        x_t = x, y_t = y;

        // Searching for the new next point
        point_pos = jarvisStep();
        x = points[point_pos];
        y = points[point_pos + 1];
    }

    return result;
}

std::vector<double> jarvisParallel(std::vector<double> points, int p_count) {
    int commsize, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int workers_count = std::min(commsize, p_count / 4);
    if (rank >= workers_count) return std::vector<double>({-1});

    const int c = p_count / workers_count;
    const int rem = p_count % workers_count;

    std::vector<double> local_res;
    std::vector<double> local_points(2 * c);

    if (rank == 0) {
        local_points =
            std::vector<double>(points.begin(), points.begin() + 2 * (c + rem));

        for (int wr = 1; wr < workers_count; wr++) {
            auto p_start = points.data() + 2 * (c + rem);
            MPI_Send(p_start + 2 * c * (wr - 1), 2 * c, MPI_DOUBLE, wr, 0,
                     MPI_COMM_WORLD);
        }
    } else {
        MPI_Status st;
        MPI_Recv(local_points.data(), 2 * c, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD,
                 &st);
    }

    local_res = jarvisSequential(local_points);

    if (rank != 0) {
        auto p_start = local_res.data();
        MPI_Send(p_start, static_cast<int>(local_res.size()), MPI_DOUBLE, 0, 0,
                 MPI_COMM_WORLD);
    } else {
        std::vector<double> points_processed(2 * p_count);
        int total_received_elements = 0;
        for (int i = 0; i < static_cast<int>(local_res.size()); i++)
            points_processed[i] = local_res[i];
        total_received_elements += local_res.size();

        for (int p_rank = 1; p_rank < workers_count; p_rank++) {
            auto p_start = points_processed.data();
            MPI_Status st;
            MPI_Recv(p_start + total_received_elements, 2 * c, MPI_DOUBLE,
                     p_rank, 0, MPI_COMM_WORLD, &st);
            int cur_received_count = 0;
            MPI_Get_count(&st, MPI_DOUBLE, &cur_received_count);

            total_received_elements += cur_received_count;
        }

        points_processed.resize(total_received_elements);

        return jarvisSequential(points_processed);
    }

    return std::vector<double>({-1});
}
