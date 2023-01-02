// Copyright 2022 Nikiforova Lada
#include "../../../modules/task_3/nikiforova_l_n_dev_rectangle_method/n_dev_rectangle_method.h"

#include <mpi.h>

#include <cmath>
#include <functional>
#include <vector>

struct Integrall {
    std::vector<double> ADots;
    std::vector<double> BDots;
    std::function<double(std::vector<double>*)> foo;
    double result;
    double eps;
    int n;

    Integrall(const std::vector<double>& _ADots,
              const std::vector<double>& _BDots,
              std::function<double(std::vector<double>*)> _foo, double _eps) {
        if (ADots.size() != BDots.size())
            throw "Uncorrect data. Vector size is differ.";

        this->ADots = _ADots;
        this->BDots = _BDots;
        this->eps = _eps;
        this->foo = _foo;
        this->n = _ADots.size();
        std::vector<double> vectorCache;
        this->result = this->getResult(&vectorCache);
    }

    double getResult(std::vector<double>* vectorCache) {
        int currSize = vectorCache->size();
        if (currSize == this->n) return foo(vectorCache);
        double currX = this->ADots.at(currSize);
        double res = 0;
        while (currX < this->BDots.at(currSize)) {
            vectorCache->push_back(currX + this->eps);
            res += this->getResult(vectorCache) * this->eps;
            vectorCache->pop_back();
            currX += this->eps;
        }
        return res;
    }
};

double parallelIntegrall(const std::vector<double>& _ADots,
                         const std::vector<double>& _BDots,
                         std::function<double(std::vector<double>*)> _foo,
                         double _eps) {
    if (_ADots.size() != _BDots.size())
        throw "Uncorrect data. Vector size is differ.";
    int n = _ADots.size();
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<double> currADots(n);
    std::vector<double> currBDots(n);
    if (rank == 0) {
        double delta = (_BDots.at(0) - _ADots.at(0)) / size;
        double currX = _ADots.at(0);
        currADots.at(0) = currX;
        currBDots.at(0) = currX + delta;
        for (int j = 1; j < n; j++) {
            currADots.at(j) = _ADots.at(j);
            currBDots.at(j) = _BDots.at(j);
        }
        currX += delta;

        for (int i = 1; i < size; i++) {
            std::vector<double> sendADots;
            std::vector<double> sendBDots;
            sendADots.push_back(currX);
            sendBDots.push_back(currX + delta);
            for (int j = 1; j < n; j++) {
                sendADots.push_back(_ADots.at(j));
                sendBDots.push_back(_BDots.at(j));
            }
            MPI_Ssend(sendADots.data(), n, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
            MPI_Ssend(sendBDots.data(), n, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
            currX += delta;
        }
    } else {
        MPI_Status status;
        MPI_Recv(currADots.data(), n, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD,
                 &status);
        MPI_Recv(currBDots.data(), n, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD,
                 &status);
    }
    Integrall integrall(currADots, currBDots, _foo, _eps);
    double localResult = integrall.result;
    double globalResult = 0;
    MPI_Reduce(&localResult, &globalResult, 1, MPI_DOUBLE, MPI_SUM, 0,
               MPI_COMM_WORLD);
    return globalResult;
}
