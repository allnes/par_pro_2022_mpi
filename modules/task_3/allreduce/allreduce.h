#include "mpi.h"
#include <cstdlib>
#include <iostream>
#include <math.h>
#include <sstream>
#ifndef ALLREDUCE_ALLREDUCE_H
#define ALLREDUCE_ALLREDUCE_H

namespace allR
{
    //Функция-обертка вокруг MPI_Allreduce
    double allreduce(double *, double *, int);
    //Функция-обертка вокруг MPI_Allreduce
    double allreduceRSAG(double *, double *, int);

}
#endif //ALLREDUCE_ALLREDUCE_H