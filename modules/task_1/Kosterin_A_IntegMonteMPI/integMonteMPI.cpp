// Copyright 2018 Nesterov Alexander
#include <iostream>
#include <stdio.h>
#include <Windows.h>
#include "mpi.h"
#include "integMonteMPI.h"

double monteCarlo(double low, double high, int count, double (*f)(double))
{
    int size, rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int delta = count / size;
    int ibeg = rank * delta + 1;
    int iend = (rank + 1) * delta;
    double res;
    float x, y;
    srand(time(NULL));
    double iter, res1;
    double d = f(low);
    for (iter = low; iter <= high; iter += (high - low) / 500.0)
    {
        // if (f(iter) < c)  c = f(iter);
        if (f(iter) > d)
            d = f(iter);
    }

    MPI_Bcast(&count, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    double sum = 0;

    for (int i = ibeg; i < iend; i++)
    {

        x = low + (rand()) * (high - low) / RAND_MAX;
        y = rand() * d / RAND_MAX;
        if (y <= f(x))
        {
            sum++;
        }
    }
    double localSum = sum * (high - low) * d / (iend - ibeg);
    MPI_Reduce(&localSum, &res, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    res = localSum;
    return res;
}